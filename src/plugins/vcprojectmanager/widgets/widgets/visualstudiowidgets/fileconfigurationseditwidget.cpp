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
#include "configurationbasewidget.h"

#include "configurationswidget.h"
#include "fileconfigurationseditwidget.h"
#include "../utils/utils.h"
#include <vcprojectmodel/tools/tool_constants.h>

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>
#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <visualstudiointerfaces/iattributecontainer.h>
#include <visualstudiointerfaces/iconfiguration.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/iconfigurationcontainer.h>
#include <visualstudiointerfaces/iconfigurations.h>
#include <visualstudiointerfaces/ifile.h>
#include <visualstudiointerfaces/ifilecontainer.h>
#include <visualstudiointerfaces/ifiles.h>
#include <visualstudiointerfaces/iplatform.h>
#include <visualstudiointerfaces/iplatforms.h>
#include <visualstudiointerfaces/isectioncontainer.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>

// figure this out
#include <vcprojectmodel/configurationcontainer.h>
#include <vcprojectxmodel/configurationcontainerx.h>

#include <utils/qtcassert.h>

#include <QMessageBox>
#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class FileConfigurationsEditWidget
 * Represents a widget for file build configuration.
 *
 * \fn void saveData()
 * \reimp
 */

FileConfigurationsEditWidget::FileConfigurationsEditWidget(IFile *file, IVisualStudioProject *project)
    : m_vsProject(project)
{
    QTC_ASSERT(m_vsProject, return);
    QTC_ASSERT(m_vsProject->configurations(), return);
    QTC_ASSERT(m_vsProject->configurations()->configurationContainer(), return);

    m_configsWidget = new ConfigurationsWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_configsWidget);
    setLayout(layout);    

    for (int i = 0; i < file->configurationContainer()->configurationCount(); ++i) {
        IConfiguration *config = file->configurationContainer()->configuration(i);
        m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
    }

    connect(m_configsWidget, &ConfigurationsWidget::addNewConfigSignal, this, &FileConfigurationsEditWidget::onAddNewConfig);
    connect(m_configsWidget, &ConfigurationsWidget::renameConfigSignal, this, &FileConfigurationsEditWidget::onRenameConfig);
    connect(m_configsWidget, &ConfigurationsWidget::removeConfigSignal, this, &FileConfigurationsEditWidget::onRemoveConfig);
}

FileConfigurationsEditWidget::~FileConfigurationsEditWidget()
{
}

void FileConfigurationsEditWidget::saveData()
{
}

void FileConfigurationsEditWidget::onAddNewConfig(QString newConfigName, QString copyFromFullName)
{
    QTC_ASSERT(m_vsProject->platforms(), return);

    IPlatforms *platforms = m_vsProject->platforms();
    QString copyFromConfigName = copyFromFullName.split(QLatin1Char('|')).at(0);

    QStringList errorMessages;

    // go through all platforms
    for (int i = 0; i < platforms->platformCount(); ++i) {
        IPlatform *platform = platforms->platform(i);

        QString newFullConfigName = newConfigName + QLatin1Char('|') + platform->displayName();
        QString copyFromFullConfigName;

        // check if configuration, we want to add, already exists
        if (m_vsProject->configurations()->configurationContainer()->configuration(newFullConfigName)) {
            QString message = tr("Configuration %1 already exists.");
            message = message.arg(newFullConfigName);
            errorMessages.append(message);
        }
        else {
            if (!copyFromConfigName.isEmpty())
                copyFromFullConfigName = copyFromConfigName + QLatin1Char('|') + platform->displayName();

            // add a configuration to the project
            addConfigToProjectBuild(newFullConfigName, copyFromFullConfigName);
            // add a configuration to files
            addConfigToFiles(newFullConfigName, copyFromFullConfigName);
        }
    }

    // display error messages
    if (errorMessages.size() <= 0)
        return;

    QString message = errorMessages.join(QLatin1Char('\n'));
    QMessageBox msg(this);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);

    if (errorMessages.size() == 1)
        msg.setWindowTitle(tr("Cannot Add Configuration"));
    else
        msg.setWindowTitle(tr("Cannot Add Configurations"));
    msg.setText(message);

    msg.exec();
}

void FileConfigurationsEditWidget::onNewConfigAdded(IConfiguration *config)
{
    QTC_ASSERT(config, return);
    m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
}

void FileConfigurationsEditWidget::onRenameConfig(QString newConfigName, QString oldConfigNameWithPlatform)
{
    QTC_ASSERT(m_vsProject->platforms(), return);

    IPlatforms *platforms = m_vsProject->platforms();
    QString copyFromConfigName = oldConfigNameWithPlatform.split(QLatin1Char('|')).at(0);

    for (int i = 0; i < platforms->platformCount(); ++i) {
        IPlatform *platform = platforms->platform(i);

        if (!platform)
            return;

        QString oldConfigNamePl = copyFromConfigName + QLatin1Char('|') + platform->displayName();
        QString newConfigNamePl = newConfigName + QLatin1Char('|') + platform->displayName();
        IConfiguration *config = m_vsProject->configurations()->configurationContainer()->configuration(oldConfigNamePl);

        if (config)
            config->setFullName(newConfigNamePl);

        renameConfig(newConfigNamePl, oldConfigNamePl);
    }
}

void FileConfigurationsEditWidget::onRemoveConfig(QString configNameWithPlatform)
{
    QTC_ASSERT(m_vsProject->platforms(), return);

    IPlatforms *platforms = m_vsProject->platforms();
    QString copyFromConfigName = configNameWithPlatform.split(QLatin1Char('|')).at(0);

    for (int i = 0; i < platforms->platformCount(); ++i) {
        IPlatform *platform = platforms->platform(i);

        if (!platform)
            continue;

        QString configName = copyFromConfigName + QLatin1Char('|') + platform->displayName();
        m_vsProject->configurations()->configurationContainer()->removeConfiguration(configName);
        m_configsWidget->removeConfiguration(configName);

        removeConfig(configName);
    }
}

void FileConfigurationsEditWidget::addConfigToProjectBuild(const QString &newConfigName, const QString &copyFrom)
{
    // if copy from is empty that means we want to add a default configuration
    if (copyFrom.isEmpty()) {
        IConfiguration *newConfig = m_vsProject->createDefaultBuildConfiguration(newConfigName);
        if (newConfig)
            m_vsProject->configurations()->configurationContainer()->addConfiguration(newConfig);
    } else {
        // else add a clone of the existing configuration with changed name
        IConfiguration *config = m_vsProject->configurations()->configurationContainer()->configuration(copyFrom);

        if (config) {
            IConfiguration *newConfig = config->clone();
            newConfig->setFullName(newConfigName);
            m_vsProject->configurations()->configurationContainer()->addConfiguration(newConfig);
        } else {

        }
    }
}

void FileConfigurationsEditWidget::addConfigToFiles(const QString &newConfigName, const QString &copyFrom)
{
    IFiles *files = m_vsProject->files();

    QTC_ASSERT(files, return);

    for (int i = 0; i < files->fileContainerCount(); ++i)
        addConfigToFiles(files->fileContainer(i), newConfigName, copyFrom);

    for (int i = 0; i < files->fileCount(); ++i)
        addConfigToFiles(files->file(i), newConfigName, copyFrom);
}

void FileConfigurationsEditWidget::addConfigToFiles(IFileContainer *container, const QString &newConfigName, const QString &copyFrom)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i)
        addConfigToFiles(container->fileContainer(i), newConfigName, copyFrom);

    for (int i = 0; i < container->fileCount(); ++i)
        addConfigToFiles(container->file(i), newConfigName, copyFrom);
}

void FileConfigurationsEditWidget::addConfigToFiles(IFile *file, const QString &newConfigName, const QString &copyFrom)
{
    QTC_ASSERT(file, return);
    QTC_ASSERT(file->configurationContainer(), return);

    IConfigurationContainer *container = file->configurationContainer();

    if (!container)
        return;

    if (copyFrom.isEmpty()) {
        QStringList list = newConfigName.split(QLatin1Char('|'));
        IConfiguration *newConfig = m_vsProject->configurations()->
                                                    configurationContainer()->createNewConfiguration(list[0], list[1]);
        VisualStudioUtils::leaveOnlyCppTool(newConfig);
        container->addConfiguration(newConfig);
    } else {
        IConfiguration *config = container->configuration(copyFrom);

        if (config) {
            IConfiguration *newConfig = config->clone();
            if (newConfig) {
                newConfig->setFullName(newConfigName);
                container->addConfiguration(newConfig);
            }
        }
    }
}

void FileConfigurationsEditWidget::renameConfig(const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform)
{
    IFiles *files = m_vsProject->files();

    QTC_ASSERT(files, return);

    for (int i = 0; i < files->fileContainerCount(); ++i)
        renameConfig(files->fileContainer(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);

    for (int i = 0; i < files->fileCount(); ++i)
        renameConfig(files->file(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);
}

void FileConfigurationsEditWidget::renameConfig(IFileContainer *container,
                                                const QString &newConfigNameWithPlatform,
                                                const QString &oldConfigNameWithPlatform)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i)
        renameConfig(container->fileContainer(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);

    for (int i = 0; i < container->fileCount(); ++i)
        renameConfig(container->file(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);
}

void FileConfigurationsEditWidget::renameConfig(IFile *file,
                                                const QString &newConfigNameWithPlatform,
                                                const QString &oldConfigNameWithPlatform)
{
    QTC_ASSERT(file, return);
    QTC_ASSERT(file->configurationContainer(), return);

    IConfigurationContainer *container = file->configurationContainer();

    if (!container)
        return;

    IConfiguration *config = container->configuration(oldConfigNameWithPlatform);
    if (config)
        config->setFullName(newConfigNameWithPlatform);
}

void FileConfigurationsEditWidget::removeConfig(const QString &configNameWithPlatform)
{
    IFiles *files = m_vsProject->files();

    QTC_ASSERT(files, return);

    for (int i = 0; i < files->fileContainerCount(); ++i)
        removeConfig(files->fileContainer(i), configNameWithPlatform);

    for (int i = 0; i < files->fileCount(); ++i)
        removeConfig(files->file(i), configNameWithPlatform);
}

void FileConfigurationsEditWidget::removeConfig(IFileContainer *container, const QString &configNameWithPlatform)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i)
        removeConfig(container->fileContainer(i), configNameWithPlatform);

    for (int i = 0; i < container->fileCount(); ++i)
        removeConfig(container->file(i), configNameWithPlatform);
}

void FileConfigurationsEditWidget::removeConfig(IFile *file, const QString &configNameWithPlatform)
{
    QTC_ASSERT(file, return);
    QTC_ASSERT(file->configurationContainer(), return);

    IConfigurationContainer *container = file->configurationContainer();

    if (!container)
        return;

    container->removeConfiguration(configNameWithPlatform);
}

void FileConfigurationsEditWidget::cleanUpConfig(IConfiguration *config)
{
    QTC_ASSERT(config, return);

    VcNodeWidget *configWidget = m_configsWidget->configWidget(config->fullName());

    if (configWidget)
        configWidget->saveData();

    IConfiguration *projectConfig = m_vsProject->configurations()->configurationContainer()->configuration(config->fullName());

    VisualStudioUtils::cleanUpConfigAttributes(config, projectConfig);
    VisualStudioUtils::cleanUpConfigTools(config, projectConfig);
}

} // namespace Internal
} // namespace VcProjectManager
