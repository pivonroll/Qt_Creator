/**************************************************************************
**
** Copyright (c) 2014 Bojan Petrovic
** Copyright (c) 2014 Radovan Zivkovic
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
#include "vcmakestep.h"

#include "interfaces/iconfiguration.h"
#include "interfaces/iconfigurations.h"
#include "interfaces/iconfigurationcontainer.h"
#include "interfaces/ifile.h"
#include "interfaces/ifilecontainer.h"

#include "vcmakestep.h"
#include "vcprojectbuildconfiguration.h"
#include "vcprojectfile.h"
#include "vcprojectmanagerconstants.h"
#include "utils.h"
#include "vcprojectmodel/vcdocumentmodel.h"
#include "vcprojectmodel/vcprojectdocument.h"
#include "vcxprojectfile.h"

#include <utils/mimetypes/mimedatabase.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/gnumakeparser.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <projectexplorer/toolchain.h>
#include <qtsupport/qtkitinformation.h>
#include <projectexplorer/session.h>

#include <utils/fileutils.h>
#include <utils/qtcassert.h>

#include <QFileInfo>

using namespace ProjectExplorer;
using namespace VcProjectManager::Constants;

////////////////////////////////////
// VcProjectBuildConfiguration class
////////////////////////////////////
namespace VcProjectManager {
namespace Internal {

VcProjectBuildConfiguration::VcProjectBuildConfiguration(Target *parent) :
    BuildConfiguration(parent, Core::Id(VC_PROJECT_BC_ID))
{
    QFileInfo info(static_cast<VcProjectFile *>(parent->project()->document())->visualStudioProject()->filePath());
    m_buildDirectory = info.canonicalPath() + QLatin1String("-build");
}

VcProjectBuildConfiguration::~VcProjectBuildConfiguration()
{
}

NamedWidget *VcProjectBuildConfiguration::createConfigWidget()
{
    return new VcProjectBuildSettingsWidget;
}

/*!
 * \return destination build directory for this build configuration.
 */
QString VcProjectBuildConfiguration::buildDirectory() const
{
    return m_buildDirectory;
}

BuildConfiguration::BuildType VcProjectBuildConfiguration::buildType() const
{
    return Debug;
}

/*!
 * \brief Sets build configuration to \a config.
 */
void VcProjectBuildConfiguration::setConfiguration(IConfiguration *config)
{
    m_configuration = config;
    connect(m_configuration, SIGNAL(nameChanged()), this, SLOT(reloadConfigurationName()));
}

QVariantMap VcProjectBuildConfiguration::toMap() const
{
    return BuildConfiguration::toMap();
}

/*!
 * \brief This function will set display name and default display name for this
 * build configuration to a full name of an attached Visual Studio project's build configration.
 */
void VcProjectBuildConfiguration::reloadConfigurationName()
{
    setDisplayName(m_configuration->fullName());
    setDefaultDisplayName(m_configuration->fullName());
}

VcProjectBuildConfiguration::VcProjectBuildConfiguration(Target *parent, VcProjectBuildConfiguration *source)
    : BuildConfiguration(parent, source)
{
    cloneSteps(source);
}

bool VcProjectBuildConfiguration::fromMap(const QVariantMap &map)
{
    return BuildConfiguration::fromMap(map);
}

///////////////////////////////////////////
// VcProjectBuildConfigurationFactory class
///////////////////////////////////////////
VcProjectBuildConfigurationFactory::VcProjectBuildConfigurationFactory(QObject *parent)
    : IBuildConfigurationFactory(parent)
{
}

int VcProjectBuildConfigurationFactory::priority(const Target *parent) const
{
    return canHandle(parent) ? 0 : -1;
}

int VcProjectBuildConfigurationFactory::priority(const Kit *k, const QString &projectPath) const
{
    ::Utils::MimeDatabase mdb;
    if (k && mdb.mimeTypeForFile(projectPath).matchesName(QLatin1String(Constants::VCPROJ_MIMETYPE)))
        return 0;
    return -1;
}

QList<BuildInfo *> VcProjectBuildConfigurationFactory::availableBuilds(const Target *parent) const
{
    QList<BuildInfo *> result;

    VcProjectFile *vcProjectFile = qobject_cast<VcProjectFile *>(parent->project()->document());
    QTC_ASSERT(vcProjectFile, return result);

    IVisualStudioProject *vsProject = vcProjectFile->visualStudioProject();
    QTC_ASSERT(vsProject, return result);

    QTC_ASSERT(vsProject->configurations(), return result);
    QTC_ASSERT(vsProject->configurations()->configurationContainer(), return result);

    for (int i = 0; i < vsProject->configurations()->configurationContainer()->configurationCount(); ++i) {
        IConfiguration *config = vsProject->configurations()->configurationContainer()->configuration(i);
        QTC_ASSERT(config, continue);

        result << createBuildInfo(parent->kit(), config);
    }
    return result;
}

QList<BuildInfo *> VcProjectBuildConfigurationFactory::availableSetups(
        const Kit *k, const QString &projectPath) const
{
    Q_UNUSED(k);
    QList<BuildInfo *> result;

    VcDocConstants::DocumentVersion docVersion = Utils::getProjectVersion(projectPath);
    VcProjectDocument vsProject = VcProjectDocument(projectPath, docVersion);

    QTC_ASSERT(vsProject.configurations(), return result);
    QTC_ASSERT(vsProject.configurations()->configurationContainer(), return result);

    for (int i = 0; i < vsProject.configurations()->configurationContainer()->configurationCount(); ++i) {
        IConfiguration *config = vsProject.configurations()->configurationContainer()->configuration(i);
        QTC_ASSERT(config, continue);

        result << createBuildInfo(k, config);
    }

    return result;
}

VcProjectBuildConfiguration *VcProjectBuildConfigurationFactory::create(Target *parent, const BuildInfo *info) const
{
    QTC_ASSERT(parent, return 0);
    QTC_ASSERT(info->factory() == this, return 0);
    QTC_ASSERT(info->kitId == parent->kit()->id(), return 0);
    QTC_ASSERT(!info->displayName.isEmpty(), return 0);

    VcProjectFile *vcProjectFile = qobject_cast<VcProjectFile *>(parent->project()->document());
    QTC_ASSERT(vcProjectFile, return 0);

    VcProjectBuildConfiguration *bc = new VcProjectBuildConfiguration(parent);
    bc->setDisplayName(info->displayName);
    bc->setDefaultDisplayName(info->displayName);

    BuildStepList *buildSteps = bc->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
    BuildStepList *cleanSteps = bc->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN));
    QTC_ASSERT(buildSteps, return 0);
    QTC_ASSERT(cleanSteps, return 0);

    VcMakeStep *makeStep = new VcMakeStep(buildSteps);
    QString argument(QLatin1String("/p:configuration=\"") + info->displayName + QLatin1String("\""));
    makeStep->addBuildArgument(vcProjectFile->filePath().toString());
    makeStep->addBuildArgument(argument);
    buildSteps->insertStep(0, makeStep);

    makeStep = new VcMakeStep(cleanSteps);
    argument = QLatin1String("/p:configuration=\"") + info->displayName + QLatin1String("\" /t:Clean");
    makeStep->addBuildArgument(vcProjectFile->filePath().toString());
    makeStep->addBuildArgument(argument);
    cleanSteps->insertStep(0, makeStep);

    return bc;
}

bool VcProjectBuildConfigurationFactory::canClone(const Target *parent, BuildConfiguration *source) const
{
    return canHandle(parent) && source->id() == VC_PROJECT_BC_ID;
}

VcProjectBuildConfiguration *VcProjectBuildConfigurationFactory::clone(Target *parent, BuildConfiguration *source)
{
    if (!canClone(parent, source))
        return 0;

    VcProjectBuildConfiguration *old = static_cast<VcProjectBuildConfiguration *>(source);
    return new VcProjectBuildConfiguration(parent, old);
}

IVisualStudioProject *VcProjectBuildConfigurationFactory::findVisualStudioProject(const QString &projectPath) const
{
    foreach(Project *project, SessionManager::projects()) {
        if (project && project->projectFilePath().toString() == projectPath) {
            Core::IDocument *document = project->document();

            VcProjectFile *vcProjectFile = dynamic_cast<VcProjectFile *>(document);

            if (vcProjectFile)
                return vcProjectFile->visualStudioProject();

            VcXProjectFile *vcXProjectFile = dynamic_cast<VcXProjectFile *>(document);

            if (vcXProjectFile)
                return vcXProjectFile->visualStudioProject();
        }
    }

    return 0;
}

bool VcProjectBuildConfigurationFactory::canRestore(const Target *parent, const QVariantMap &map) const
{
    return canHandle(parent) && idFromMap(map) == VC_PROJECT_BC_ID;
}

VcProjectBuildConfiguration *VcProjectBuildConfigurationFactory::restore(Target *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;

    VcProjectBuildConfiguration *bc = new VcProjectBuildConfiguration(parent);
    if (bc->fromMap(map))
        return bc;
    delete bc;
    return 0;
}

/*!
 * \return \b true if this project supports selected build kit.
 * Otherwise it returns \b false.
 */
bool VcProjectBuildConfigurationFactory::canHandle(const Target *t) const
{
    QTC_ASSERT(t, return false);
    if (!t->project()->supportsKit(t->kit()))
        return false;
    return qobject_cast<VcProject *>(t->project());
}

/*!
 * \return a build information contained in the \a BuildInfo object.
 * Build information will contain:
 * a full name of a Visual Studio project's build configuration,
 * id of a kit that will be used to build Visual Studio project,
 * support for shadow builds set to \b true,
 * type name of BuildInfo object will be set to a full name of a
 * Visual Studio project's build configuration.
 */
BuildInfo *VcProjectBuildConfigurationFactory::createBuildInfo(const ProjectExplorer::Kit *k,
                                                               IConfiguration *config) const
{
    ProjectExplorer::BuildInfo *buildInfo = new ProjectExplorer::BuildInfo(this);
    buildInfo->displayName = config->fullName();
    buildInfo->kitId = k->id();
    buildInfo->typeName = config->fullName();

    return buildInfo;
}

} // namespace Internal
} // namespace VcProjectManager
