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
#include "common/vcschemamanager.h"
#include "menuhandler.h"
#include "ms_build/msbuildversionmanager.h"
#include "ms_build/vcmakestep.h"
#include "vcprojectbuildconfiguration.h"
#include "vcprojectbuildoptionspage.h"
#include "vcprojectkitinformation.h"
#include "vcprojectmanagerplugin.h"
#include "visual_studio_old_version/vcprojectmanager.h"
#include "visual_studio_new_version/vcxprojectmanager.h"
#include "visualstudiosolution/visualstudiosolutionmanager.h"

#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <utils/mimetypes/mimedatabase.h>
#include <projectexplorer/kitmanager.h>

#include <QtPlugin>

/*!
 * \class VcProjectManagerPlugin inheriates ExtensionSystem::IPlugin.
 * For the details about reimplemented functions see \class ExtensionSystem::IPlugin.
 */

namespace VcProjectManager {
namespace Internal {

VcProjectManagerPlugin::VcProjectManagerPlugin()
{
}

VcProjectManagerPlugin::~VcProjectManagerPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool VcProjectManagerPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize method, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    Utils::MimeDatabase::addMimeTypes(QLatin1String(":vcproject/VcProject.mimetypes.xml"));

    VcProjectBuildOptionsPage *confPage = new VcProjectBuildOptionsPage;
    addAutoReleasedObject(confPage);
    addAutoReleasedObject(new VcProjectManager(confPage));
    addAutoReleasedObject(new VcXProjectManager);
    addAutoReleasedObject(new VcProjectBuildConfigurationFactory);
    addAutoReleasedObject(new VcMakeStepFactory);
    addAutoReleasedObject(new MsBuildVersionManager);
    addAutoReleasedObject(new VcSchemaManager);
    addAutoReleasedObject(new MenuHandler);
    addAutoReleasedObject(new ToolDescriptionDataManager);
    addAutoReleasedObject(new VisualStudioSolutionManager);

    ProjectExplorer::KitManager::registerKitInformation(new VcProjectKitInformation);

    return true;
}

void VcProjectManagerPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized method, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag VcProjectManagerPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}


} // namespace VcProjectManager
} // namespace Internal
