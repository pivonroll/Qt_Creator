#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_INTEGERTOOLATTRIBUTEX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_INTEGERTOOLATTRIBUTEX_H

#include <visualstudiointerfaces/itoolattribute.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;
class ConfigurationX;

class IntegerToolAttributeX : public IToolAttribute
{
public:
    IntegerToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration);
    IntegerToolAttributeX(const IntegerToolAttributeX &other);

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

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_INTEGERTOOLATTRIBUTEX_H
