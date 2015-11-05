#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CLCOMPILETOOL_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CLCOMPILETOOL_H

#include <visualstudiointerfaces/iconfigurationbuildtool.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;

class ConfigurationToolX : public IConfigurationBuildTool
{
public:
    ConfigurationToolX(IToolDescription *toolDesc, Project *project);
    ConfigurationToolX(const ConfigurationToolX &configToolRef);
    ConfigurationToolX &operator=(const ConfigurationToolX &configToolRef);

    // IVcProjectXMLNode interface
    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    // IConfigurationBuildTool interface
    ISectionContainer *sectionContainer() const;
    const IToolDescription *toolDescription() const;
    IConfigurationBuildTool *clone() const;
    bool allAttributesAreDefault() const;

private:
    const IToolDescription *m_toolDescription;
    Project *m_project;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CLCOMPILETOOL_H
