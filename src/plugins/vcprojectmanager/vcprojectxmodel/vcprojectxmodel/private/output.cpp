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
#include "output.h"

#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Output::Output()
{
}

Output::Output(const Output &other)
{
    m_taskParameter = other.m_taskParameter;
    m_condition = other.m_condition;
    m_itemName = other.m_itemName;
    m_propertyName = other.m_propertyName;
}

Output::Output(Output &&other)
    : Output()
{
    swap(*this, other);
}

Output &Output::operator=(Output other)
{
    swap(*this, other);
    return *this;
}

Output::~Output()
{
}

QString Output::taskParameter() const
{
    return m_taskParameter;
}

void Output::setTaskParameter(const QString &taskParameter)
{
    m_taskParameter = taskParameter;
}

QString Output::propertyName() const
{
    return m_propertyName;
}

void Output::setPropertyName(const QString &propertyName)
{
    m_propertyName = propertyName;
}

QString Output::itemName() const
{
    return m_itemName;
}

void Output::setItemName(const QString &itemName)
{
    m_itemName = itemName;
}

QString Output::condition() const
{
    return m_condition;
}

void Output::setCondition(const QString &condition)
{
    m_condition = condition;
}

void Output::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
}

QDomNode Output::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(OUTPUT_ATTRIBUTE));

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (!m_taskParameter.isEmpty())
        element.setAttribute(QLatin1String(TASK_PARAMETER_ATTRIBUTE), m_taskParameter);

    if (!m_propertyName.isEmpty())
        element.setAttribute(QLatin1String(PROPERTY_NAME_ATTRIBUTE), m_propertyName);

    if (!m_itemName.isEmpty())
        element.setAttribute(QLatin1String(ITEM_NAME_ATTRIBUTE), m_itemName);

    return element;
}

void Output::swap(Output &first, Output &second)
{
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_itemName, second.m_itemName);
    std::swap(first.m_propertyName, second.m_propertyName);
    std::swap(first.m_taskParameter, second.m_taskParameter);
}

void Output::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(TASK_PARAMETER_ATTRIBUTE))
                m_taskParameter = domElement.value();
            else if (domElement.name() == QLatin1String(PROPERTY_NAME_ATTRIBUTE))
                m_propertyName = domElement.value();
            else if (domElement.name() == QLatin1String(ITEM_NAME_ATTRIBUTE))
                m_itemName = domElement.value();
            else if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
                m_condition = domElement.value();
        }
    }
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
