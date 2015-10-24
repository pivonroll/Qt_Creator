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
#include "configurationcontainerx.h"

#include "private/itemdefinitiongroup.h"
#include "private/importgroup.h"
#include "private/item.h"
#include "private/itemgroup.h"
#include "private/itemmetadata.h"
#include "private/project.h"
#include "private/propertygroup.h"

#include "conditionmanipulation.h"
#include "configurationx.h"
#include "vcprojx_constants.h"
#include "utilsx.h"

#include <visualstudiointerfaces/iconfiguration.h>
#include "../../condition_parser/expression/evaluatearguments.h"

#include <utils/qtcassert.h>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConfigurationContainerX::ConfigurationContainerX(ItemGroup *configurationsItemGroup, Project *project, QObject *parent)
    : IConfigurationContainer(parent),
      m_configurationsItemGroup(configurationsItemGroup),
      m_project(project)
{
}

ConfigurationContainerX::ConfigurationContainerX(const ConfigurationContainerX &configCont)
    : IConfigurationContainer(configCont)
{
    m_configurationsItemGroup = new ItemGroup(*configCont.m_configurationsItemGroup);
    m_project = configCont.m_project;
}

void ConfigurationContainerX::addConfiguration(IConfiguration *config)
{
    ConfigurationX *configX = static_cast<ConfigurationX *>(config);

    if (!configX)
        return;

    if (findConfiguration(configX->fullName()))
        return;

    m_configs.append(configX);
    addConfigurationToProject(configX);
}

void ConfigurationContainerX::removeConfiguration(const QString &fullName)
{
    // if there is already a configuration with the same name
    foreach (IConfiguration *conf, m_configs) {
        if (conf->fullName() == fullName) {
            m_configs.removeOne(conf);
            emit configurationRemoved(fullName);
            delete conf;
        }
    }

    removeConfigurationFromProject(fullName);
}

IConfigurationContainer &ConfigurationContainerX::operator =(const IConfigurationContainer &configurationContainer)
{
    if (this != &configurationContainer) {
        m_configs.clear();

        for (int i = 0; i < configurationContainer.configurationCount(); ++i) {
            IConfiguration *config = configurationContainer.configuration(i);
            if (config)
                m_configs.append(config->clone());
        }
    }

    return *this;
}

IConfiguration *ConfigurationContainerX::configuration(const QString &fullName) const
{
    foreach (IConfiguration *config, m_configs) {
        if (config->fullName() == fullName)
            return config;
    }
    return 0;
}

IConfiguration *ConfigurationContainerX::configuration(int index) const
{
    QTC_ASSERT(0 <= index && index < m_configs.size(), return 0);
    return m_configs[index];
}

int ConfigurationContainerX::configurationCount() const
{
    return m_configs.size();
}

IConfiguration *ConfigurationContainerX::createNewConfiguration(const QString &configDisplayName, const QString &platformName)
{
    QString condition = QLatin1String("\'%1%2%3\'==\'%4%5%6\'");
    condition = condition.arg(QLatin1String(CONFIGURATION_VARIABLE)).arg(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER)).
            arg(QLatin1String(PLATFORM_VARIABLE)).arg(configDisplayName).arg(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER)).
            arg(platformName);

    ConfigurationX *newConfig = new ConfigurationX;
    newConfig->m_project = m_project;
    newConfig->m_item = new Item;
    newConfig->m_item->setName(QLatin1String(PROJECT_CONFIGURATION));

    QString include = QLatin1String("%1%2%3");
    include = include.arg(configDisplayName).arg(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER)).arg(platformName);
    newConfig->m_item->setInclude(include);

    ItemMetaData *newMetaData = new ItemMetaData;
    newMetaData->setName(QLatin1String(CONFIGURATION));
    newMetaData->setValue(configDisplayName);
    newConfig->m_item->addItemMetaData(newMetaData);

    newMetaData = new ItemMetaData;
    newMetaData->setName(QLatin1String(PLATFORM));
    newMetaData->setValue(platformName);
    newConfig->m_item->addItemMetaData(newMetaData);


    PropertyGroup *newPropertyGroup = new PropertyGroup;
    newPropertyGroup->setCondition(condition);
    newConfig->m_propertyGroups.append(newPropertyGroup);

    newPropertyGroup = new PropertyGroup(*newPropertyGroup);
    newPropertyGroup->setLabel(QLatin1String(CONFIGURATION));
    newConfig->m_propertyGroups.append(newPropertyGroup);

    ImportGroup *importGroup = new ImportGroup;
    importGroup->setLabel(QLatin1String(PROPERTY_SHEETS));
    importGroup->setCondition(condition);
    newConfig->m_importGroups.append(importGroup);

    newConfig->m_itemDefinitionGroup = new ItemDefinitionGroup;
    newConfig->m_itemDefinitionGroup->setCondition(condition);

    return newConfig;
}

IConfigurationContainer *ConfigurationContainerX::clone() const
{
    return new ConfigurationContainerX(*this);
}

ConfigurationX *ConfigurationContainerX::createConfiguration(Item *item,
                                                             ItemDefinitionGroup *itemDefGroup,
                                                             const QList<PropertyGroup *> &propertyGroups,
                                                             const QList<ImportGroup *> &importGroups)
{
    ConfigurationX *newConfig = new ConfigurationX;
    newConfig->m_item = item;
    newConfig->m_itemDefinitionGroup = itemDefGroup;
    newConfig->m_importGroups = importGroups;
    newConfig->m_propertyGroups = propertyGroups;
    newConfig->m_project = m_project;

    return newConfig;
}

ConfigurationX *ConfigurationContainerX::findConfiguration(const QString &configurationNameFull) const
{
    foreach (IConfiguration *config, m_configs) {
        if (config && config->fullName() == configurationNameFull)
            return static_cast<ConfigurationX *>(config);
    }

    return 0;
}

void ConfigurationContainerX::removeConfigurationFromPropertyGroups(const EvaluateArguments &evalArgs)
{
    for (int i = 0; i < m_project->propertyGroupCount();) {
        PropertyGroup *propertyGroup = m_project->propertyGroup(i);

        if (!propertyGroup)
            continue;

        ConditionManipulation condMan(propertyGroup->condition());
        if (condMan.evaluate(evalArgs))
            m_project->removePropertyGroup(propertyGroup);
        else
            ++i;
    }
}

void ConfigurationContainerX::removeConfigurationFromImportGroups(const EvaluateArguments &evalArgs)
{
    for (int i = 0; i < m_project->importGroupCount();) {
        ImportGroup *importGroup = m_project->importGroup(i);

        if (!importGroup)
            continue;

        ConditionManipulation condMan(importGroup->condition());
        if (condMan.evaluate(evalArgs))
            m_project->removeImportGroup(importGroup);
        else
            ++i;
    }
}

void ConfigurationContainerX::removeConfigurationFromItemDefinitionGroups(const EvaluateArguments &evalArgs)
{
    for (int i = 0; i < m_project->itemDefinitionGroupCount();) {
        ItemDefinitionGroup *itemDefGroup = m_project->itemDefinitionGroup(i);

        if (!itemDefGroup)
            continue;

        ConditionManipulation condMan(itemDefGroup->condition());
        if (condMan.evaluate(evalArgs))
            m_project->removeItemDefinitionGroup(itemDefGroup);
        else
            ++i;
    }
}

void ConfigurationContainerX::removeConfigurationFromItemGroups(const EvaluateArguments &evalArgs)
{
    for (int i = 0; i < m_project->itemGroupCount(); ++i) {
        ItemGroup *itemGroup = m_project->itemGroup(i);

        if (!itemGroup)
            continue;

        for (int j = 0; j < itemGroup->itemCount(); ++j) {
            Item *item = itemGroup->item(j);

            if (!item)
                continue;

            for (int k = 0; k < item->itemMetaDataCount();) {
                ItemMetaData *itemMetaData = item->itemMetaData(k);

                if (!itemMetaData)
                    continue;

                ConditionManipulation condMan(itemMetaData->condition());
                if (condMan.evaluate(evalArgs))
                    item->removeMetaData(itemMetaData);
                else
                    ++k;
            }
        }
    }
}

void ConfigurationContainerX::removeConfigurationFromProject(const QString &fullName)
{
    // remove items in the project associated with removed configuration
    QStringList clist = fullName.split(QLatin1Char('|'));

    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(clist[0]));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(clist[1]));

    for (int i = 0; i < m_configurationsItemGroup->itemCount(); ++i) {
        Item *item = m_configurationsItemGroup->item(i);

        if (!item)
            continue;

        ConditionManipulation condMan(item->condition());

        if (condMan.evaluate(evalArgs))
            m_configurationsItemGroup->removeItem(item);

    }

    removeConfigurationFromPropertyGroups(evalArgs);
    removeConfigurationFromImportGroups(evalArgs);
    removeConfigurationFromItemDefinitionGroups(evalArgs);
    // typical for files in the project
    removeConfigurationFromItemGroups(evalArgs);
}

void ConfigurationContainerX::addConfigurationToPropertyGroups(const ConfigurationX *config)
{
    QTC_ASSERT(config != 0, return);

    int foundIndex = findPropertyGroupWithConfiguration(config->fullName());

    if (foundIndex != -1)
        return;

    foreach (PropertyGroup *propertyGroup, config->m_propertyGroups)
        m_project->addPropertyGroup(propertyGroup);
}

void ConfigurationContainerX::addConfigurationToImportGroups(const ConfigurationX *config)
{
    QTC_ASSERT(config != 0, return);

    int foundIndex = findImportGroupWithConfiguration(config->fullName());

    if (foundIndex != -1)
        return;

    foreach (ImportGroup *importGroup, config->m_importGroups)
        m_project->addImportGroup(importGroup);
}

void ConfigurationContainerX::addConfigurationToItemDefinitions(const ConfigurationX *config)
{
    QTC_ASSERT(config != 0, return);

    int foundIndex = findItemDefinitionWithConfiguration(config->fullName());

    if (foundIndex != -1)
        return;

    m_project->addItemDefinitionGroup(config->m_itemDefinitionGroup);
}

void ConfigurationContainerX::addConfigurationToProject(const ConfigurationX *configX)
{
    addConfigurationToPropertyGroups(configX);
    addConfigurationToImportGroups(configX);
    addConfigurationToItemDefinitions(configX);

    ItemGroup *itemGroup = Utils::findItemGroupWithLabel(QLatin1String(PROJECT_CONFIGURATIONS), m_project);

    if (!itemGroup) {
        itemGroup = createConfigurationItemGroup();
        m_project->addItemGroup(itemGroup);
    }

    itemGroup->addItem(configX->m_item);
}

int ConfigurationContainerX::findPropertyGroupWithConfiguration(const QString &configFullName)
{
    QStringList clist = configFullName.split(QLatin1Char('|'));

    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(clist[0]));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(clist[1]));

    for (int i = 0; i < m_project->propertyGroupCount(); ++i) {
        PropertyGroup *propertyGroup = m_project->propertyGroup(i);

        if (!propertyGroup)
            continue;

        ConditionManipulation condMan(propertyGroup->condition());
        if (condMan.evaluate(evalArgs))
            return i;
    }

    return -1;
}

int ConfigurationContainerX::findImportGroupWithConfiguration(const QString &configFullName)
{
    QStringList clist = configFullName.split(QLatin1Char('|'));

    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(clist[0]));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(clist[1]));

    for (int i = 0; i < m_project->importGroupCount(); ++i) {
        ImportGroup *importGroup = m_project->importGroup(i);

        if (!importGroup)
            continue;

        ConditionManipulation condMan(importGroup->condition());
        if (condMan.evaluate(evalArgs))
            return i;
    }

    return -1;
}

int ConfigurationContainerX::findItemDefinitionWithConfiguration(const QString &configFullName)
{
    QStringList clist = configFullName.split(QLatin1Char('|'));

    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(clist[0]));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(clist[1]));

    for (int i = 0; i < m_project->itemDefinitionGroupCount(); ++i) {
        ItemDefinitionGroup *itemDefinitionGroup = m_project->itemDefinitionGroup(i);

        if (!itemDefinitionGroup)
            continue;

        ConditionManipulation condMan(itemDefinitionGroup->condition());
        if (condMan.evaluate(evalArgs))
            return i;
    }

    return -1;
}

ItemGroup *ConfigurationContainerX::createConfigurationItemGroup()
{
    ItemGroup *newItemGroup = new ItemGroup;
    newItemGroup->setLabel(QLatin1String(PROJECT_CONFIGURATIONS));

    return newItemGroup;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
