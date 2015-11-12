#include "stringtoolattributex.h"

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

StringToolAttributeX::StringToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : m_attributeDescription(attributeDescription)
    , m_isUsed(false)
    , m_project(project)
    , m_configuration(configuration)
{

}

StringToolAttributeX::StringToolAttributeX(const StringToolAttributeX &other)
{
    m_attributeDescription = other.m_attributeDescription;
    m_value = other.m_value;
    m_isUsed = other.m_isUsed;
}

const AttributeDescriptionDataItem *StringToolAttributeX::descriptionDataItem() const
{
    return m_attributeDescription;
}

IToolAttributeSettingsWidget *StringToolAttributeX::createSettingsItem()
{
    return nullptr;
}

QString StringToolAttributeX::value() const
{
    PropertyGroup *propertyGroup = Utils::findPropertyGroup(m_project, m_configuration->fullName(), QLatin1String(CONFIGURATION));

    if (!propertyGroup)
        //create property group;
        ;

    Property *property = Utils::findProperty(propertyGroup, m_attributeDescription->key());

    if (!property)
        // create property
        ;

    m_value = property->value();

    return m_value;
}

void StringToolAttributeX::setValue(const QString &value)
{
    m_value = value;
    PropertyGroup *propertyGroup = Utils::findPropertyGroup(m_project, m_configuration->fullName(), QLatin1String(CONFIGURATION));

    if (!propertyGroup)
        //create property group;
        ;

    Property *property = Utils::findProperty(propertyGroup, m_attributeDescription->key());

    if (!property)
        // create property
        ;

    property->setValue(m_value);
}

bool StringToolAttributeX::isUsed() const
{
    return m_isUsed;
}

IToolAttribute *StringToolAttributeX::clone() const
{
    return new StringToolAttributeX(*this);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

