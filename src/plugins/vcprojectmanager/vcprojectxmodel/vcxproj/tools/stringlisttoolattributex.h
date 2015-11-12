#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGLISTTOOLATTRIBUTEX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGLISTTOOLATTRIBUTEX_H

#include <visualstudiointerfaces/itoolattribute.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;
class ConfigurationX;

class StringListToolAttributeX : public IToolAttribute
{
public:
    StringListToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration);
    StringListToolAttributeX(const StringListToolAttributeX &other);

    // IToolAttribute interface
    const AttributeDescriptionDataItem *descriptionDataItem() const;
    IToolAttributeSettingsWidget *createSettingsItem();
    QString value() const;
    void setValue(const QString &value);
    bool isUsed() const;
    IToolAttribute *clone() const;

private:
    const AttributeDescriptionDataItem *m_attributeDescription;
    bool m_isUsed;
    Project *m_project;
    ConfigurationX *m_configuration;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGLISTTOOLATTRIBUTEX_H
