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
#include <visualstudiointerfaces/itooldescription.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <utils/qtcassert.h>

#include <QMessageBox>
#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class ConfigurationsEditWidget
 * Widget which represents all build configurations in Visual Studio project.
 */

ConfigurationsEditWidget::ConfigurationsEditWidget(VcProjectManager::Internal::IVisualStudioProject *vsProj, IConfigurationContainer *configContainer)
    : m_vsProject(vsProj)
{
    m_configsWidget = new ConfigurationsWidget;
    m_buildConfigurations = m_vsProject->configurations()->configurationContainer()->clone();

    if (configContainer == m_vsProject->configurations()->configurationContainer()) {
        connect(m_buildConfigurations, SIGNAL(configurationAdded(IConfiguration*)), this, SLOT(addConfigWidget(IConfiguration*)));

        for (int i = 0; i < m_buildConfigurations->configurationCount(); ++i) {
            IConfiguration *config = m_buildConfigurations->configuration(i);
            if (config)
                m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
        }
    }

    readFileBuildConfigurations();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_configsWidget);
    setLayout(layout);

    connect(m_configsWidget, SIGNAL(addNewConfigSignal(QString, QString)), this, SLOT(onAddNewConfig(QString, QString)));
    connect(m_configsWidget, SIGNAL(renameConfigSignal(QString,QString)), this, SLOT(onRenameConfig(QString, QString)));
    connect(m_configsWidget, SIGNAL(removeConfigSignal(QString)), this, SLOT(onRemoveConfig(QString)));
}

ConfigurationsEditWidget::~ConfigurationsEditWidget()
{
    delete m_buildConfigurations;

    QList<IConfigurationContainer *> fileConfigurations = m_fileConfigurations.values();
    qDeleteAll(fileConfigurations);
}

/*!
 * \brief Saves data for all build configurations in Visual Studio project.
 */
void ConfigurationsEditWidget::saveData()
{
    for (int i = 0; i < m_buildConfigurations->configurationCount(); ++i) {
        VcNodeWidget *configWidget = m_configsWidget->configWidget(m_buildConfigurations->configuration(i)->fullName());
        if (configWidget)
            configWidget->saveData();
    }

    IConfigurationContainer *configContainer = m_vsProject->configurations()->configurationContainer();
    *configContainer = *m_buildConfigurations;

    QMapIterator<IFile *, IConfigurationContainer *> it(m_fileConfigurations);

    while (it.hasNext()) {
        it.next();
        IFile *file = it.key();
        IConfigurationContainer *newConfigCont = it.value();

        if (file && newConfigCont) {
            IConfigurationContainer *oldConfigContainer = file->configurationContainer();
            *oldConfigContainer = *newConfigCont;
        }
    }
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

    for (int i = 0; i < platforms->platformCount(); ++i) {
        IPlatform *platform = platforms->platform(i);

        if (!platform)
            continue;

        QString oldConfigName = copyFromConfigName + QLatin1Char('|') + platform->displayName();
        QString newConfigNamePl = newConfigName + QLatin1Char('|') + platform->displayName();
        IConfiguration *config = m_buildConfigurations->configuration(oldConfigName);

        if (config)
            config->setFullName(newConfigNamePl);

        QMapIterator<IFile*, IConfigurationContainer*> it(m_fileConfigurations);

        while (it.hasNext()) {
            it.next();
            config = it.value()->configuration(oldConfigName);
            if (config)
                config->setFullName(newConfigNamePl);
        }
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
        m_buildConfigurations->removeConfiguration(configName);
        m_configsWidget->removeConfiguration(configName);

        QMapIterator<IFile*, IConfigurationContainer*> it(m_fileConfigurations);

        while (it.hasNext()) {
            it.next();
            it.value()->removeConfiguration(configName);
        }
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
 * \brief Reads file build configurations from all files in a project.
 */
void ConfigurationsEditWidget::readFileBuildConfigurations()
{
    QTC_ASSERT(m_vsProject, return);
    QTC_ASSERT(m_vsProject->files(), return);

    IFiles *files = m_vsProject->files();

    for (int i = 0; i < files->fileContainerCount(); ++i)
        readFileBuildConfigurations(files->fileContainer(i));

    for (int i = 0; i < files->fileCount(); ++i)
        readFileBuildConfigurations(files->file(i));
}

/*!
 * \brief Reads file build configurations from all files in a file container.
 */
void ConfigurationsEditWidget::readFileBuildConfigurations(IFileContainer *container)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i)
        readFileBuildConfigurations(container->fileContainer(i));

    for (int i = 0; i < container->fileCount(); ++i)
        readFileBuildConfigurations(container->file(i));
}

/*!
 * \brief Reads file build configurations from a file.
 */
void ConfigurationsEditWidget::readFileBuildConfigurations(IFile *file)
{
    QTC_ASSERT(file, return);
    QTC_ASSERT(file->configurationContainer(), return);

    IConfigurationContainer *configCont = file->configurationContainer()->clone();
    m_fileConfigurations[file] = configCont;
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
            m_buildConfigurations->addConfiguration(newConfig);
    } else {
        IConfiguration *config = m_buildConfigurations->configuration(copyFrom);

        if (config) {
            IConfiguration *newConfig = config->clone();
            newConfig->setFullName(newConfigName);
            m_buildConfigurations->addConfiguration(newConfig);
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
void ConfigurationsEditWidget::addConfigToFiles(const QString &newConfigName, const QString &copyFrom)
{
    QMapIterator<IFile *, IConfigurationContainer *> it(m_fileConfigurations);

    while (it.hasNext()) {
        it.next();
        IConfigurationContainer *container = it.value();

        if (!container)
            continue;
        IConfiguration *config = container->configuration(copyFrom);

        if (!config)
            continue;

        IConfiguration *newConfig = config->clone();
        if (newConfig) {
            newConfig->setFullName(newConfigName);
            container->addConfiguration(newConfig);
        }
    }
}


} // Internal
} // VcProjectManager
