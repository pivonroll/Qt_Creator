#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_INTEGERTOOLATTRIBUTEX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_INTEGERTOOLATTRIBUTEX_H

#include "basetoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;
class ConfigurationX;

class IntegerToolAttributeX : public BaseToolAttributeX
{
public:
    IntegerToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration);
    IntegerToolAttributeX(const IntegerToolAttributeX &other);
    IntegerToolAttributeX(IntegerToolAttributeX &&other);
    IntegerToolAttributeX& operator=(IntegerToolAttributeX other);

    // IToolAttribute interface
    IToolAttributeSettingsWidget *createSettingsItem();
    IToolAttribute *clone() const;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_INTEGERTOOLATTRIBUTEX_H
