#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TOOLSX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TOOLSX_H

#include <visualstudiointerfaces/itools.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class ConfigurationX;

class ToolsX : public ITools
{
public:
    ToolsX(ConfigurationX *parentConfiguration);
    ~ToolsX();

    // ITools interface
    IConfigurationBuildTools *configurationBuildTools() const;
    IDeploymentTools *deploymentTools() const;
    IDebuggerTools *debuggerTools() const;

private:
    ConfigurationX *m_parentConfiguration;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TOOLSX_H
