#include "integertoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

IntegerToolAttributeX::IntegerToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : BaseToolAttributeX(attributeDescription, project, configuration)
{
}

IntegerToolAttributeX::IntegerToolAttributeX(const IntegerToolAttributeX &other)
{
    BaseToolAttributeX::BaseToolAttributeX(other);
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

