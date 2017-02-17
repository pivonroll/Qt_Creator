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
#include "msbuildversionmanager.h"
#include "../common/vcprojectmanagerconstants.h"

#include <utils/qtcassert.h>
#include <coreplugin/icore.h>

namespace VcProjectManager {
namespace Internal {

/*!
    \class MsBuildInformation
    \brief This class is used to store information about MSBuild executable.

    It stores the information about MSBuild executable's path and version.

    \fn Core::Id getId() const
        Returns an id. Id is used to uniquely identify objects of this class.

    \fn void setId(Core::Id id)
        Sets and id. Id is used to uniqely identify objects of this class type. Please do not use this function unless you are really sure of what you are doing.
*/

/*!
    \class MsBuildVersionManager
    \brief This class manages available MSBuilds which can be used to build the Visual Studio project.

    This class contains all available MSBuilds which user
    can use to build Visual Studio project.
    It is a \b singleton.
    When plugin is loaded this class reads data from Qt Creator's settings and lists all available MSBuilds from those settings.
    List of available MSBuilds is saved when Qt Creator is closed.
 */

/*!
    A static method that returns an instance of MsBuildVersionManager.
 */
MsBuildVersionManager *MsBuildVersionManager::instance()
{
    static MsBuildVersionManager instance;
    return &instance;
}

/*!
    Destructor.
 */
MsBuildVersionManager::~MsBuildVersionManager()
{
    qDeleteAll(m_msBuildInfos);
    m_msBuildInfos.clear();
}

/*!
    Adds MsBuildInformation to the list of available MSBuild(s).
    \a msBuildInfo contains all the information about MsBuildInformation that is added to the list.
    If the MsBuildInformation provided by \a msBuildInfo already exists this function will terminate
    and no new MsBuildInformation will be added to the list of available MsBuildInformations.
 * \param msBuildInfo
 * \return
 */
bool MsBuildVersionManager::addMsBuildInformation(MsBuildInformation *msBuildInfo)
{
    QTC_ASSERT(msBuildInfo, return false);

    foreach (MsBuildInformation *info, m_msBuildInfos) {
        if (info->m_executable == msBuildInfo->m_executable)
            return false;
    }

    m_msBuildInfos.append(msBuildInfo);
    emit msBuildAdded(msBuildInfo->getId());
    return true;
}

/*!
    Returns a list of available MsBuildInformations.
 */
QList<MsBuildInformation*> MsBuildVersionManager::msBuildInformations() const
{
    return m_msBuildInfos;
}

/*!
    Returns MsBuildInformation with a matching \a msBuildID.
    If MsBuildInformation with Id \a msBuildID does not exist in the list, then 0 (NULL pointer) is returned.

 */
MsBuildInformation *MsBuildVersionManager::msBuildInformation(Core::Id msBuildID)
{
    foreach (MsBuildInformation *info, m_msBuildInfos) {
        if (info->getId() == msBuildID)
            return info;
    }
    return nullptr;
}

/*!
    Returns a first MsBuildInformation match which \b version is in range [\a minVersion, \a maxVersion].
 */

MsBuildInformation *MsBuildVersionManager::msBuildInformation(MsBuildInformation::MsBuildVersion minVersion,
                                                              MsBuildInformation::MsBuildVersion maxVersion)
{
    foreach (MsBuildInformation *info, m_msBuildInfos) {
        if (info->m_msBuildVersion >= minVersion && info->m_msBuildVersion <= maxVersion)
            return info;
    }
    return nullptr;
}

/*!
    Removes MsBuildInformation which \b id matches \a msBuildId.
    If MsBuildInformation with \a msBuildId does not exists, nothing happens.
 */
void MsBuildVersionManager::removeMsBuildInformation(const Core::Id &msBuildId)
{
    for (int i = 0; i < m_msBuildInfos.size(); ++i) {
        MsBuildInformation *info = m_msBuildInfos[i];
        if (info->getId() == msBuildId) {
            m_msBuildInfos.removeOne(info);
            emit msBuildRemoved(msBuildId);
            delete info;
            return;
        }
    }
}

/*!
    Replaces MsBuildInformation with id \a targetMsBuild with MsBuildInformation \a newMsBuild.
    If MsBuildInformation with \a msBuildId does not exists, nothing happens.
 */
void MsBuildVersionManager::replace(Core::Id targetMsBuild, MsBuildInformation *newMsBuild)
{
    MsBuildInformation *oldMsBuild = msBuildInformation(targetMsBuild);

    QTC_ASSERT(oldMsBuild, return);
    int index = m_msBuildInfos.indexOf(oldMsBuild);
    m_msBuildInfos.replace(index, newMsBuild);
    delete oldMsBuild;
    emit msBuildReplaced(targetMsBuild, newMsBuild->getId());
}

/*!
    Saves the list of MsBuildInformations.
    This function is called when Qt Creator is terminating.
 */
void MsBuildVersionManager::saveSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginWriteArray(QLatin1String(VcProjectManager::Constants::VC_PROJECT_MS_BUILD_INFORMATIONS));
    for (int i = 0; i < m_msBuildInfos.size(); ++i) {
        settings->setArrayIndex(i);
        settings->setValue(QLatin1String(VcProjectManager::Constants::VC_PROJECT_MS_BUILD_EXECUTABLE), m_msBuildInfos[i]->m_executable);
        settings->setValue(QLatin1String(VcProjectManager::Constants::VC_PROJECT_MS_BUILD_EXECUTABLE_VERSION), m_msBuildInfos[i]->m_versionString);
    }
    settings->endArray();
}

/*!
    Creates MsBuildInformation for provided \a executablePath and \a version.
 */
MsBuildInformation *MsBuildVersionManager::createMsBuildInfo(const QString &executablePath, const QString &version)
{
    MsBuildInformation *newMsBuild = new MsBuildInformation(executablePath, version);

    // check if there is already a ms build with the same id, collision detection
    MsBuildInformation *info = MsBuildVersionManager::instance()->msBuildInformation(newMsBuild->getId());
    int i = 0;

    // if there is a id collision continue to generate id until unique id is created
    while (info) {
        QString argument = QString::number(i);
        QString temp = newMsBuild->m_executable + newMsBuild->m_versionString + argument;
        Core::Id newId(temp.toStdString().c_str());
        newMsBuild->setId(newId);
        info = MsBuildVersionManager::instance()->msBuildInformation(newMsBuild->getId());
        ++i;
    }

    return newMsBuild;
}

/*!
    Constructor.
 */
MsBuildVersionManager::MsBuildVersionManager()
{
    loadSettings();
}

/*!
    \internal
 */
void MsBuildVersionManager::loadSettings()
{
    qDeleteAll(m_msBuildInfos);
    m_msBuildInfos.clear();

    QSettings *settings = Core::ICore::settings();
    int size = settings->beginReadArray(QLatin1String(VcProjectManager::Constants::VC_PROJECT_MS_BUILD_INFORMATIONS));
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        MsBuildInformation *buildInfo = createMsBuildInfo(settings->value(QLatin1String(VcProjectManager::Constants::VC_PROJECT_MS_BUILD_EXECUTABLE)).toString(),
                                                          settings->value(QLatin1String(VcProjectManager::Constants::VC_PROJECT_MS_BUILD_EXECUTABLE_VERSION)).toString());
        m_msBuildInfos.append(buildInfo);
    }

    settings->endArray();
}

} // namespace Internal
} // namespace VcProjectManager
