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
#include "basetoolattributex.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include "../configurationx.h"
#include "../private/itemdefinitiongroup.h"
#include "../private/itemdefinition.h"
#include "../private/itemmetadata.h"
#include "../private/project.h"
#include "../private/propertygroup.h"
#include "../private/property.h"

#include "../utilsx.h"
#include "../vcprojx_constants.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

BaseToolAttributeX::BaseToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : m_attributeDescription(attributeDescription)
    , m_isUsed(false)
    , m_project(project)
    , m_configuration(configuration)
{
}

BaseToolAttributeX::BaseToolAttributeX(const BaseToolAttributeX &other)
{
    m_attributeDescription = other.m_attributeDescription;
    m_isUsed = other.m_isUsed;
    m_project = other.m_project;
    m_configuration = other.m_configuration;
}

BaseToolAttributeX::BaseToolAttributeX(BaseToolAttributeX &&other)
    : BaseToolAttributeX()
{
    swap(*this, other);
}

BaseToolAttributeX &BaseToolAttributeX::operator=(BaseToolAttributeX other)
{
    swap(*this, other);
    return *this;
}

BaseToolAttributeX::~BaseToolAttributeX()
{
}

const AttributeDescriptionDataItem *BaseToolAttributeX::descriptionDataItem() const
{
    return m_attributeDescription;
}

IToolAttributeSettingsWidget *BaseToolAttributeX::createSettingsItem()
{
    return nullptr;
}

QString BaseToolAttributeX::value() const
{
    // if Tag is PropertyGroup
    if (m_attributeDescription->tag() == QLatin1String("PropertyGroup")) {
        EvaluateArguments evalArgs;
        QStringList args = m_configuration->fullName().split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));

        evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(args[0]));
        evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(args[1]));
        PropertyGroup *propertyGroup = m_project->findPropertyGroupWithConditionAndLabel(evalArgs, QLatin1String(CONFIGURATION));

        if (!propertyGroup)
            return m_attributeDescription->defaultValue();

        Property *property = propertyGroup->findProperty(m_attributeDescription->key());

        if (!property)
            return m_attributeDescription->defaultValue();

        return property->value();
    } else { // if Tag is specific
        EvaluateArguments evalArgs;
        QStringList args = m_configuration->fullName().split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));

        evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(args[0]));
        evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(args[1]));

        ItemDefinitionGroup *itemDefinitionGroup = m_project->findItemDefinitionGroupWithCondition(evalArgs);
        ItemDefinition *itemDefinition = itemDefinitionGroup->findItemDefinition(m_attributeDescription->tag());
        // if Tag node does not exist insert it
        if (!itemDefinition)
            return m_attributeDescription->defaultValue();

        ItemMetaData *itemMetaData = itemDefinition->findItemMetaData(m_attributeDescription->key());

        if (!itemMetaData)
            return m_attributeDescription->defaultValue();

        return itemMetaData->value();
    }
}

void BaseToolAttributeX::setValue(const QString &value)
{
    if (!m_isUsed && value == m_attributeDescription->defaultValue())
        return;

    // if Tag is PropertyGroup
    if (m_attributeDescription->tag() == QLatin1String("PropertyGroup")) {
        EvaluateArguments evalArgs;
        QStringList args = m_configuration->fullName().split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));

        evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(args[0]));
        evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(args[1]));
        PropertyGroup *propertyGroup = m_project->findPropertyGroupWithConditionAndLabel(evalArgs, QLatin1String(CONFIGURATION));

        if (!propertyGroup) {
            propertyGroup = new PropertyGroup;
            QString condition = QLatin1String(CONFIGURATION_VARIABLE)
                    + QLatin1String(CONFIGURATION_PLATFORM_DELIMITER)
                    + QLatin1String(PLATFORM_VARIABLE)
                    + QLatin1String("==")
                    + m_configuration->fullName();
            propertyGroup->setCondition(condition);
            propertyGroup->setLabel(QLatin1String(CONFIGURATION));
            m_project->addPropertyGroup(propertyGroup);
        }

        Property *property = propertyGroup->findProperty(m_attributeDescription->key());

        if (!property) {
            property = new Property;
            property->setName(m_attributeDescription->key());
            propertyGroup->addProperty(property);
        }

        property->setValue(value);
        m_isUsed = true;
    } else { // else if Tag is specific
        EvaluateArguments evalArgs;
        QStringList args = m_configuration->fullName().split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));

        evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(args[0]));
        evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(args[1]));

        ItemDefinitionGroup *itemDefinitionGroup = m_project->findItemDefinitionGroupWithCondition(evalArgs);
        ItemDefinition *itemDefinition = itemDefinitionGroup->findItemDefinition(m_attributeDescription->tag());
        // if Tag node does not exist insert it
        if (!itemDefinition) {
            itemDefinition = new ItemDefinition;
            itemDefinition->setName(m_attributeDescription->tag());
        }

        ItemMetaData *itemMetaData = itemDefinition->findItemMetaData(m_attributeDescription->key());

        if (!itemMetaData) {
            ItemMetaData *itemMetaData = new ItemMetaData;
            itemMetaData->setName(m_attributeDescription->key());
            itemDefinition->addMetaData(itemMetaData);
        }

        itemMetaData->setValue(value);
    }
}

bool BaseToolAttributeX::isUsed() const
{
    return m_isUsed;
}

IToolAttribute *BaseToolAttributeX::clone() const
{
    return new BaseToolAttributeX(*this);
}

void BaseToolAttributeX::deleteFromProjectTree()
{
    if (!m_project)
        return;

    // if Tag is PropertyGroup
    if (m_attributeDescription->tag() == QLatin1String("PropertyGroup")) {
        EvaluateArguments evalArgs;
        QStringList args = m_configuration->fullName().split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));

        evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(args[0]));
        evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(args[1]));
        PropertyGroup *propertyGroup = m_project->findPropertyGroupWithConditionAndLabel(evalArgs, QLatin1String(CONFIGURATION));

        if (!propertyGroup)
            return;

        Property *property = propertyGroup->findProperty(m_attributeDescription->key());

        if (!property)
            return;

        propertyGroup->removeProperty(property);
        delete property;
    } else { // if Tag is specific
        EvaluateArguments evalArgs;
        QStringList args = m_configuration->fullName().split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));

        evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(args[0]));
        evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(args[1]));

        ItemDefinitionGroup *itemDefinitionGroup = m_project->findItemDefinitionGroupWithCondition(evalArgs);
        ItemDefinition *itemDefinition = itemDefinitionGroup->findItemDefinition(m_attributeDescription->tag());
        // if Tag node does not exist insert it
        if (!itemDefinition)
            return;

        ItemMetaData *itemMetaData = itemDefinition->findItemMetaData(m_attributeDescription->key());

        if (!itemMetaData)
            return;

        itemDefinition->removeMetaData(itemMetaData);
        delete itemMetaData;
    }
}

BaseToolAttributeX::BaseToolAttributeX()
    : m_attributeDescription(nullptr),
      m_configuration(nullptr),
      m_isUsed(false),
      m_project(nullptr)
{
}

void BaseToolAttributeX::swap(BaseToolAttributeX &first, BaseToolAttributeX &second)
{
    std::swap(first.m_attributeDescription, second.m_attributeDescription);
    std::swap(first.m_configuration, second.m_configuration);
    std::swap(first.m_isUsed, second.m_isUsed);
    std::swap(first.m_project, second.m_project);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
