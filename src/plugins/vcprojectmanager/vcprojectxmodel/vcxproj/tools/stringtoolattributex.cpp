#include "stringtoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

StringToolAttributeX::StringToolAttributeX(const AttributeDescriptionDataItem *attributeDescription)
    : m_attributeDescription(other)
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
    return m_value;
}

void StringToolAttributeX::setValue(const QString &value)
{
    m_value = value;
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

