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
#include "item.h"

#include "itemmetadata.h"
#include "../vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Item::Item()
{
}

Item::Item(const QString &include)
    : m_include(include)
{
}

Item::Item(const Item &other)
{
    m_include = other.m_include;
    m_exclude = other.m_exclude;
    m_remove = other.m_remove;
    m_condition = other.m_condition;
    m_name = other.m_name;

    qDeleteAll(m_itemMetaData);

    foreach (ItemMetaData *itemMetaData, other.m_itemMetaData)
        m_itemMetaData.append(new ItemMetaData(*itemMetaData));
}

Item::Item(Item &&other)
    : Item()
{
    swap(*this, other);
}

Item &Item::operator=(Item other)
{
    swap(*this, other);
    return *this;
}

Item::~Item()
{
    qDeleteAll(m_itemMetaData);
}

ItemMetaData *Item::itemMetaData(int index) const
{
    QTC_ASSERT(0 <= index && index < m_itemMetaData.size(), return nullptr);
    return m_itemMetaData[index];
}

int Item::itemMetaDataCount() const
{
    return m_itemMetaData.size();
}

void Item::addItemMetaData(ItemMetaData *metaData)
{
    if (m_itemMetaData.contains(metaData))
        return;
    m_itemMetaData.append(metaData);
}

void Item::removeMetaData(ItemMetaData *metaData)
{
    m_itemMetaData.removeOne(metaData);
}

ItemMetaData *Item::findMetaDataWithName(const QString &name) const
{
    foreach (ItemMetaData *metaData, m_itemMetaData) {
        if (metaData && metaData->name() == name)
            return metaData;
    }

    return nullptr;
}

QString Item::include() const
{
    return m_include;
}

void Item::setInclude(const QString &include)
{
    m_include = include;
}

QString Item::exclude() const
{
    return m_exclude;
}

void Item::setExclude(const QString &exclude)
{
    m_exclude = exclude;
}

QString Item::remove() const
{
    return m_remove;
}

void Item::setRemove(const QString &remove)
{
    m_remove = remove;
}

QString Item::condition() const
{
    return m_condition;
}

void Item::setCondition(const QString &condition)
{
    m_condition = condition;
}

QString Item::name() const
{
    return m_name;
}

void Item::setName(const QString &name)
{
    m_name = name;
}

void Item::processNode(const QDomNode &node)
{
    if (node.isElement())
        processAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode Item::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(m_name);

    if (!m_include.isEmpty())
        element.setAttribute(QLatin1String(INCLUDE_ATTRIBUTE), m_include);

    if (!m_exclude.isEmpty())
        element.setAttribute(QLatin1String(EXCLUDE_ATTRIBUTE), m_exclude);

    if (!m_remove.isEmpty())
        element.setAttribute(QLatin1String(REMOVE_ATTRIBUTE), m_remove);

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    QListIterator<ItemMetaData *> it(m_itemMetaData);
    while (it.hasNext()) {
        ItemMetaData *itemMetaData = it.next();

        if (itemMetaData) {
            QDomNode node = itemMetaData->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void Item::swap(Item &first, Item &second)
{
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_exclude, second.m_exclude);
    std::swap(first.m_include, second.m_include);
    std::swap(first.m_itemMetaData, second.m_itemMetaData);
    std::swap(first.m_name, second.m_name);
    std::swap(first.m_remove, second.m_remove);
}

void Item::processAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
                m_condition = domElement.value();
            else if (domElement.name() == QLatin1String(INCLUDE_ATTRIBUTE))
                m_include = domElement.value();
            else if (domElement.name() == QLatin1String(EXCLUDE_ATTRIBUTE))
                m_exclude = domElement.value();
            else if (domElement.name() == QLatin1String(REMOVE_ATTRIBUTE))
                m_remove = domElement.value();
        }
    }

    m_name = nodeElement.nodeName();
}

void Item::processChildNodes(const QDomNode &node)
{
    ItemMetaData *itemMetaElement = new ItemMetaData;
    itemMetaElement->processNode(node);
    m_itemMetaData.append(itemMetaElement);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
