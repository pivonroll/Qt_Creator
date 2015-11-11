#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGTOOLATTRIBUTEX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGTOOLATTRIBUTEX_H

#include <visualstudiointerfaces/itoolattribute.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class StringToolAttributeX : public IToolAttribute
{
public:
    StringToolAttributeX(const AttributeDescriptionDataItem *attributeDescription);
    StringToolAttributeX(const StringToolAttributeX &other);

    // IToolAttribute interface
    const AttributeDescriptionDataItem *descriptionDataItem() const;
    IToolAttributeSettingsWidget *createSettingsItem();
    QString value() const;
    void setValue(const QString &value);
    bool isUsed() const;
    IToolAttribute *clone() const;

private:
    const AttributeDescriptionDataItem *m_attributeDescription;
    QString m_value;
    bool m_isUsed;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_STRINGTOOLATTRIBUTEX_H
