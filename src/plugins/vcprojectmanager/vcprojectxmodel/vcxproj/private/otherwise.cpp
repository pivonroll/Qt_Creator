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
#include "otherwise.h"

#include "choose.h"
#include "itemgroup.h"
#include "propertygroup.h"

#include "../vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Otherwise::Otherwise()
{
}

Otherwise::Otherwise(const Otherwise &other)
{
    qDeleteAll(m_chooseElements);
    qDeleteAll(m_itemGroupElements);
    qDeleteAll(m_propertyGroupElements);

    foreach (Choose *choose, other.m_chooseElements)
            addChooseElement(new Choose(*choose));

    foreach (ItemGroup *itemGroup, other.m_itemGroupElements)
            addItemGroup(new ItemGroup(*itemGroup));

    foreach (PropertyGroup *propertyGroup, other.m_propertyGroupElements)
        addPropertyGroup(new PropertyGroup(*propertyGroup));
}

Otherwise::Otherwise(Otherwise &&other)
    : Otherwise()
{
    swap(*this, other);
}

Otherwise &Otherwise::operator=(Otherwise other)
{
    swap(*this, other);
    return *this;
}

Otherwise::~Otherwise()
{
    qDeleteAll(m_chooseElements);
    qDeleteAll(m_itemGroupElements);
    qDeleteAll(m_propertyGroupElements);
}

Choose *Otherwise::chooseElement(int index) const
{
    QTC_ASSERT(0 <= index && index < m_chooseElements.size(), return nullptr);
    return m_chooseElements[index];
}

int Otherwise::chooseElementCount() const
{
    return m_chooseElements.size();
}

void Otherwise::addChooseElement(Choose *choose)
{
    if (m_chooseElements.contains(choose))
        return;
    m_chooseElements.append(choose);
    m_nodeList.append(choose);
}

void Otherwise::removeChooseElement(Choose *choose)
{
    m_chooseElements.removeOne(choose);
    m_nodeList.removeOne(choose);
}

ItemGroup *Otherwise::itemGroupElement(int index) const
{
    QTC_ASSERT(0 <= index && index < m_itemGroupElements.size(), return nullptr);
    return m_itemGroupElements[index];
}

int Otherwise::itemGroupElementCount() const
{
    return m_itemGroupElements.size();
}

void Otherwise::addItemGroup(ItemGroup *itemGroup)
{
    if (m_itemGroupElements.contains(itemGroup))
        return;
    m_itemGroupElements.append(itemGroup);
    m_nodeList.append(itemGroup);
}

void Otherwise::removeItemGroup(ItemGroup *itemGroup)
{
    m_itemGroupElements.removeOne(itemGroup);
    m_nodeList.removeOne(itemGroup);
}

PropertyGroup *Otherwise::propertyGroupElement(int index) const
{
    QTC_ASSERT(0 <= index && index < m_propertyGroupElements.size(), return nullptr);
    return m_propertyGroupElements[index];
}

int Otherwise::propertyGroupElementCount() const
{
    return m_propertyGroupElements.size();
}

void Otherwise::addPropertyGroup(PropertyGroup *propertyGroup)
{
    if (m_propertyGroupElements.contains(propertyGroup))
        return;
    m_propertyGroupElements.append(propertyGroup);
    m_nodeList.append(propertyGroup);
}

void Otherwise::removePropertyGroup(PropertyGroup *propertyGroup)
{
    m_propertyGroupElements.removeOne(propertyGroup);
    m_nodeList.removeOne(propertyGroup);
}

void Otherwise::processNode(const QDomNode &node)
{
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode Otherwise::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(OTHERWISE_ITEM));

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

void Otherwise::swap(Otherwise &first, Otherwise &second)
{
    std::swap(first.m_chooseElements, second.m_chooseElements);
    std::swap(first.m_itemGroupElements, second.m_itemGroupElements);
    std::swap(first.m_nodeList, second.m_nodeList);
    std::swap(first.m_propertyGroupElements, second.m_propertyGroupElements);
}

void Otherwise::processChildNodes(const QDomNode &node)
{
    if (node.nodeName() == QLatin1String(CHOOSE_ITEM)) {
        Choose *choose = new Choose;
        choose->processNode(node);
        m_chooseElements.append(choose);
        m_nodeList.append(choose);
    }

    else if (node.nodeName() == QLatin1String(ITEM_GROUP_ITEM)) {
        ItemGroup *itemGroup = new ItemGroup;
        itemGroup->processNode(node);
        m_itemGroupElements.append(itemGroup);
        m_nodeList.append(itemGroup);
    }

    else if (node.nodeName() == QLatin1String(PROPERTY_GROUP_ITEM)) {
        PropertyGroup *propertyGroup = new PropertyGroup;
        propertyGroup->processNode(node);
        m_propertyGroupElements.append(propertyGroup);
        m_nodeList.append(propertyGroup);
    }

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
