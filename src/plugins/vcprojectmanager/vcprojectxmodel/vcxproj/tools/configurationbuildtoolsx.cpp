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
#include "configurationbuildtoolsx.h"

#include <visualstudiointerfaces/iconfigurationbuildtool.h>

#include <utils/qtcassert.h>

#include <QDomElement>
#include <QDomDocument>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConfigurationBuildToolsX::ConfigurationBuildToolsX()
{
}

ConfigurationBuildToolsX::ConfigurationBuildToolsX(const ConfigurationBuildToolsX &other)
{
    foreach (IConfigurationBuildTool *tool, other.m_configTools)
        m_configTools << tool->clone();
}

ConfigurationBuildToolsX::ConfigurationBuildToolsX(ConfigurationBuildToolsX &&other)
    : ConfigurationBuildToolsX()
{
    swap(*this, other);
}

ConfigurationBuildToolsX &ConfigurationBuildToolsX::operator=(ConfigurationBuildToolsX other)
{
    swap(*this, other);
    return *this;
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

void ConfigurationBuildToolsX::swap(ConfigurationBuildToolsX &first, ConfigurationBuildToolsX &second)
{
    std::swap(first.m_configTools, second.m_configTools);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

