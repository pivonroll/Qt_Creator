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
#include "configurationtoolx.h"

#include "toolsectionx.h"

#include <visualstudiotoolattributes/toolsectiondescription.h>
#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/toolsectioncontainer.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConfigurationToolX::ConfigurationToolX(ToolDescription *toolDesc, Project *project, ConfigurationX *configuration)
    : m_toolDescription(toolDesc)
    , m_project(project)
    , m_configuration(configuration)
    , m_toolSectionContainer(new ToolSectionContainer)
{
    for (int i = 0; i < m_toolDescription->sectionDescriptionCount(); ++i) {
        if (m_toolDescription->sectionDescription(i))
            m_toolSectionContainer->appendSection(new ToolSectionX(m_toolDescription->sectionDescription(i), m_project, m_configuration));
    }
}

ConfigurationToolX::ConfigurationToolX(const ConfigurationToolX &other)
{
    m_toolDescription = other.m_toolDescription;
    m_project = other.m_project;
    m_toolSectionContainer = new ToolSectionContainer;
    *m_toolSectionContainer = *other.m_toolSectionContainer;
}

ConfigurationToolX &ConfigurationToolX::operator=(ConfigurationToolX other)
{
    swap(*this, other);
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

const ToolDescription *ConfigurationToolX::toolDescription() const
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

ConfigurationToolX::ConfigurationToolX()
    : m_configuration(nullptr),
      m_project(nullptr),
      m_toolDescription(nullptr),
      m_toolSectionContainer(nullptr)
{
}

void ConfigurationToolX::swap(ConfigurationToolX &first, ConfigurationToolX &second)
{
    std::swap(first.m_configuration, second.m_configuration);
    std::swap(first.m_project, second.m_project);
    std::swap(first.m_toolDescription, second.m_toolDescription);
    std::swap(first.m_toolSectionContainer, second.m_toolSectionContainer);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

