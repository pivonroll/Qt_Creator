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
#include "itemgroup.h"

#include "item.h"
#include "../vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ItemGroup::ItemGroup()
{
}

ItemGroup::ItemGroup(const ItemGroup &other)
{
    m_condition = other.m_condition;
    m_label = other.m_label;
    qDeleteAll(m_items);

    foreach (Item *item, other.m_items)
        m_items.append(new Item(*item));
}

ItemGroup::ItemGroup(ItemGroup &&other)
    : ItemGroup()
{
    swap(*this, other);
}

ItemGroup &ItemGroup::operator=(ItemGroup other)
{
    swap(*this, other);
    return *this;
}

ItemGroup::~ItemGroup()
{
    qDeleteAll(m_items);
}

Item *ItemGroup::item(int index) const
{
    QTC_ASSERT(0 <= index && index < m_items.size(), return nullptr);
    return m_items[index];
}

int ItemGroup::itemCount() const
{
    return m_items.size();
}

void ItemGroup::addItem(Item *item)
{
    if (m_items.contains(item))
        return;
    m_items.append(item);
}

void ItemGroup::removeItem(Item *item)
{
    m_items.removeOne(item);
}

Item *ItemGroup::findItemWithInclude(const QString &include) const
{
    foreach (Item *item, m_items) {
        if (item && item->include() == include)
            return item;
    }

    return nullptr;
}

Item *ItemGroup::findItemWithName(const QString &name) const
{
    foreach (Item *item, m_items) {
        if (item && item->name() == name)
            return item;
    }

    return nullptr;
}

QString ItemGroup::condition() const
{
    return m_condition;
}

void ItemGroup::setCondition(const QString &condition)
{
    m_condition = condition;
}

void ItemGroup::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode ItemGroup::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(ITEM_GROUP_ITEM));

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (!m_label.isEmpty())
        element.setAttribute(QLatin1String(LABEL_ATTRIBUTE), m_label);

    QListIterator<Item *> it(m_items);
    while (it.hasNext()) {
        Item *item = it.next();

        if (item) {
            QDomNode node = item->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void ItemGroup::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();
    QDomNode domNode = namedNodeMap.item(0);

    if (domNode.nodeType() == QDomNode::AttributeNode) {
        QDomAttr domElement = domNode.toAttr();

        if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
            m_condition = domElement.value();
        else if (domElement.name() == QLatin1String(LABEL_ATTRIBUTE))
            m_label = domElement.value();
    }
}

void ItemGroup::processChildNodes(const QDomNode &node)
{
    Item *item = new Item;
    item->processNode(node);
    m_items.append(item);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}
QString ItemGroup::label() const
{
    return m_label;
}

void ItemGroup::setLabel(const QString &label)
{
    m_label = label;
}

void ItemGroup::swap(ItemGroup &first, ItemGroup &second)
{
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_items, second.m_items);
    std::swap(first.m_label, second.m_label);
}


} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
