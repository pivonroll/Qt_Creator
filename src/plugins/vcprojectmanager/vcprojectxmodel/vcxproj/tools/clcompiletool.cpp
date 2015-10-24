#include "clcompiletool.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ClCompileTool::ClCompileTool()
{

}

void ClCompileTool::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

VcNodeWidget *ClCompileTool::createSettingsWidget()
{
    return nullptr;
}

QDomNode ClCompileTool::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);
    return QDomNode();
}

ISectionContainer *ClCompileTool::sectionContainer() const
{
    return nullptr;
}

const IToolDescription *ClCompileTool::toolDescription() const
{
    return nullptr;
}

IConfigurationBuildTool *ClCompileTool::clone() const
{
    return nullptr;
}

bool ClCompileTool::allAttributesAreDefault() const
{
    return true;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

