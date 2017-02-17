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
#include "configurationsx.h"

#include "private/itemmetadata.h"
#include "private/itemdefinitiongroup.h"
#include "private/importgroup.h"
#include "private/itemgroup.h"
#include "private/item.h"
#include "private/project.h"
#include "private/propertygroup.h"

#include "configurationcontainerx.h"
#include "configurationx.h"
#include "conditionmanipulation.h"
#include "utilsx.h"
#include "vcprojx_constants.h"
#include "../condition_parser/expression/evaluatearguments.h"

#include <QDomNode>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConfigurationsX::ConfigurationsX(Project *project)
    : m_project(project)
{
    m_configurationContainer = new ConfigurationContainerX(project);

    processItemGroup();
}

ConfigurationsX::ConfigurationsX(const ConfigurationsX &other)
{
    m_configurationContainer = new ConfigurationContainerX(*other.m_configurationContainer);
    m_project = other.m_project;
}

ConfigurationsX::ConfigurationsX(ConfigurationsX &&other)
    : ConfigurationsX()
{
    swap(*this, other);
}

ConfigurationsX &ConfigurationsX::operator=(ConfigurationsX other)
{
    swap(*this, other);
    return *this;
}

IConfigurationContainer *ConfigurationsX::configurationContainer() const
{
    return m_configurationContainer;
}

VcNodeWidget *ConfigurationsX::createSettingsWidget()
{
    return nullptr;
}

void ConfigurationsX::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

QDomNode ConfigurationsX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument)
    return QDomNode();
}

void ConfigurationsX::swap(ConfigurationsX &first, ConfigurationsX &second)
{
    std::swap(first.m_configurationContainer, second.m_configurationContainer);
    std::swap(first.m_project, second.m_project);
}

void ConfigurationsX::processItemGroup()
{
    ItemGroup *itemGroup = m_project->findItemGroupWithLabel(QLatin1String(PROJECT_CONFIGURATIONS));

    if (!itemGroup)
        return;

    for (int i = 0; i < itemGroup->itemCount(); ++i) {
        Item *item = itemGroup->item(i);

        if (!item)
            continue;

        ConfigurationX *config = new ConfigurationX(m_project);
        config->setEvaluationValue(item->include());
        m_configurationContainer->m_configs.append(config);
    }
}

QList<PropertyGroup *> ConfigurationsX::findConfigurationPropertyGroups(const QString &configName, const QString &platform)
{
    QList<PropertyGroup *> propertyGroups;
    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(configName));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(platform));

    for (int i = 0; i < m_project->propertyGroupCount(); ++i) {
        PropertyGroup *propertyGroup = m_project->propertyGroup(i);

        if (!propertyGroup)
            continue;

        ConditionManipulation condMan(propertyGroup->condition());

        if (condMan.evaluate(evalArgs))
            propertyGroups.append(propertyGroup);
    }

    return propertyGroups;
}

QList<ImportGroup *> ConfigurationsX::findConfigurationImportGroups(const QString &configName, const QString &platform)
{
    QList<ImportGroup *> importGroups;
    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(configName));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(platform));

    for (int i = 0; i < m_project->importGroupCount(); ++i) {
        ImportGroup *importGroup = m_project->importGroup(i);

        if (!importGroup)
            continue;

        ConditionManipulation condMan(importGroup->condition());

        if (condMan.evaluate(evalArgs))
            importGroups.append(importGroup);
    }

    return importGroups;
}

ItemDefinitionGroup *ConfigurationsX::findConfigurationDefGroup(const QString &configName, const QString &platform)
{
    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), configName);
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), platform);

    for (int i = 0; i < m_project->itemDefinitionGroupCount(); ++i) {
        ItemDefinitionGroup *itemDefGroup = m_project->itemDefinitionGroup(i);

        if (!itemDefGroup)
            continue;

        ConditionManipulation condMan(itemDefGroup->condition());

        if (condMan.evaluate(evalArgs))
            return itemDefGroup;
    }

    return nullptr;
}

QList<ItemMetaData *> ConfigurationsX::findConfigurationItemMetaData(const QString &configName, const QString &platform)
{
    QList <ItemMetaData *> itemMetaDatas;

    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), configName);
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), platform);

    // for files in the project
    for (int i = 0; i < m_project->itemGroupCount(); ++i) {
        ItemGroup *itemGroup = m_project->itemGroup(i);

        if (!itemGroup)
            continue;

        for (int j = 0; j < itemGroup->itemCount(); ++j) {
            Item *item = itemGroup->item(j);

            if (!item)
                continue;

            for (int k = 0; k < item->itemMetaDataCount(); ++k) {
                ItemMetaData *itemMetaData = item->itemMetaData(k);
                ConditionManipulation condMan(itemMetaData->condition());

                if (condMan.evaluate(evalArgs))
                    itemMetaDatas << itemMetaData;
            }
        }
    }

    return itemMetaDatas;
}

QString ConfigurationsX::findConfigName(Item *item)
{
    if (!item)
        return QString();

    for (int i = 0; i < item->itemMetaDataCount(); ++i) {
        ItemMetaData *itemMetaData = item->itemMetaData(i);

        if (itemMetaData && itemMetaData->name() == QLatin1String(CONFIGURATION))
            return itemMetaData->value();
    }

    return QString();
}

QString ConfigurationsX::findConfigPlatform(Item *item)
{
    if (!item)
        return QString();

    for (int i = 0; i < item->itemMetaDataCount(); ++i) {
        ItemMetaData *itemMetaData = item->itemMetaData(i);

        if (itemMetaData && itemMetaData->name() == QLatin1String(PLATFORM))
            return itemMetaData->value();
    }

    return QString();
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
