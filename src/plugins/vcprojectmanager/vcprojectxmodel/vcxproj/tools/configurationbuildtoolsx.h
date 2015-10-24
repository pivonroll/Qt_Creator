#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATIONBUILDTOOLSX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATIONBUILDTOOLSX_H

#include <visualstudiointerfaces/iconfigurationbuildtools.h>

#include <QList>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class ConfigurationBuildToolsX : public IConfigurationBuildTools
{
public:
    ConfigurationBuildToolsX();
    ~ConfigurationBuildToolsX();

    // IConfigurationBuildTools interface
    void addTool(IConfigurationBuildTool *tool);
    void removeTool(IConfigurationBuildTool *tool);
    IConfigurationBuildTool *tool(const QString &toolKey) const;
    IConfigurationBuildTool *tool(int index) const;
    int toolCount() const;
    void appendToXMLNode(QDomElement &domElement, QDomDocument &domDocument) const;

private:
    QList<IConfigurationBuildTool *> m_configTools;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONFIGURATIONBUILDTOOLSX_H
