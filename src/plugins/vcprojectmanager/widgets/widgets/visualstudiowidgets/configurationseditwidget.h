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
#ifndef VCPROJECTMANAGER_INTERNAL_CONFIGURATIONSEDITWIDGET_H
#define VCPROJECTMANAGER_INTERNAL_CONFIGURATIONSEDITWIDGET_H

#include "vcnodewidget.h"

#include <QMap>

namespace VcProjectManager {
namespace Internal {

class IConfigurationContainer;
class ConfigurationsWidget;
class IConfiguration;
class IFile;
class IFileContainer;
class IVisualStudioProject;

class ConfigurationsEditWidget : public VcNodeWidget
{
    Q_OBJECT

public:
    ConfigurationsEditWidget(IVisualStudioProject *vsProj);
    ~ConfigurationsEditWidget();

    void saveData();

private slots:
    void onAddNewConfig(QString newConfigName, QString copyFrom);
    void onNewConfigAdded(IConfiguration *config);
    void onRenameConfig(QString newConfigName, QString oldConfigNameWithPlatform);
    void onRemoveConfig(QString configNameWithPlatform);

    void addConfigWidget(IConfiguration *config);

private:
    void addConfigToProjectBuild(const QString &newConfigName, const QString &copyFrom);
    void addConfigToFiles(const QString &newFullConfigName, const QString &copyFromFullConfigName);
    void addConfigToFiles(IFileContainer *container, const QString &newFullConfigName, const QString &copyFromFullConfigName);
    void addConfigToFiles(IFile *file, const QString &newFullConfigName, const QString &copyFromFullConfigName);
    void renameFileBuildConfiguration(const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform);
    void renameFileBuildConfiguration(IFileContainer *container, const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform);
    void renameFileBuildConfiguration(IFile *file, const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform);
    void removeConfigFromFiles(const QString &configNameWithPlatform);
    void removeConfigFromFiles(IFileContainer *container, const QString &configNameWithPlatform);
    void removeConfigFromFiles(IFile *file, const QString &configNameWithPlatform);

    IVisualStudioProject *m_vsProject;
    ConfigurationsWidget *m_configsWidget;
};

} // Internal
} // VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_CONFIGURATIONSEDITWIDGET_H
