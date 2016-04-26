/**************************************************************************
**
** Copyright (c) 2016 Bojan Petrovic
** Copyright (c) 2016 Radovan Zivkovic
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/
#include "toolsectionx.h"

#include <visualstudiotoolattributes/toolsectiondescription.h>
#include <visualstudiotoolattributes/generaltoolattributecontainer.h>
#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include "booltoolattributex.h"
#include "integertoolattributex.h"
#include "stringtoolattributex.h"
#include "stringlisttoolattributex.h"
#include "../private/project.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ToolSectionX::ToolSectionX(const ToolSectionDescription *toolSectionDescription, Project *project, ConfigurationX *configuration)
    : m_toolSectionDescription(toolSectionDescription)
    , m_configuration(configuration)
    , m_project(project)
    , m_attributeContainer(new GeneralToolAttributeContainer)
{
    if (!m_project)
        return;

    for (int i = 0; i < m_toolSectionDescription->attributeDescriptionCount(); ++i) {

        if (!m_toolSectionDescription->attributeDescription(i)
                || m_toolSectionDescription->attributeDescription(i)->version() != m_project->version())
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

ToolSectionX::ToolSectionX(ToolSectionX &&other)
    : ToolSectionX()
{
    swap(*this, other);
}

ToolSectionX &ToolSectionX::operator=(ToolSectionX other)
{
    swap(*this, other);
    return *this;
}

ToolSectionX::~ToolSectionX()
{
    delete m_attributeContainer;
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

ToolSectionX::ToolSectionX()
    : m_attributeContainer(nullptr),
      m_configuration(nullptr),
      m_project(nullptr),
      m_toolSectionDescription(nullptr)
{
}

void ToolSectionX::swap(ToolSectionX &first, ToolSectionX &second)
{
    std::swap(first.m_attributeContainer, second.m_attributeContainer);
    std::swap(first.m_configuration, second.m_configuration);
    std::swap(first.m_project, second.m_project);
    std::swap(first.m_toolSectionDescription, second.m_toolSectionDescription);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

