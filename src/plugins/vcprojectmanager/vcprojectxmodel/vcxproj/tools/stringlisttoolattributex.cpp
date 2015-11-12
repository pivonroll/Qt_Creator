#include "stringlisttoolattributex.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include "../configurationx.h"
#include "../private/project.h"
#include "../private/propertygroup.h"
#include "../private/property.h"

#include "../utilsx.h"
#include "../vcprojx_constants.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

StringListToolAttributeX::StringListToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : m_attributeDescription(attributeDescription)
    , m_isUsed(false)
    , m_project(project)
    , m_configuration(configuration)
{

}

StringListToolAttributeX::StringListToolAttributeX(const StringListToolAttributeX &other)
{
    m_attributeDescription = other.m_attributeDescription;
    m_isUsed = other.m_isUsed;
    m_project = other.m_project;
    m_configuration = other.m_configuration;
}

const AttributeDescriptionDataItem *StringListToolAttributeX::descriptionDataItem() const
{
    return m_attributeDescription;
}

IToolAttributeSettingsWidget *StringListToolAttributeX::createSettingsItem()
{
    return nullptr;
}

QString StringListToolAttributeX::value() const
{
    PropertyGroup *propertyGroup = Utils::findPropertyGroup(m_project, m_configuration->fullName(), QLatin1String(CONFIGURATION));

    if (!propertyGroup)
        return m_attributeDescription->defaultValue();

    Property *property = Utils::findProperty(propertyGroup, m_attributeDescription->key());

    if (!property)
        return m_attributeDescription->defaultValue();

    return property->value();
}

void StringListToolAttributeX::setValue(const QString &value)
{
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
}

bool StringListToolAttributeX::isUsed() const
{
    return m_isUsed;
}

IToolAttribute *StringListToolAttributeX::clone() const
{
    return new StringListToolAttributeX(*this);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

