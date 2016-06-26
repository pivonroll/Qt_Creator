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

#include <vcdebuging.h>

#include <visualstudiointerfaces/iconfiguration.h>
#include "../condition_parser/expression/evaluatearguments.h"

#include <utils/qtcassert.h>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConfigurationContainerX::ConfigurationContainerX(Project *project, QObject *parent)
    : IConfigurationContainer(parent),
      m_project(project)
{
}

ConfigurationContainerX::ConfigurationContainerX(const ConfigurationContainerX &configCont)
    : IConfigurationContainer(configCont)
{
    m_project = configCont.m_project;
}

ConfigurationContainerX::ConfigurationContainerX(ConfigurationContainerX &&other)
    : ConfigurationContainerX()
{
    swap(*this, other);
}

ConfigurationContainerX &ConfigurationContainerX::operator =(ConfigurationContainerX other)
{
    swap(*this, other);
    return *this;
}

void ConfigurationContainerX::addConfiguration(IConfiguration *config)
{
    ConfigurationX *configX = static_cast<ConfigurationX *>(config);

    QTC_ASSERT(configX, return);

    VS_DEBUG_ASSERT_ACTION(!findConfiguration(configX->fullName()),
                            QLatin1String("Configuration with name: ") + configX->fullName() + QLatin1String(" already exists"),
                            return);

    m_configs.append(configX);
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

IConfiguration *ConfigurationContainerX::configuration(const QString &fullName) const
{
    foreach (IConfiguration *config, m_configs) {
        if (config->fullName() == fullName)
            return config;
    }
    return nullptr;
}

IConfiguration *ConfigurationContainerX::configuration(int index) const
{
    QTC_ASSERT(0 <= index && index < m_configs.size(), return nullptr);
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

    ConfigurationX *newConfig = new ConfigurationX(m_project);

    ItemGroup *itemGroup = m_project->findItemGroupWithLabel(QLatin1String(PROJECT_CONFIGURATIONS));

    if (!itemGroup) {
        itemGroup = new ItemGroup;
        itemGroup->setLabel(QLatin1String(PROJECT_CONFIGURATIONS));
        m_project->addItemGroup(itemGroup);
    }

    Item *item = new Item;
    itemGroup->addItem(item);
    item->setName(QLatin1String(PROJECT_CONFIGURATION));

    QString include = QLatin1String("%1%2%3");
    include = include.arg(configDisplayName).arg(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER)).arg(platformName);
    item->setInclude(include);

    ItemMetaData *newMetaData = new ItemMetaData;
    newMetaData->setName(QLatin1String(CONFIGURATION));
    newMetaData->setValue(configDisplayName);
    item->addItemMetaData(newMetaData);

    newMetaData = new ItemMetaData;
    newMetaData->setName(QLatin1String(PLATFORM));
    newMetaData->setValue(platformName);
    item->addItemMetaData(newMetaData);

    // Configuration property group with label
    PropertyGroup *newPropertyGroup = new PropertyGroup;
    m_project->addPropertyGroup(newPropertyGroup);
    newPropertyGroup->setCondition(condition);
    newPropertyGroup->setLabel(QLatin1String(CONFIGURATION));

    // Configuration property group without any label
    newPropertyGroup = new PropertyGroup;
    m_project->addPropertyGroup(newPropertyGroup);
    newPropertyGroup->setCondition(condition);

    // Property sheets
    ImportGroup *importGroup = new ImportGroup;
    m_project->addImportGroup(importGroup);
    importGroup->setLabel(QLatin1String(PROPERTY_SHEETS));
    importGroup->setCondition(condition);

    ItemDefinitionGroup *itemDefinitionGroup = new ItemDefinitionGroup;
    m_project->addItemDefinitionGroup(itemDefinitionGroup);
    itemDefinitionGroup->setCondition(condition);

    // look for file build configurations (new Item group etc.)
    return newConfig;
}

IConfigurationContainer *ConfigurationContainerX::clone() const
{
    return new ConfigurationContainerX(*this);
}

void ConfigurationContainerX::copyDataFrom(IConfigurationContainer *config)
{
    ConfigurationContainerX *container = dynamic_cast<ConfigurationContainerX *>(config);
    if (container)
        *this = *container;
}

ConfigurationContainerX::ConfigurationContainerX()
    : m_project(nullptr)
{
}

void ConfigurationContainerX::swap(ConfigurationContainerX &first, ConfigurationContainerX &second)
{
    std::swap(first.m_configs, second.m_configs);
    std::swap(first.m_project, second.m_project);
}

ConfigurationX *ConfigurationContainerX::findConfiguration(const QString &configurationNameFull) const
{
    foreach (IConfiguration *config, m_configs) {
        if (config && config->fullName() == configurationNameFull)
            return static_cast<ConfigurationX *>(config);
    }

    return nullptr;
}

void ConfigurationContainerX::removeConfigurationFromProject(const QString &fullName)
{
    // remove items in the project associated with removed configuration
    ConfigurationX *config = findConfiguration(fullName);

    VS_DEBUG_ASSERT_ACTION(config != nullptr,
                           QLatin1String("Configuration with name: ") + fullName + QLatin1String(" does not exist"),
                           return);

    QStringList clist = fullName.split(QLatin1Char('|'));

    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(clist[0]));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(clist[1]));

    // delete from configuration item group
    ItemGroup *itemGroup = m_project->findItemGroupWithLabel(QLatin1String(PROJECT_CONFIGURATIONS));

    VS_DEBUG_ASSERT_ACTION(itemGroup, QLatin1String("Item group not found!"), return);

    Item *item = itemGroup->findItemWithInclude(config->evaluationValue());

    VS_DEBUG_ASSERT(item != nullptr, QLatin1String("Item does not exist!"));

    if (item) {
        itemGroup->removeItem(item);
        delete item;
    }

    // delete from property groups
    PropertyGroup *propertyGroup = m_project->findPropertyGroupWithConditionAndLabel(evalArgs);

    VS_DEBUG_ASSERT(propertyGroup != nullptr, QLatin1String("PropertyGroup not found!"));

    if (propertyGroup) {
        m_project->removePropertyGroup(propertyGroup);
        delete propertyGroup;
    }

    propertyGroup = m_project->findPropertyGroupWithConditionAndLabel(evalArgs, QLatin1String(CONFIGURATION));
    VS_DEBUG_ASSERT(propertyGroup != nullptr, QLatin1String("PropertyGroup not found!"));

    if (propertyGroup) {
        m_project->removePropertyGroup(propertyGroup);
        delete propertyGroup;
    }

    // delete from item definition groups groups
    ItemDefinitionGroup *itemDefinitionGroup = m_project->findItemDefinitionGroupWithCondition(evalArgs);
    VS_DEBUG_ASSERT(itemDefinitionGroup != nullptr, QLatin1String("ItemDefinitionGroup not found!"));

    if (itemDefinitionGroup) {
        m_project->removeItemDefinitionGroup(itemDefinitionGroup);
        delete itemDefinitionGroup;
    }

    // delete from import groups
    ImportGroup *importGroup = m_project->findImportGroupWithConditionAndLabel(evalArgs, QLatin1String(PROPERTY_SHEETS));
    VS_DEBUG_ASSERT(importGroup != nullptr, QLatin1String("ImportGroup not found!"));

    if (importGroup) {
        m_project->removeImportGroup(importGroup);
        delete importGroup;
    }

    m_configs.removeOne(config);
    delete config;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
