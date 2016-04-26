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
#include "taskbody.h"

#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

TaskBody::TaskBody()
{
}

TaskBody::TaskBody(const TaskBody &other)
{
    m_data = other.m_data;
    m_evaluate = other.m_evaluate;
}

TaskBody::TaskBody(TaskBody &&other)
    : TaskBody()
{
    swap(*this, other);
}

TaskBody &TaskBody::operator=(TaskBody other)
{
    swap(*this, other);
    return *this;
}

TaskBody::~TaskBody()
{
}

QString TaskBody::data() const
{
    return m_data;
}

void TaskBody::setData(const QString &data)
{
    m_data = data;
}

QString TaskBody::evaluate() const
{
    return m_evaluate;
}

void TaskBody::setEvaluate(const QString &evaluate)
{
    m_evaluate = evaluate;
}

void TaskBody::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());

    m_data = node.nodeValue();
}

QDomNode TaskBody::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(TASK_BODY_ITEM));

    if (!m_evaluate.isEmpty())
        element.setAttribute(QLatin1String(EVALUATE_ATTRIBUTE), m_evaluate);

    element.setNodeValue(m_data);

    return element;
}

void TaskBody::swap(TaskBody &first, TaskBody &second)
{
    std::swap(first.m_data, second.m_data);
    std::swap(first.m_evaluate, second.m_evaluate);
}

void TaskBody::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(EVALUATE_ATTRIBUTE))
                m_evaluate = domElement.value();
        }
    }
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
