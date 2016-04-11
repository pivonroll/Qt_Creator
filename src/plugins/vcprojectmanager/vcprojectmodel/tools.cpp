/**************************************************************************
**
** Copyright (c) 2014 Bojan Petrovic
** Copyright (c) 2014 Radovan Zivkovic
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
#include "configurationbuildtools.h"
#include "debuggertools.h"
#include "deploymenttools.h"
#include "tools.h"

namespace VcProjectManager {
namespace Internal {

Tools::Tools()
    : m_configurationBuildTools(new ConfigurationBuildTools),
      m_deploymentTools(new DeploymentTools),
      m_debuggerTools(new DebuggerTools)
{
}

Tools::Tools(const Tools &other)
{
    m_configurationBuildTools = new ConfigurationBuildTools;
    m_deploymentTools = new DeploymentTools;
    m_debuggerTools = new DebuggerTools;
    *m_configurationBuildTools = *other.m_configurationBuildTools;
    *m_deploymentTools = *other.m_deploymentTools;
    *m_debuggerTools = *other.m_debuggerTools;
}

Tools::Tools(Tools &&other)
    : Tools()
{
    swap(*this, other);
}

Tools &Tools::operator=(Tools other)
{
    swap(*this, other);
    return *this;
}

Tools::~Tools()
{
    delete m_configurationBuildTools;
    delete m_debuggerTools;
    delete m_deploymentTools;
}

IConfigurationBuildTools *Tools::configurationBuildTools() const
{
    return m_configurationBuildTools;
}

IDeploymentTools *Tools::deploymentTools() const
{
    return m_deploymentTools;
}

IDebuggerTools *Tools::debuggerTools() const
{
    return m_debuggerTools;
}

void Tools::swap(Tools &first, Tools &second)
{
    std::swap(first.m_configurationBuildTools, second.m_configurationBuildTools);
    std::swap(first.m_debuggerTools, second.m_debuggerTools);
    std::swap(first.m_deploymentTools, second.m_deploymentTools);
}

} // namespace Internal
} // namespace VcProjectManager
