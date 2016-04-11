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
    BaseToolAttributeX(const BaseToolAttributeX &other);
    BaseToolAttributeX(BaseToolAttributeX &&other);
    BaseToolAttributeX& operator=(BaseToolAttributeX other);

    // IToolAttribute interface
    const AttributeDescriptionDataItem *descriptionDataItem() const;
    IToolAttributeSettingsWidget *createSettingsItem();
    QString value() const;
    void setValue(const QString &value);
    bool isUsed() const;
    IToolAttribute *clone() const;

protected:
    BaseToolAttributeX();

    static void swap(BaseToolAttributeX &first, BaseToolAttributeX &second);
    const AttributeDescriptionDataItem *m_attributeDescription;
    bool m_isUsed;
    Project *m_project;
    ConfigurationX *m_configuration;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // BASETOOLATTRIBUTE_H
