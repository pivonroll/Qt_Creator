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
#include "configurationbasewidget.h"

#include "configurationswidget.h"
#include "fileconfigurationseditwidget.h"
#include "../utils.h"
#include "../vcprojectmodel/tools/tool_constants.h"

#include <visualstudiointerfaces/iattributecontainer.h>
#include <visualstudiointerfaces/iattributedescriptiondataitem.h>
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
#include <visualstudiointerfaces/itooldescription.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/itoolsectiondescription.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>

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
    : m_file(file),
      m_vsProject(project)
{
    QTC_ASSERT(m_vsProject, return);
    QTC_ASSERT(m_vsProject->configurations(), return);
    QTC_ASSERT(m_vsProject->configurations()->configurationContainer(), return);

    m_buildConfigurations = m_vsProject->configurations()->configurationContainer()->clone();
    m_configsWidget = new ConfigurationsWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_configsWidget);
    setLayout(layout);

    readFileBuildConfigurations();

    IConfigurationContainer *fileConfigContainer = m_fileConfigurations[m_file];

    for (int i = 0; i < fileConfigContainer->configurationCount(); ++i) {
        IConfiguration *config = fileConfigContainer->configuration(i);
        if (config)
            m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
    }

    connect(m_configsWidget, SIGNAL(addNewConfigSignal(QString, QString)), this, SLOT(onAddNewConfig(QString, QString)));
    connect(m_configsWidget, SIGNAL(renameConfigSignal(QString,QString)), this, SLOT(onRenameConfig(QString, QString)));
    connect(m_configsWidget, SIGNAL(removeConfigSignal(QString)), this, SLOT(onRemoveConfig(QString)));
}

void FileConfigurationsEditWidget::saveData()
{
    IConfigurationContainer *newFileConfigContainer = m_fileConfigurations[m_file];

    QTC_ASSERT(newFileConfigContainer, return);

    for (int i = 0; i < newFileConfigContainer->configurationCount(); ++i) {
        VcNodeWidget *configWidget = m_configsWidget->configWidget(newFileConfigContainer->configuration(i)->fullName());
        if (configWidget)
            configWidget->saveData();
    }

    QMapIterator<IFile *, IConfigurationContainer *> it(m_fileConfigurations);

    while (it.hasNext()) {
        it.next();
        IFile *file = it.key();
        IConfigurationContainer *newConfigContainer = it.value();

        if (file && file->configurationContainer() && newConfigContainer)
            *file->configurationContainer() = *newConfigContainer;
    }

    QTC_ASSERT(m_vsProject, return);
    QTC_ASSERT(m_vsProject->configurations(), return);
    *m_vsProject->configurations()->configurationContainer() = *m_buildConfigurations;
}

void FileConfigurationsEditWidget::onAddNewConfig(QString newConfigName, QString copyFrom)
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

        if (m_vsProject->configurations()->configurationContainer()->configuration(newFullConfigName)) {
            QString message = tr("Configuration %1 already exists.");
            message = message.arg(newFullConfigName);
            errorMessages.append(message);
        }
        else {
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

        QString oldConfigName = copyFromConfigName + QLatin1Char('|') + platform->displayName();
        QString newConfigNamePl = newConfigName + QLatin1Char('|') + platform->displayName();
        IConfiguration *config = m_buildConfigurations->configuration(oldConfigName);

        if (config)
            config->setFullName(newConfigNamePl);

        QMapIterator<IFile *, IConfigurationContainer *> it(m_fileConfigurations);

        while (it.hasNext()) {
            it.next();
            config = it.value()->configuration(oldConfigName);
            if (config)
                config->setFullName(newConfigNamePl);
        }
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
        m_buildConfigurations->removeConfiguration(configName);
        m_configsWidget->removeConfiguration(configName);

        QMapIterator<IFile*, IConfigurationContainer*> it(m_fileConfigurations);

        while (it.hasNext()) {
            it.next();
            it.value()->removeConfiguration(configName);
        }
    }
}

void FileConfigurationsEditWidget::addConfigWidget(IConfiguration *config)
{
    QTC_ASSERT(config, return);
    m_configsWidget->addConfiguration(config->fullName(), config->createSettingsWidget());
}

void FileConfigurationsEditWidget::addConfigToProjectBuild(const QString &newConfigName, const QString &copyFrom)
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

void FileConfigurationsEditWidget::addConfigToFiles(const QString &newConfigName, const QString &copyFrom)
{
    QTC_ASSERT(newConfigName.isEmpty(), return);

    QMapIterator<IFile *, IConfigurationContainer *> it(m_fileConfigurations);

    while (it.hasNext()) {
        it.next();
        IConfigurationContainer *container = it.value();

        if (!container)
            continue;

        if (copyFrom.isEmpty()) {
            QStringList list = newConfigName.split(QLatin1Char('|'));
            IConfiguration *newConfig = m_buildConfigurations->createNewConfiguration(list[0], list[1]);
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
}

void FileConfigurationsEditWidget::readFileBuildConfigurations()
{
    IFiles *files = m_vsProject->files();

    for (int i = 0; i < files->fileContainerCount(); ++i) {
        IFileContainer *fileContainer = files->fileContainer(i);
        readFileBuildConfigurations(fileContainer);
    }

    for (int i = 0; i < files->fileCount(); ++i) {
        IFile *file = files->file(i);
        readFileBuildConfigurations(file);
    }
}

void FileConfigurationsEditWidget::readFileBuildConfigurations(IFileContainer *container)
{
    QTC_ASSERT(container, return);

    for (int i = 0; i < container->childCount(); ++i) {
        IFileContainer *fileContainer = container->fileContainer(i);
        readFileBuildConfigurations(fileContainer);
    }

    for (int i = 0; i < container->fileCount(); ++i) {
        IFile *file = container->file(i);
        readFileBuildConfigurations(file);
    }
}

void FileConfigurationsEditWidget::readFileBuildConfigurations(IFile *file)
{
    QTC_ASSERT(file, return);
    QTC_ASSERT(file->configurationContainer(), return);

    IConfigurationContainer *configCont = 0;

    if (file != m_file)
        configCont = file->configurationContainer()->clone();
    else {
        configCont = file->configurationContainer()->clone();
        connect(configCont, SIGNAL(configurationAdded(IConfiguration*)), this, SLOT(addConfigWidget(IConfiguration*)));
    }

    m_fileConfigurations[file] = configCont;
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
