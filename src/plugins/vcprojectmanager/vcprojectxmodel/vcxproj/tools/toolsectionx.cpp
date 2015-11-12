#include "toolsectionx.h"

#include <visualstudiotoolattributes/toolsectiondescription.h>
#include <visualstudiotoolattributes/generaltoolattributecontainer.h>
#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include "booltoolattributex.h"
#include "integertoolattributex.h"
#include "stringtoolattributex.h"
#include "stringlisttoolattributex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ToolSectionX::ToolSectionX(const ToolSectionDescription *toolSectionDescription, Project *project, ConfigurationX *configuration)
    : m_toolSectionDescription(toolSectionDescription)
    , m_configuration(configuration)
    , m_project(project)
    , m_attributeContainer(new GeneralToolAttributeContainer)
{
    for (int i = 0; i < m_toolSectionDescription->attributeDescriptionCount(); ++i) {

        if (!m_toolSectionDescription->attributeDescription(i))
            continue;

            IToolAttribute *toolAttribute = nullptr;

            if (m_toolSectionDescription->attributeDescription(i)->type() == QLatin1String("Boolean"))
                toolAttribute = new BoolToolAttributeX(m_toolSectionDescription->attributeDescription(i), m_project, m_configuration);
            else if (m_toolSectionDescription->attributeDescription(i)->type() == QLatin1String("Integer"))
                toolAttribute = new IntegerToolAttributeX(m_toolSectionDescription->attributeDescription(i), m_project, m_configuration);
            else if (m_toolSectionDescription->attributeDescription(i)->type() == QLatin1String("String"))
                toolAttribute = new StringToolAttributeX(m_toolSectionDescription->attributeDescription(i), m_project, m_configuration);
            else if (m_toolSectionDescription->attributeDescription(i)->type() == QLatin1String("StringList"))
                toolAttribute = new StringListToolAttributeX(m_toolSectionDescription->attributeDescription(i), m_project, m_configuration);

            if (toolAttribute)
                m_attributeContainer->addToolAttribute(toolAttribute);
    }
}

ToolSectionX::ToolSectionX(const ToolSectionX &other)
{
    m_toolSectionDescription = other.m_toolSectionDescription;
    m_configuration = other.m_configuration;
    m_project = other.m_project;
    *m_attributeContainer = *other.m_attributeContainer;
}

IToolAttributeContainer *ToolSectionX::attributeContainer() const
{
    return m_attributeContainer;
}

const ToolSectionDescription *ToolSectionX::sectionDescription() const
{
    return m_toolSectionDescription;
}

VcNodeWidget *ToolSectionX::createSettingsWidget()
{
    return nullptr;
}

IToolSection *ToolSectionX::clone() const
{
    return new ToolSectionX(*this);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

