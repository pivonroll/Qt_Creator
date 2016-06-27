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
#include "importgroup.h"

#include "import.h"
#include "../vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ImportGroup::ImportGroup()
{
}

ImportGroup::ImportGroup(const ImportGroup &other)
{
    m_label = other.m_label;
    m_condition = other.m_condition;

    qDeleteAll(m_importElements);

    foreach (Import *import, other.m_importElements)
        m_importElements.append(new Import(*import));
}

ImportGroup::ImportGroup(ImportGroup &&other)
    : ImportGroup()
{
    swap(*this, other);
}

ImportGroup &ImportGroup::operator=(ImportGroup other)
{
    swap(*this, other);
    return *this;
}

ImportGroup::~ImportGroup()
{
    qDeleteAll(m_importElements);
}

QString ImportGroup::condition() const
{
    return m_condition;
}

void ImportGroup::setCondition(const QString &condition)
{
    m_condition = condition;
}

QString ImportGroup::label() const
{
    return m_label;
}

void ImportGroup::setLabel(const QString &label)
{
    m_label = label;
}

Import *ImportGroup::importElement(int index) const
{
    QTC_ASSERT(0 <= index && index < m_importElements.size(), return nullptr);
    return m_importElements[index];
}

int ImportGroup::importElementCount() const
{
    return m_importElements.size();
}

void ImportGroup::addImportElement(Import *import)
{
    if (m_importElements.contains(import))
        return;
    m_importElements.append(import);
}

void ImportGroup::removeImportElement(Import *import)
{
    m_importElements.removeOne(import);
}

void ImportGroup::processNode(const QDomNode &node)
{
    if (node.isElement())
        processAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode ImportGroup::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(IMPORT_GROUP_ITEM));

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (!m_label.isEmpty())
        element.setAttribute(QLatin1String(LABEL_ATTRIBUTE), m_label);

    QListIterator<Import *> it(m_importElements);
    while (it.hasNext()) {
        Import *import = it.next();

        if (import) {
            QDomNode node = import->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void ImportGroup::swap(ImportGroup &first, ImportGroup &second)
{
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_importElements, second.m_importElements);
    std::swap(first.m_label, second.m_label);
}

void ImportGroup::processChildNodes(const QDomNode &node)
{
    Import *importElement = new Import;
    importElement->processNode(node);
    m_importElements.append(importElement);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

void ImportGroup::processAttributes(const QDomElement &nodeElement)
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


} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
