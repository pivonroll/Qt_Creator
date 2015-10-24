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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATIONCONTAINERX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATIONCONTAINERX_H

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
    ConfigurationContainerX(ItemGroup *configurationsItemGroup, Project *project, QObject *parent = 0);
    ConfigurationContainerX(const ConfigurationContainerX &configCont);

    void addConfiguration(IConfiguration *config);
    void removeConfiguration(const QString &fullName);
    IConfigurationContainer &operator =(const IConfigurationContainer &configurationContainer);
    IConfiguration *configuration(const QString &fullName) const;
    IConfiguration *configuration(int index) const;
    int configurationCount() const;
    IConfiguration *createNewConfiguration(const QString &configDisplayName, const QString &platformName);
    IConfigurationContainer *clone() const;
private:
    ConfigurationX *createConfiguration(Item *item, ItemDefinitionGroup *itemDefGroup,
                                        const QList<PropertyGroup *> &propertyGroups,
                                        const QList<ImportGroup *> &importGroups);
    ConfigurationX *findConfiguration(const QString &configurationNameFull) const;

    void removeConfigurationFromPropertyGroups(const EvaluateArguments &evalArgs);
    void removeConfigurationFromImportGroups(const EvaluateArguments &evalArgs);
    void removeConfigurationFromItemDefinitionGroups(const EvaluateArguments &evalArgs);
    void removeConfigurationFromItemGroups(const EvaluateArguments &evalArgs);
    void removeConfigurationFromProject(const QString &fullName);

    void addConfigurationToPropertyGroups(const ConfigurationX *config);
    void addConfigurationToImportGroups(const ConfigurationX *config);
    void addConfigurationToItemDefinitions(const ConfigurationX *config);
    void addConfigurationToProject(const ConfigurationX *configX);

    int findPropertyGroupWithConfiguration(const QString &configFullName);
    int findImportGroupWithConfiguration(const QString &configFullName);
    int findItemDefinitionWithConfiguration(const QString &configFullName);
    ItemGroup *createConfigurationItemGroup();

    ItemGroup *m_configurationsItemGroup;
    Project *m_project;
    QList<IConfiguration *> m_configs;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATIONCONTAINERX_H
