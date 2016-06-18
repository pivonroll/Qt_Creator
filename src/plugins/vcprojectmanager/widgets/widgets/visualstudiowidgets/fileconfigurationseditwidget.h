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
#ifndef VCPROJECTMANAGER_INTERNAL_FILECONFIGURATIONS_EDIT_WIDGET_H
#define VCPROJECTMANAGER_INTERNAL_FILECONFIGURATIONS_EDIT_WIDGET_H

#include "vcnodewidget.h"

#include <QMap>

namespace VcProjectManager {
namespace Internal {

class IVisualStudioProject;
class ConfigurationsWidget;
class IFile;
class IConfigurationContainer;
class IFileContainer;
class IConfiguration;
class IConfigurationBuildTool;
class IToolSection;

class FileConfigurationsEditWidget : public VcNodeWidget
{
    Q_OBJECT

public:
    FileConfigurationsEditWidget(IFile *file, IVisualStudioProject *project);
    ~FileConfigurationsEditWidget();

    // VcNodeWidget interface
    void saveData();

private slots:
    void onAddNewConfig(QString newConfigName, QString copyFrom);
    void onNewConfigAdded(IConfiguration *config);
    void onRenameConfig(QString newConfigName, QString oldConfigNameWithPlatform);
    void onRemoveConfig(QString configNameWithPlatform);

private:
    void addConfigToProjectBuild(const QString &newConfigName, const QString &copyFrom);
    void addConfigToFiles(const QString &newConfigName, const QString &copyFrom);
    void addConfigToFiles(IFileContainer *container, const QString &newConfigName, const QString &copyFrom);
    void addConfigToFiles(IFile *file, const QString &newConfigName, const QString &copyFrom);

    void renameConfig(const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform);
    void renameConfig(IFileContainer *container, const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform);
    void renameConfig(IFile *file, const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform);

    void removeConfig(const QString &configNameWithPlatform);
    void removeConfig(IFileContainer *container, const QString &configNameWithPlatform);
    void removeConfig(IFile *file, const QString &configNameWithPlatform);

    void cleanUpConfig(IConfiguration *config);

    IVisualStudioProject *m_vsProject;
    ConfigurationsWidget *m_configsWidget;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_FILECONFIGURATIONSEDITWIDGET_H
