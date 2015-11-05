#include "configurationbuildtoolsx.h"

#include <utils/qtcassert.h>

#include <QDomElement>
#include <QDomDocument>

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
    Q_UNUSED(tool)
}

IConfigurationBuildTool *ConfigurationBuildToolsX::tool(const QString &toolKey) const
{
    Q_UNUSED(toolKey)
    return nullptr;
}

IConfigurationBuildTool *ConfigurationBuildToolsX::tool(int index) const
{
    Q_UNUSED(index)
    return nullptr;
}

int ConfigurationBuildToolsX::toolCount() const
{
    return 0;
}

void ConfigurationBuildToolsX::appendToXMLNode(QDomElement &domElement, QDomDocument &domDocument) const
{
    Q_UNUSED(domElement)
    Q_UNUSED(domDocument)
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

