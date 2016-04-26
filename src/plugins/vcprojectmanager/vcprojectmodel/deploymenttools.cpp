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
#include "deploymenttools.h"
#include <visualstudiointerfaces/ideploymenttool.h>

#include <utils/qtcassert.h>

#include <QDomElement>

namespace VcProjectManager {
namespace Internal {

DeploymentTools::DeploymentTools()
{
}

DeploymentTools::DeploymentTools(const DeploymentTools &other)
{
    foreach (IDeploymentTool *tool, other.m_deploymentTools)
        m_deploymentTools << tool->clone();
}

DeploymentTools::DeploymentTools(DeploymentTools &&other)
    : DeploymentTools()
{
    swap(*this, other);
}

DeploymentTools &DeploymentTools::operator=(DeploymentTools other)
{
    swap(*this, other);
    return *this;
}

void DeploymentTools::addTool(IDeploymentTool *tool)
{
    if (!tool || m_deploymentTools.contains(tool))
        return;

    m_deploymentTools.append(tool);
}

void DeploymentTools::removeTool(IDeploymentTool *tool)
{
    if (!tool || !m_deploymentTools.contains(tool))
        return;

    m_deploymentTools.removeOne(tool);
    delete tool;
}

IDeploymentTool *DeploymentTools::tool(int index) const
{
    QTC_ASSERT(0 <= index && index < m_deploymentTools.size(), return 0);
    return m_deploymentTools[index];
}

int DeploymentTools::toolCount() const
{
    return m_deploymentTools.size();
}

void DeploymentTools::appendToXMLNode(QDomElement &domElement, QDomDocument &domDocument) const
{
    foreach (const IDeploymentTool *tool, m_deploymentTools)
        domElement.appendChild(tool->toXMLDomNode(domDocument));
}

void DeploymentTools::swap(DeploymentTools &first, DeploymentTools &second)
{
    std::swap(first.m_deploymentTools, second.m_deploymentTools);
}

} // namespace Internal
} // namespace VcProjectManager
