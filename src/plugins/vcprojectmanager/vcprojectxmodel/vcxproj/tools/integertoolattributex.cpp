#include "integertoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

IntegerToolAttributeX::IntegerToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : BaseToolAttributeX(attributeDescription, project, configuration)
{
}

IntegerToolAttributeX::IntegerToolAttributeX(const IntegerToolAttributeX &other)
    : BaseToolAttributeX(other)
{
}

IntegerToolAttributeX::IntegerToolAttributeX(IntegerToolAttributeX &&other)
    : BaseToolAttributeX(other)
{
}

IntegerToolAttributeX &IntegerToolAttributeX::operator=(IntegerToolAttributeX other)
{
    BaseToolAttributeX::operator =(other);
    return *this;
}

IToolAttributeSettingsWidget *IntegerToolAttributeX::createSettingsItem()
{
    return nullptr;
}

IToolAttribute *IntegerToolAttributeX::clone() const
{
    return new IntegerToolAttributeX(*this);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

