#include "toolsectioncontainerx.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itoolsection.h>

#include <QDomElement>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ToolSectionContainerX::ToolSectionContainerX(VisualStudioProjectX::Project *project, ConfigurationX *configurationX)
    : m_project(project)
    , m_configurationX(configurationX)
{
}

ToolSectionContainerX::ToolSectionContainerX(const ToolSectionContainerX &toolSec)
{
    qDeleteAll(m_toolSections);
    m_toolSections.clear();

    foreach (IToolSection *sec, toolSec.m_toolSections) {
        if (sec)
            m_toolSections.append(sec->clone());
    }
}

ToolSectionContainerX &ToolSectionContainerX::operator =(const ToolSectionContainerX &toolSec)
{
    if (this != &toolSec) {
        qDeleteAll(m_toolSections);
        m_toolSections.clear();

        for (int i = 0; i < toolSec.sectionCount(); ++i) {
            IToolSection *sec = toolSec.section(i);
            if (sec)
                m_toolSections.append(sec->clone());
        }
    }

    return *this;
}

ToolSectionContainerX::~ToolSectionContainerX()
{
    qDeleteAll(m_toolSections);
}

IToolSection *ToolSectionContainerX::section(int index) const
{
    if (0 <= index && index < m_toolSections.size())
        return m_toolSections[index];
    return 0;
}

IToolSection *ToolSectionContainerX::section(const QString &sectionName) const
{
    foreach (IToolSection *sec, m_toolSections) {
        if (sec && sec->sectionDescription()->displayName() == sectionName) {
            return sec;
        }
    }
    return 0;
}

int ToolSectionContainerX::sectionCount() const
{
    return m_toolSections.size();
}

void ToolSectionContainerX::appendSection(IToolSection *section)
{
    if (!section)
        return;

    foreach (IToolSection *sec, m_toolSections) {
        if (sec && sec->sectionDescription()->displayName() == section->sectionDescription()->displayName())
            return;
    }
    m_toolSections.append(section);
}

void ToolSectionContainerX::removeSection(const QString &sectionName)
{
    foreach (IToolSection *sec, m_toolSections) {
        if (sec && sec->sectionDescription()->displayName() == sectionName) {
            m_toolSections.removeOne(sec);
            delete sec;
            return;
        }
    }
}

void ToolSectionContainerX::appendToXMLNode(QDomElement &elementNode, QDomDocument &domXMLDocument)
{
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
