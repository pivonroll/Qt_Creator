#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_BASETOOLATTRIBUTEX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_BASETOOLATTRIBUTEX_H

#include <visualstudiointerfaces/itoolattribute.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;
class ConfigurationX;


class BaseToolAttributeX : public IToolAttribute
{
public:
    BaseToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration);

    // IToolAttribute interface
public:
    const AttributeDescriptionDataItem *descriptionDataItem() const;
    IToolAttributeSettingsWidget *createSettingsItem();
    QString value() const;
    void setValue(const QString &value);
    bool isUsed() const;

protected:
    BaseToolAttributeX(const BaseToolAttributeX &other);

    const AttributeDescriptionDataItem *m_attributeDescription;
    bool m_isUsed;
    Project *m_project;
    ConfigurationX *m_configuration;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // BASETOOLATTRIBUTE_H
