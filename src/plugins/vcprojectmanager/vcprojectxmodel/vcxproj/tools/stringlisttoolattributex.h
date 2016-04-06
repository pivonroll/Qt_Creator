#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGLISTTOOLATTRIBUTEX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGLISTTOOLATTRIBUTEX_H

#include "basetoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;
class ConfigurationX;

class StringListToolAttributeX : public BaseToolAttributeX
{
public:
    StringListToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration);
    StringListToolAttributeX(const StringListToolAttributeX &other);

    // IToolAttribute interface
    IToolAttributeSettingsWidget *createSettingsItem();
    IToolAttribute *clone() const;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGLISTTOOLATTRIBUTEX_H
