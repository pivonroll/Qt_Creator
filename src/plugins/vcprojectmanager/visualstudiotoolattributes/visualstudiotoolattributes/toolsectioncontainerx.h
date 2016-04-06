#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TOOLSECTIONCONTAINERX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TOOLSECTIONCONTAINERX_H

#include <visualstudiointerfaces/isectioncontainer.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Project;
class ConfigurationX;

class ToolSectionContainerX : public ISectionContainer
{
public:
    ToolSectionContainerX(VisualStudioProjectX::Project *project, ConfigurationX *configurationX);
    ToolSectionContainerX(const ToolSectionContainerX &toolSec);
    ToolSectionContainerX &operator =(const ToolSectionContainerX &toolSec);
    ~ToolSectionContainerX();

    // ISectionContainer interface
    IToolSection *section(int index) const;
    IToolSection *section(const QString &sectionName) const;
    int sectionCount() const;
    void appendSection(IToolSection *section);
    void removeSection(const QString &sectionName);
    void appendToXMLNode(QDomElement &elementNode, QDomDocument &domXMLDocument);
private:
    QList<IToolSection *> m_toolSections;
    VisualStudioProjectX::Project *m_project;
    ConfigurationX *m_configurationX;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TOOLSECTIONCONTAINERX_H
