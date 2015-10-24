#include "configurationtoolx.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConfigurationToolX::ConfigurationToolX(IToolDescription *toolDesc)
    : m_toolDescription(toolDesc)
{
}

ConfigurationToolX::ConfigurationToolX(const ConfigurationToolX &configToolRef)
{
    m_toolDescription = configToolRef.m_toolDescription;
}

ConfigurationToolX &ConfigurationToolX::operator=(const ConfigurationToolX &configToolRef)
{
    if (this != &configToolRef)
        m_toolDescription = configToolRef.m_toolDescription;
    return *this;
}

void ConfigurationToolX::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

VcNodeWidget *ConfigurationToolX::createSettingsWidget()
{
    return nullptr;
}

QDomNode ConfigurationToolX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);
    return QDomNode();
}

ISectionContainer *ConfigurationToolX::sectionContainer() const
{
    return nullptr;
}

const IToolDescription *ConfigurationToolX::toolDescription() const
{
    return m_toolDescription;
}

IConfigurationBuildTool *ConfigurationToolX::clone() const
{
    return new ConfigurationToolX(*this);
}

bool ConfigurationToolX::allAttributesAreDefault() const
{
    return true;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

