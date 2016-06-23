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
#include "debuggertool.h"
#include "generalattributecontainer.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {

DebuggerTool::DebuggerTool()
{
    m_attributeContainer = new GeneralAttributeContainer;
}

DebuggerTool::DebuggerTool(const DebuggerTool &other)
{
    m_attributeContainer = new GeneralAttributeContainer;
    *m_attributeContainer = *other.m_attributeContainer;
}

DebuggerTool::DebuggerTool(DebuggerTool &&other)
    : DebuggerTool()
{
    swap(*this, other);
}

DebuggerTool &DebuggerTool::operator=(DebuggerTool other)
{
    swap(*this, other);
    return *this;
}

DebuggerTool::~DebuggerTool()
{
}

void DebuggerTool::processNode(const QDomNode &node)
{
    if (node.isNull())
        return;

    if (node.nodeType() == QDomNode::ElementNode)
        processNodeAttributes(node.toElement());
}

VcNodeWidget *DebuggerTool::createSettingsWidget()
{
    return nullptr;
}

QDomNode DebuggerTool::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement toolNode = domXMLDocument.createElement(QLatin1String("DebuggerTool"));
    m_attributeContainer->appendToXMLNode(toolNode);
    return toolNode;
}

IAttributeContainer *DebuggerTool::attributeContainer() const
{
    return m_attributeContainer;
}

IDebuggerTool *DebuggerTool::clone() const
{
    return new DebuggerTool(*this);
}

void DebuggerTool::swap(DebuggerTool &first, DebuggerTool &second)
{
    std::swap(first.m_attributeContainer, second.m_attributeContainer);
}

void DebuggerTool::processNodeAttributes(const QDomElement &element)
{
    QDomNamedNodeMap namedNodeMap = element.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();
            m_attributeContainer->setAttribute(domElement.name(), domElement.value());
        }
    }
}

} // namespace Internal
} // namespace VcProjectManager
