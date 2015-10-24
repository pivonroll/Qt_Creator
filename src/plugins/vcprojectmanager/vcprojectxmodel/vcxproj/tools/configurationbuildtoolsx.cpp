#include "configurationbuildtoolsx.h"

#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConfigurationBuildToolsX::ConfigurationBuildToolsX()
{
}

ConfigurationBuildToolsX::~ConfigurationBuildToolsX()
{
}

void ConfigurationBuildToolsX::addTool(IConfigurationBuildTool *tool)
{
    QTC_ASSERT(tool && !m_configTools.contains(tool), return);
    m_configTools.append(tool);
}

void ConfigurationBuildToolsX::removeTool(IConfigurationBuildTool *tool)
{

}

IConfigurationBuildTool *ConfigurationBuildToolsX::tool(const QString &toolKey) const
{
    return nullptr;
}

IConfigurationBuildTool *ConfigurationBuildToolsX::tool(int index) const
{
    return nullptr;
}

int ConfigurationBuildToolsX::toolCount() const
{
    return 0;
}

void ConfigurationBuildToolsX::appendToXMLNode(QDomElement &domElement, QDomDocument &domDocument) const
{
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

