/**************************************************************************
**
** Copyright (c) 2016 Bojan Petrovic
** Copyright (c) 2016 Radovan Zivkovic
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/
#include "ms_build/msbuildversionmanager.h"
#include "vcprojectkitinformation.h"
#include "common/vcprojectmanagerconstants.h"

#include <widgets/pluginwidgets/vcprojectkitconfigwidget.h>

#include <projectexplorer/projectexplorerconstants.h>

#include <vcdebuging.h>
#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class VcProjectKitInformation
 * This class is used to add MsBuild executable in the Kit.
 * When Qt Creator's option dialog is opened, in Kit section,
 * you can see a combo box for selecting MsBuild executable.
 * This widget is done by \class VcProjectKitConfigWidget.
 */
VcProjectKitInformation::VcProjectKitInformation()
{
    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
    connect(msBVM, &MsBuildVersionManager::msBuildRemoved, this, &VcProjectKitInformation::onMSBuildRemoved);
    connect(msBVM, &MsBuildVersionManager::msBuildReplaced, this, &VcProjectKitInformation::onMSBuildReplaced);
}

VcProjectKitInformation::~VcProjectKitInformation()
{
}

/*!
 * \return id used for visual studio project kit informations.
 */
Core::Id VcProjectKitInformation::dataId() const
{
    static Core::Id id = Core::Id(Constants::VC_PROJECT_KIT_INFO_ID);
    return id;
}

unsigned int VcProjectKitInformation::priority() const
{
    return 100;
}

QVariant VcProjectKitInformation::defaultValue(ProjectExplorer::Kit *) const
{
    QList<MsBuildInformation *> msBuilds = MsBuildVersionManager::instance()->msBuildInformations();

    if (msBuilds.size() == 0)
        return QVariant();

    return msBuilds.at(0)->getId().toSetting();
}

QList<ProjectExplorer::Task> VcProjectKitInformation::validate(const ProjectExplorer::Kit *k) const
{
    QList<ProjectExplorer::Task> result;
    const MsBuildInformation* msBuild = msBuildInfo(k);

    if (!msBuild)
        result << ProjectExplorer::Task(ProjectExplorer::Task::Error, tr("No MS Build in kit."), Utils::FileName(), -1, Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM));
    else
        result << QList<ProjectExplorer::Task>();

    return result;
}

void VcProjectKitInformation::fix(ProjectExplorer::Kit *k)
{
    QTC_ASSERT(MsBuildVersionManager::instance(), return);
    if (msBuildInfo(k))
        return;

    VS_DEBUG_PRINT(QLatin1String("Ms Build is no longer known, removing from kit: ") + k->displayName());
    setMsBuild(k, nullptr); // make sure to clear out no longer known Ms Builds
}

void VcProjectKitInformation::setup(ProjectExplorer::Kit *k)
{
    Core::Id id = Core::Id::fromSetting(k->value(Core::Id(Constants::VC_PROJECT_KIT_INFO_ID)));

    if (!id.isValid())
        return;

    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
    MsBuildInformation *msBuild = msBVM->msBuildInformation(id);

    if (msBuild)
        return;

    QList<MsBuildInformation *> msBuilds = msBVM->msBuildInformations();

    if (msBuilds.size() > 0)
        setMsBuild(k, msBuilds.at(0));
}

ProjectExplorer::KitInformation::ItemList VcProjectKitInformation::toUserOutput(const ProjectExplorer::Kit *) const
{
    return ItemList() << qMakePair(tr("MsBuild Version"), tr("None"));
}

ProjectExplorer::KitConfigWidget *VcProjectKitInformation::createConfigWidget(ProjectExplorer::Kit *k) const
{
    return new VcProjectKitConfigWidget(k, this);
}

/*!
 * \return Information about MS Build set in the kit \a k.
 * Information is contained in \class MsBuildInformation.
 */
MsBuildInformation *VcProjectKitInformation::msBuildInfo(const ProjectExplorer::Kit *k)
{
    QTC_ASSERT(k, return nullptr);

    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
    return msBVM->msBuildInformation(Core::Id::fromSetting(k->value(Core::Id(Constants::VC_PROJECT_KIT_INFO_ID))));
}

/*!
 * \brief Sets MS Build executable to be used by kit \a k.
 * Ms Build executable is contained in \a msBuild.
 */
void VcProjectKitInformation::setMsBuild(ProjectExplorer::Kit *k, MsBuildInformation *msBuild)
{
    QTC_ASSERT(k, return);
    k->setValue(Core::Id(Constants::VC_PROJECT_KIT_INFO_ID), msBuild ? msBuild->getId().toSetting() : QVariant());
}

void VcProjectKitInformation::onMSBuildAdded(Core::Id msBuildId)
{
    Q_UNUSED(msBuildId);
    foreach (ProjectExplorer::Kit *k, ProjectExplorer::KitManager::kits()) {
        fix(k);
        notifyAboutUpdate(k);
    }
}

void VcProjectKitInformation::onMSBuildRemoved(Core::Id msBuildId)
{
    Q_UNUSED(msBuildId);
    foreach (ProjectExplorer::Kit *k, ProjectExplorer::KitManager::kits())
        fix(k);
}

void VcProjectKitInformation::onMSBuildReplaced(Core::Id oldMsBuildId, Core::Id newMsBuildId)
{
    Q_UNUSED(oldMsBuildId);
    foreach (ProjectExplorer::Kit *k, ProjectExplorer::KitManager::kits()) {
        fix(k);
        setMsBuild(k, MsBuildVersionManager::instance()->msBuildInformation(newMsBuildId));
        notifyAboutUpdate(k);
    }
}

} // namespace Internal
} // namespace VcProjectManager
