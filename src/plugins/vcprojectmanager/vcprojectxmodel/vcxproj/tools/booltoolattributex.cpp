#include "booltoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

BoolToolAttributeX::BoolToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : BaseToolAttributeX(attributeDescription, project, configuration)
{
}

BoolToolAttributeX::BoolToolAttributeX(const BoolToolAttributeX &other)
    : BaseToolAttributeX(other)
{
}

BoolToolAttributeX &BoolToolAttributeX::operator=(BoolToolAttributeX other)
{
    BaseToolAttributeX::operator=(other);
    return *this;
}

IToolAttributeSettingsWidget *BoolToolAttributeX::createSettingsItem()
{
    return nullptr;
}

IToolAttribute *BoolToolAttributeX::clone() const
{
    return new BoolToolAttributeX(*this);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

