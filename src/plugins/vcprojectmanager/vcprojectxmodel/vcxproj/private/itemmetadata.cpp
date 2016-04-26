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
#include "itemmetadata.h"

#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ItemMetaData::ItemMetaData()
{
}

ItemMetaData::ItemMetaData(const ItemMetaData &other)
{
    m_name = other.m_name;
    m_condition = other.m_condition;
    m_value = other.m_value;
}

ItemMetaData::ItemMetaData(ItemMetaData &&other)
    : ItemMetaData()
{
    swap(*this, other);
}

ItemMetaData &ItemMetaData::operator=(ItemMetaData other)
{
    swap(*this, other);
    return *this;
}

ItemMetaData::~ItemMetaData()
{
}

QString ItemMetaData::condition() const
{
    return m_condition;
}

void ItemMetaData::setCondition(const QString &condition)
{
    m_condition = condition;
}

QString ItemMetaData::name() const
{
    return m_name;
}

void ItemMetaData::setName(const QString &name)
{
    m_name = name;
}

QString ItemMetaData::value() const
{
    return m_value;
}

void ItemMetaData::setValue(const QString &value)
{
    m_value = value;
}

void ItemMetaData::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
}

QDomNode ItemMetaData::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(m_name);

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    QDomText valueText = domXMLDocument.createTextNode(m_value);
    element.appendChild(valueText);

    return element;
}

void ItemMetaData::swap(ItemMetaData &first, ItemMetaData &second)
{
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_name, second.m_name);
    std::swap(first.m_value, second.m_value);
}

void ItemMetaData::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();
    QDomNode domNode = namedNodeMap.item(0);

    if (domNode.nodeType() == QDomNode::AttributeNode) {
        QDomAttr domElement = domNode.toAttr();

        if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
            m_condition = domElement.value();
    }


    m_name = nodeElement.nodeName();
    m_value = nodeElement.text();
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
