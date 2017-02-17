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
#include "itemdefinitiongroup.h"

#include "itemdefinition.h"
#include "../vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ItemDefinitionGroup::ItemDefinitionGroup()
{
}

ItemDefinitionGroup::ItemDefinitionGroup(const ItemDefinitionGroup &other)
{
    m_condition = other.m_condition;
    qDeleteAll(m_items);

    foreach (ItemDefinition *itemDef, other.m_items)
        m_items.append(new ItemDefinition(*itemDef));
}

ItemDefinitionGroup::ItemDefinitionGroup(ItemDefinitionGroup &&other)
    : ItemDefinitionGroup()
{
    swap(*this, other);
}

ItemDefinitionGroup &ItemDefinitionGroup::operator=(ItemDefinitionGroup other)
{
    swap(*this, other);
    return *this;
}

ItemDefinitionGroup::~ItemDefinitionGroup()
{
    qDeleteAll(m_items);
}

ItemDefinition *ItemDefinitionGroup::itemDefinition(int index) const
{
    QTC_ASSERT(0 <= index && index < m_items.size(), return nullptr);
    return m_items[index];
}

int ItemDefinitionGroup::itemDefinitionCount() const
{
    return m_items.size();
}

void ItemDefinitionGroup::addItem(ItemDefinition *item)
{
    if (m_items.contains(item))
        return;
    m_items.append(item);
}

void ItemDefinitionGroup::removeItem(ItemDefinition *item)
{
    m_items.removeOne(item);
}

ItemDefinition *ItemDefinitionGroup::findItemDefinition(const QString &name) const
{
    foreach (ItemDefinition *itemDef, m_items) {
        if (itemDef && itemDef->name() == name)
            return itemDef;
    }
    return nullptr;
}

QString ItemDefinitionGroup::condition() const
{
    return m_condition;
}

void ItemDefinitionGroup::setCondition(const QString &condition)
{
    m_condition = condition;
}

void ItemDefinitionGroup::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode ItemDefinitionGroup::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(ITEM_DEFINITION_GROUP_ITEM));

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    QListIterator<ItemDefinition *> it(m_items);
    while (it.hasNext()) {
        ItemDefinition *itemDefinition = it.next();

        if (itemDefinition) {
            QDomNode node = itemDefinition->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void ItemDefinitionGroup::swap(ItemDefinitionGroup &first, ItemDefinitionGroup &second)
{
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_items, second.m_items);
}

void ItemDefinitionGroup::processNodeAttributes(const QDomElement &nodeElement)
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

void ItemDefinitionGroup::processChildNodes(const QDomNode &node)
{
    ItemDefinition *itemDefinition = new ItemDefinition;
    itemDefinition->processNode(node);
    m_items.append(itemDefinition);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
