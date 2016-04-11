#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_BOOLTOOLATTRIBUTEX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_BOOLTOOLATTRIBUTEX_H

#include "basetoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;
class ConfigurationX;

class BoolToolAttributeX : public BaseToolAttributeX
{
public:
    BoolToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration);
    BoolToolAttributeX(const BoolToolAttributeX &other);
    BoolToolAttributeX& operator=(BoolToolAttributeX other);

    // IToolAttribute interface
    IToolAttributeSettingsWidget *createSettingsItem();
    IToolAttribute *clone() const;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_BOOLTOOLATTRIBUTEX_H
