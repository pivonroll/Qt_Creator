#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CLCOMPILETOOL_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CLCOMPILETOOL_H

#include <visualstudiointerfaces/iconfigurationbuildtool.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class ClCompileTool : public IConfigurationBuildTool
{
public:
    ClCompileTool();
    // IVcProjectXMLNode interface
public:
    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    // IConfigurationBuildTool interface
public:
    ISectionContainer *sectionContainer() const;
    const IToolDescription *toolDescription() const;
    IConfigurationBuildTool *clone() const;
    bool allAttributesAreDefault() const;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CLCOMPILETOOL_H
