#include "toolsx.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ToolsX::ToolsX(ConfigurationX *parentConfiguration)
    : m_parentConfiguration(parentConfiguration)
{
}

ToolsX::~ToolsX()
{
}

IConfigurationBuildTools *ToolsX::configurationBuildTools() const
{
    return nullptr;
}

IDeploymentTools *ToolsX::deploymentTools() const
{
    return nullptr;
}

IDebuggerTools *ToolsX::debuggerTools() const
{
    return nullptr;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

