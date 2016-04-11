#include "stringlisttoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

StringListToolAttributeX::StringListToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : BaseToolAttributeX(attributeDescription, project, configuration)
{

}

StringListToolAttributeX::StringListToolAttributeX(const StringListToolAttributeX &other)
    : BaseToolAttributeX(other)
{
}

StringListToolAttributeX::StringListToolAttributeX(StringListToolAttributeX &&other)
    : BaseToolAttributeX(other)
{
}

StringListToolAttributeX &StringListToolAttributeX::operator=(StringListToolAttributeX other)
{
    BaseToolAttributeX::operator =(other);
    return *this;
}

IToolAttributeSettingsWidget *StringListToolAttributeX::createSettingsItem()
{
    return nullptr;
}

IToolAttribute *StringListToolAttributeX::clone() const
{
    return new StringListToolAttributeX(*this);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

