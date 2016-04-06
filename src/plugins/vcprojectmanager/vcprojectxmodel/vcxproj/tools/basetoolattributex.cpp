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
        PropertyGroup *propertyGroup = Utils::findPropertyGroup(m_project, m_configuration->fullName(), QLatin1String(CONFIGURATION));

        if (!propertyGroup)
            return m_attributeDescription->defaultValue();

        Property *property = Utils::findProperty(propertyGroup, m_attributeDescription->key());

        if (!property)
            return m_attributeDescription->defaultValue();

        return property->value();
    } else { // if Tag is specific
        ItemDefinitionGroup *itemDefinitionGroup = Utils::findItemDefinitionGroup(m_project, m_configuration->fullName());
        ItemDefinition *itemDefinition = Utils::findItemDefinition(itemDefinitionGroup, m_attributeDescription->tag());
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
        PropertyGroup *propertyGroup = Utils::findPropertyGroup(m_project, m_configuration->fullName(), QLatin1String(CONFIGURATION));

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

        Property *property = Utils::findProperty(propertyGroup, m_attributeDescription->key());

        if (!property) {
            property = new Property;
            property->setName(m_attributeDescription->key());
            propertyGroup->addProperty(property);
        }

        property->setValue(value);
        m_isUsed = true;
    } else { // else if Tag is specific
        ItemDefinitionGroup *itemDefinitionGroup = Utils::findItemDefinitionGroup(m_project, m_configuration->fullName());
        ItemDefinition *itemDefinition = Utils::findItemDefinition(itemDefinitionGroup, m_attributeDescription->tag());
        // if Tag node does not exist insert it
        if (!itemDefinition) {
            itemDefinition = new ItemDefinition;
            itemDefinition->setName(toolAttr->descriptionDataItem()->tag());
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

BaseToolAttributeX::BaseToolAttributeX(const BaseToolAttributeX &other)
{
    m_attributeDescription = other.m_attributeDescription;
    m_isUsed = other.m_isUsed;
    m_project = other.m_project;
    m_configuration = other.m_configuration;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
