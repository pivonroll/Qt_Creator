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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATION_CONTAINER_X_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATION_CONTAINER_X_H

#include <visualstudiointerfaces/iconfigurationcontainer.h>

#include <QList>

namespace VcProjectManager {
namespace Internal {

class EvaluateArguments;

namespace VisualStudioProjectX {

class ConfigurationX;
class ImportGroup;
class Item;
class ItemDefinitionGroup;
class ItemGroup;
class Project;
class PropertyGroup;

class ConfigurationContainerX : public IConfigurationContainer
{
    Q_OBJECT

    friend class ConfigurationsX;

public:
    ConfigurationContainerX(Project *project, QObject *parent = 0);
    ConfigurationContainerX(const ConfigurationContainerX &configCont);
    ConfigurationContainerX(ConfigurationContainerX &&configCont);
    ConfigurationContainerX& operator=(ConfigurationContainerX configCont);

    void addConfiguration(IConfiguration *config);
    void removeConfiguration(const QString &fullName);
    IConfiguration *configuration(const QString &fullName) const;
    IConfiguration *configuration(int index) const;
    int configurationCount() const;
    IConfiguration *createNewConfiguration(const QString &configDisplayName, const QString &platformName);
    IConfigurationContainer *clone() const;
    void copyDataFrom(IConfigurationContainer *config);

private:
    ConfigurationContainerX();
    static void swap(ConfigurationContainerX &first, ConfigurationContainerX &second);
    ConfigurationX *findConfiguration(const QString &configurationNameFull) const;

    void removeConfigurationFromProject(const QString &fullName);

    Project *m_project;
    QList<IConfiguration *> m_configs;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATION_CONTAINER_X_H
