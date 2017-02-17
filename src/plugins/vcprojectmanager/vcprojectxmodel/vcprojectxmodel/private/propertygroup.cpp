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
#include "propertygroup.h"

#include "property.h"
#include "../vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

PropertyGroup::PropertyGroup()
{
}

PropertyGroup::PropertyGroup(const PropertyGroup &other)
{
    m_condition = other.m_condition;
    m_label = other.m_label;

    qDeleteAll(m_properties);

    foreach (Property *prop, other.m_properties)
        m_properties.append(new Property(*prop));
}

PropertyGroup::PropertyGroup(PropertyGroup &&other)
    : PropertyGroup()
{
    swap(*this, other);
}

PropertyGroup &PropertyGroup::operator=(PropertyGroup other)
{
    swap(*this, other);
    return *this;
}

PropertyGroup::~PropertyGroup()
{
    qDeleteAll(m_properties);
}

QString PropertyGroup::condition() const
{
    return m_condition;
}

void PropertyGroup::setCondition(const QString &condition)
{
    m_condition = condition;
}

Property *PropertyGroup::property(int index) const
{
    QTC_ASSERT(0 <= index && index < m_properties.size(), return nullptr);
    return m_properties[index];
}

int PropertyGroup::propertyCount() const
{
    return m_properties.size();
}

void PropertyGroup::addProperty(Property *property)
{
    if (m_properties.contains(property))
        return;
    m_properties.append(property);
}

void PropertyGroup::removeProperty(Property *property)
{
    m_properties.removeOne(property);
}

Property *PropertyGroup::findProperty(const QString &propertyName) const
{
    foreach (Property *property, m_properties) {
        if (property && property->name() == propertyName)
            return property;
    }
    return nullptr;
}

void PropertyGroup::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());

    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode PropertyGroup::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(PROPERTY_GROUP_ITEM));

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (!m_label.isEmpty())
        element.setAttribute(QLatin1String(LABEL_ATTRIBUTE), m_label);

    QListIterator<Property *> it(m_properties);
    while (it.hasNext()) {
        Property *property = it.next();

        if (property) {
            QDomNode node = property->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void PropertyGroup::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
                m_condition = domElement.value();
            else if (domElement.name() == QLatin1String(LABEL_ATTRIBUTE))
                m_label = domElement.value();
        }
    }
}

void PropertyGroup::processChildNodes(const QDomNode &node)
{
    Property *property = new Property;
    property->processNode(node);
    m_properties.append(property);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}
QString PropertyGroup::label() const
{
    return m_label;
}

void PropertyGroup::setLabel(const QString &label)
{
    m_label = label;
}

void PropertyGroup::swap(PropertyGroup &first, PropertyGroup &second)
{
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_label, second.m_label);
    std::swap(first.m_properties, second.m_properties);
}


} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
