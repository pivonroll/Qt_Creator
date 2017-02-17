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
#include "when.h"

#include "choose.h"
#include "itemgroup.h"
#include "propertygroup.h"
#include "../vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

When::When()
{
}

When::When(const When &other)
{
    m_condition = other.m_condition;

    qDeleteAll(m_chooseElements);
    qDeleteAll(m_propertyGroups);
    qDeleteAll(m_itemGroups);

    foreach (Choose *choose, other.m_chooseElements)
        addChooseElement(new Choose(*choose));

    foreach (PropertyGroup *propertyGroup, other.m_propertyGroups)
        addPropertyGroup(new PropertyGroup(*propertyGroup));

    foreach (ItemGroup *itemGroup, other.m_itemGroups)
        addItemGroup(new ItemGroup(*itemGroup));
}

When::When(When &&other)
    : When()
{
    swap(*this, other);
}

When &When::operator=(When other)
{
    swap(*this, other);
    return *this;
}

When::~When()
{
    qDeleteAll(m_chooseElements);
    qDeleteAll(m_propertyGroups);
    qDeleteAll(m_itemGroups);
}

QString When::condition() const
{
    return m_condition;
}

void When::setCondition(const QString &condition)
{
    m_condition = condition;
}

Choose *When::chooseElement(int index) const
{
    QTC_ASSERT(0 <= index && index < m_chooseElements.size(), return nullptr);
    return m_chooseElements[index];
}

int When::chooseElementCount() const
{
    return m_chooseElements.size();
}

void When::addChooseElement(Choose *choose)
{
    if (m_chooseElements.contains(choose))
        return;
    m_chooseElements.append(choose);
    m_nodeList.append(choose);
}

void When::removeChoose(Choose *choose)
{
    m_chooseElements.removeOne(choose);
    m_nodeList.removeOne(choose);
}

ItemGroup *When::itemGroup(int index) const
{
    QTC_ASSERT(0 <= index && index < m_itemGroups.size(), return nullptr);
    return m_itemGroups[index];
}

int When::itemGroupCount() const
{
    return m_itemGroups.size();
}

void When::addItemGroup(ItemGroup *itemGroup)
{
    if (m_itemGroups.contains(itemGroup))
        return;
    m_itemGroups.append(itemGroup);
    m_nodeList.append(itemGroup);
}

void When::removeItemGroup(ItemGroup *itemGroup)
{
    m_itemGroups.removeOne(itemGroup);
    m_nodeList.removeOne(itemGroup);
}

PropertyGroup *When::propertyGroup(int index) const
{
    QTC_ASSERT(0 <= index && index < m_propertyGroups.size(), return nullptr);
    return m_propertyGroups[index];
}

int When::propertyGroupCount() const
{
    return m_propertyGroups.size();
}

void When::addPropertyGroup(PropertyGroup *propertyGroup)
{
    if (m_propertyGroups.contains(propertyGroup))
        return;
    m_propertyGroups.append(propertyGroup);
    m_nodeList.append(propertyGroup);
}

void When::removePropertyGroup(PropertyGroup *propertyGroup)
{
    m_propertyGroups.removeOne(propertyGroup);
    m_nodeList.removeOne(propertyGroup);
}

void When::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode When::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(TASK_BODY_ITEM));

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    QListIterator<IVisualStudioNodeX *> it(m_nodeList);
    while (it.hasNext()) {
        IVisualStudioNodeX *visualStudioNode = it.next();

        if (visualStudioNode) {
            QDomNode node = visualStudioNode->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void When::swap(When &first, When &second)
{
    std::swap(first.m_chooseElements, second.m_chooseElements);
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_itemGroups, second.m_itemGroups);
    std::swap(first.m_nodeList, second.m_nodeList);
    std::swap(first.m_propertyGroups, second.m_propertyGroups);
}

void When::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();
    QDomNode domNode = namedNodeMap.item(0);

    if (domNode.nodeType() == QDomNode::AttributeNode) {
        QDomAttr domElement = domNode.toAttr();

        if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
            m_condition = domElement.value();
    }
}

void When::processChildNodes(const QDomNode &node)
{
    if (node.nodeName() == QLatin1String(PROPERTY_GROUP_ITEM)) {
        PropertyGroup *propertyGroup = new PropertyGroup;
        propertyGroup->processNode(node);
        m_propertyGroups.append(propertyGroup);
        m_nodeList.append(propertyGroup);
    }

    else if (node.nodeName() == QLatin1String(ITEM_GROUP_ITEM)) {
        ItemGroup *itemGroup = new ItemGroup;
        itemGroup->processNode(node);
        m_itemGroups.append(itemGroup);
        m_nodeList.append(itemGroup);
    }

    else if (node.nodeName() == QLatin1String(CHOOSE_ITEM)) {
        Choose *choose = new Choose;
        choose->processNode(node);
        m_chooseElements.append(choose);
        m_nodeList.append(choose);
    }

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
