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
#include "itemdefinition.h"

#include "itemmetadata.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ItemDefinition::ItemDefinition()
{
}

ItemDefinition::ItemDefinition(const ItemDefinition &other)
{
    m_name = other.m_name;

    qDeleteAll(m_metaData);

    foreach (ItemMetaData *itemMetaData, other.m_metaData)
        m_metaData.append(new ItemMetaData(*itemMetaData));
}

ItemDefinition::ItemDefinition(ItemDefinition &&other)
    : ItemDefinition()
{
    swap(*this, other);
}

ItemDefinition &ItemDefinition::operator=(ItemDefinition other)
{
    swap(*this, other);
    return *this;
}

ItemDefinition::~ItemDefinition()
{
    qDeleteAll(m_metaData);
}

ItemMetaData *ItemDefinition::metaData(int index) const
{
    QTC_ASSERT(0 <= index && index < m_metaData.size(), return nullptr);
    return m_metaData[index];
}

ItemMetaData *ItemDefinition::findItemMetaData(const QString &name) const
{
    foreach (ItemMetaData *item, m_metaData) {
        if (item && item->name() == name)
            return item;
    }

    return nullptr;
}

int ItemDefinition::metaDataCount() const
{
    return m_metaData.size();
}

void ItemDefinition::addMetaData(ItemMetaData *metaData)
{
    if (m_metaData.contains(metaData))
        return;
    m_metaData.append(metaData);
}

void ItemDefinition::removeMetaData(ItemMetaData *metaData)
{
    m_metaData.removeOne(metaData);
}

QString ItemDefinition::name() const
{
    return m_name;
}

void ItemDefinition::setName(const QString &name)
{
    m_name = name;
}

void ItemDefinition::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode ItemDefinition::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(m_name);

    QListIterator<ItemMetaData *> it(m_metaData);
    while (it.hasNext()) {
        ItemMetaData *metaData = it.next();

        if (metaData) {
            QDomNode node = metaData->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void ItemDefinition::swap(ItemDefinition &first, ItemDefinition &second)
{
    std::swap(first.m_metaData, second.m_metaData);
    std::swap(first.m_name, second.m_name);
}

void ItemDefinition::processNodeAttributes(const QDomElement &domElement)
{
    m_name = domElement.nodeName();
}

void ItemDefinition::processChildNodes(const QDomNode &node)
{
    ItemMetaData *itemMetaElement = new ItemMetaData;
    itemMetaElement->processNode(node);
    m_metaData.append(itemMetaElement);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
