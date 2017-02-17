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
#include "configurationseditwidget.h"

#include "configurationswidget.h"

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
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>

#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

// figure this out
#include <vcprojectmodel/configurationcontainer.h>
#include <vcprojectxmodel/configurationcontainerx.h>

#include <utils/qtcassert.h>

#include <QMessageBox>
#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class ConfigurationsEditWidget
 * Widget which represents all build configurations in Visual Studio project.
 */

ConfigurationsEditWidget::ConfigurationsEditWidget(VcProjectManager::Internal::IVisualStudioProject *vsProj)
    : m_vsProject(vsProj)
{
    m_configsWidget = new ConfigurationsWidget;
    IConfigurationContainer *buildConfigurations = m_vsProject->configurations()->configurationContainer();

    connect(buildConfigurations,
            &IConfigurationContainer::configurationAdded,
            this,
            &ConfigurationsEditWidget::addConfigWidget);

    for (int i = 0; i < buildConfigurations->configurationCount(); ++i) {
        IConfiguration *config = buildConfigurations->configuration(i);
        if (config)
            m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_configsWidget);
    setLayout(layout);

    connect(m_configsWidget, &ConfigurationsWidget::addNewConfigSignal, this, &ConfigurationsEditWidget::onAddNewConfig);
    connect(m_configsWidget, &ConfigurationsWidget::renameConfigSignal, this, &ConfigurationsEditWidget::onRenameConfig);
    connect(m_configsWidget, &ConfigurationsWidget::removeConfigSignal, this, &ConfigurationsEditWidget::onRemoveConfig);
}

ConfigurationsEditWidget::~ConfigurationsEditWidget()
{
}

/*!
 * \brief Saves data for all build configurations in Visual Studio project.
 */
void ConfigurationsEditWidget::saveData()
{
}

/*!
 * \brief Called when we create a copy of existing configuration and add it to a Visual Studio project.
 */
void ConfigurationsEditWidget::onAddNewConfig(QString newConfigName, QString copyFrom)
{
    QTC_ASSERT(m_vsProject->platforms(), return);

    IPlatforms *platforms = m_vsProject->platforms();
    QString copyFromConfigName = copyFrom.split(QLatin1Char('|')).at(0);

    QStringList errorMessages;

    for (int i = 0; i < platforms->platformCount(); ++i) {
        IPlatform *platform = platforms->platform(i);

        if (!platform)
            continue;

        QString newFullConfigName = newConfigName + QLatin1Char('|') + platform->displayName();
        QString copyFromFullConfigName;

        // if configuration with the same name already exists show error message
        if (m_vsProject->configurations()->configurationContainer()->configuration(newFullConfigName)) {
            QString message = tr("Configuration %1 already exists.");
            message = message.arg(newFullConfigName);
            errorMessages.append(message);
        } else {
            if (!copyFromConfigName.isEmpty())
                copyFromFullConfigName = copyFromConfigName + QLatin1Char('|') + platform->displayName();

            addConfigToProjectBuild(newFullConfigName, copyFromFullConfigName);
            addConfigToFiles(newFullConfigName, copyFromFullConfigName);
        }
    }

    if (errorMessages.size() <= 0)
        return;

    QString message = errorMessages.join(QLatin1Char('\n'));
    QMessageBox msg(this);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);

    if (errorMessages.size() == 1)
        msg.setWindowTitle(tr("An Error Occurred While Adding A Configuration"));
    else
        msg.setWindowTitle(tr("Cannot Add Configurations"));
    msg.setText(message);

    msg.exec();
}

/*!
 * \brief Called we new configuratio is added to a widget.
 */
void ConfigurationsEditWidget::onNewConfigAdded(IConfiguration *config)
{
    QTC_ASSERT(config, return);
    m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
}

/*!
 * \brief Called when configuration is renamed.
 */
void ConfigurationsEditWidget::onRenameConfig(QString newConfigName, QString oldConfigNameWithPlatform)
{
    QTC_ASSERT(m_vsProject->platforms(), return);

    IPlatforms *platforms = m_vsProject->platforms();
    QString copyFromConfigName = oldConfigNameWithPlatform.split(QLatin1Char('|')).at(0);

    // when renaming a configuration, do it for all platforms
    for (int i = 0; i < platforms->platformCount(); ++i) {
        IPlatform *platform = platforms->platform(i);

        if (!platform)
            continue;

        QString oldConfigNamePl = copyFromConfigName + QLatin1Char('|') + platform->displayName();
        QString newConfigNamePl = newConfigName + QLatin1Char('|') + platform->displayName();
        IConfiguration *config = m_vsProject->configurations()->configurationContainer()->configuration(oldConfigNamePl);

        if (config)
            config->setFullName(newConfigNamePl);

        renameFileBuildConfiguration(newConfigNamePl, oldConfigNamePl);
    }
}

/*!
 * \brief Called when configuration is removed.
 */
void ConfigurationsEditWidget::onRemoveConfig(QString configNameWithPlatform)
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

        removeConfigFromFiles(configNameWithPlatform);
    }
}

/*!
 * \brief Adds a configuration's widget.
 */
void ConfigurationsEditWidget::addConfigWidget(IConfiguration *config)
{
    QTC_ASSERT(config, return);
    m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
}

/*!
 * \brief Adds a configuration to a list of project build configurations.
 * Configuration will be named \a newConfigName.
 * \a copyFrom idetifies a configuration from which we want to copy settings.
 * If \a copyFrom is an empty string then new default configuration will be created and
 * add to a project.
 */
void ConfigurationsEditWidget::addConfigToProjectBuild(const QString &newConfigName, const QString &copyFrom)
{
    if (copyFrom.isEmpty()) {
        IConfiguration *newConfig = m_vsProject->createDefaultBuildConfiguration(newConfigName);
        if (newConfig)
            m_vsProject->configurations()->configurationContainer()->addConfiguration(newConfig);
    } else {
        IConfiguration *config = m_vsProject->configurations()->configurationContainer()->configuration(copyFrom);

        if (config) {
            IConfiguration *newConfig = config->clone();
            newConfig->setFullName(newConfigName);
            m_vsProject->configurations()->configurationContainer()->addConfiguration(newConfig);
        }
    }
}

/*!
 * \brief Adds a configuration to a list of file's build configurations.
 * Configuration will be named \a newConfigName.
 * \a copyFrom idetifies a configuration from which we want to copy settings.
 * If \a copyFrom is an empty string then new default configuration will be created and
 * add to a list of file's build configurations.
 */
void ConfigurationsEditWidget::addConfigToFiles(const QString &newFullConfigName, const QString &copyFromFullConfigName)
{
    QTC_ASSERT(m_vsProject, return);
    QTC_ASSERT(m_vsProject->files(), return);

    IFiles *files = m_vsProject->files();

    for (int i = 0; i < files->fileContainerCount(); ++i)
        renameFileBuildConfiguration(files->fileContainer(i), newFullConfigName, copyFromFullConfigName);

    for (int i = 0; i < files->fileCount(); ++i)
        renameFileBuildConfiguration(files->file(i), newFullConfigName, copyFromFullConfigName);
}

void ConfigurationsEditWidget::addConfigToFiles(IFileContainer *container, const QString &newFullConfigName, const QString &copyFromFullConfigName)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i)
        addConfigToFiles(container->fileContainer(i), newFullConfigName, copyFromFullConfigName);

    for (int i = 0; i < container->fileCount(); ++i)
        addConfigToFiles(container->file(i), newFullConfigName, copyFromFullConfigName);
}

void ConfigurationsEditWidget::addConfigToFiles(IFile *file, const QString &newFullConfigName, const QString &copyFromFullConfigName)
{
    QTC_ASSERT(file, return);
    QTC_ASSERT(file->configurationContainer(), return);

    IConfiguration *config = file->configurationContainer()->configuration(copyFromFullConfigName);

    if (!config)
        return;

    IConfiguration *newConfig = config->clone();
    if (newConfig) {
        newConfig->setFullName(newFullConfigName);
        file->configurationContainer()->addConfiguration(newConfig);
    }
}

void ConfigurationsEditWidget::renameFileBuildConfiguration(const QString &newConfigNameWithPlatform,
                                                            const QString &oldConfigNameWithPlatform)
{
    QTC_ASSERT(m_vsProject, return);
    QTC_ASSERT(m_vsProject->files(), return);

    IFiles *files = m_vsProject->files();

    for (int i = 0; i < files->fileContainerCount(); ++i)
        renameFileBuildConfiguration(files->fileContainer(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);

    for (int i = 0; i < files->fileCount(); ++i)
        renameFileBuildConfiguration(files->file(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);
}

void ConfigurationsEditWidget::renameFileBuildConfiguration(IFileContainer *container,
                                                            const QString &newConfigNameWithPlatform,
                                                            const QString &oldConfigNameWithPlatform)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i)
        renameFileBuildConfiguration(container->fileContainer(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);

    for (int i = 0; i < container->fileCount(); ++i)
        renameFileBuildConfiguration(container->file(i), newConfigNameWithPlatform, oldConfigNameWithPlatform);
}

void ConfigurationsEditWidget::renameFileBuildConfiguration(IFile *file, const
                                                            QString &newConfigNameWithPlatform,
                                                            const QString &oldConfigNameWithPlatform)
{
    QTC_ASSERT(file, return);

    IConfiguration *config = file->configurationContainer()->configuration(oldConfigNameWithPlatform);
    config->setFullName(newConfigNameWithPlatform);
}

void ConfigurationsEditWidget::removeConfigFromFiles(const QString &configNameWithPlatform)
{
    QTC_ASSERT(m_vsProject, return);
    QTC_ASSERT(m_vsProject->files(), return);

    IFiles *files = m_vsProject->files();

    for (int i = 0; i < files->fileContainerCount(); ++i)
        removeConfigFromFiles(files->fileContainer(i), configNameWithPlatform);

    for (int i = 0; i < files->fileCount(); ++i)
        removeConfigFromFiles(files->file(i), configNameWithPlatform);
}

void ConfigurationsEditWidget::removeConfigFromFiles(IFileContainer *container, const QString &configNameWithPlatform)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i)
        removeConfigFromFiles(container->fileContainer(i), configNameWithPlatform);

    for (int i = 0; i < container->fileCount(); ++i)
        removeConfigFromFiles(container->file(i), configNameWithPlatform);
}

void ConfigurationsEditWidget::removeConfigFromFiles(IFile *file, const QString &configNameWithPlatform)
{
    QTC_ASSERT(file, return);
    QTC_ASSERT(file->configurationContainer(), return);

    file->configurationContainer()->removeConfiguration(configNameWithPlatform);
}


} // Internal
} // VcProjectManager
