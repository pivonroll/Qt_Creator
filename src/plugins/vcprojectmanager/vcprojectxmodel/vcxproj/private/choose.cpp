/**************************************************************************
**
** Copyright (c) 2014 Bojan Petrovic
** Copyright (c) 2014 Radovan Zivkovic
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
#include "choose.h"

#include "otherwise.h"
#include "when.h"

#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Choose::Choose()
    : m_otherwise(0)
{
}

Choose::Choose(const Choose &choose)
{
    delete m_otherwise;
    m_otherwise = new Otherwise(*(choose.otherwise()));

    qDeleteAll(m_whenElements);

    for (int i = 0; i < choose.whenElementCount(); ++i) {
        When *when = choose.whenElement(i);

        if (when)
            m_whenElements.append(new When(*when));
    }
}

Choose::~Choose()
{
    qDeleteAll(m_whenElements);
    delete m_otherwise;
}

Otherwise *Choose::otherwise() const
{
    return m_otherwise;
}

void Choose::setOtherwise(Otherwise *otherwise)
{
    if (m_otherwise)
        delete m_otherwise;
    m_otherwise = otherwise;
}

When *Choose::whenElement(int index) const
{
    if (0 <= index && index < m_whenElements.size())
        return m_whenElements[index];
    return 0;
}

int Choose::whenElementCount() const
{
    return m_whenElements.size();
}

void Choose::addWhenElement(When *when)
{
    if (m_whenElements.contains(when))
        return;
    m_whenElements.append(when);
}

void Choose::removeWhenElement(When *when)
{
    m_whenElements.removeOne(when);
}

void Choose::processNode(const QDomNode &node)
{
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode Choose::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(CHOOSE_ITEM));

    if (m_otherwise) {
        QDomNode otherwiseNode = m_otherwise->toXMLDomNode(domXMLDocument);
        element.appendChild(otherwiseNode);
    }

    QListIterator<When *> it(m_whenElements);
    while (it.hasNext()) {
        When *when = it.next();

        if (when) {
            QDomNode whenNode = when->toXMLDomNode(domXMLDocument);
            element.appendChild(whenNode);
        }
    }

    return element;
}

void Choose::processChildNodes(const QDomNode &node)
{
    if (node.nodeName() == QLatin1String(OTHERWISE_ITEM) && !m_otherwise) {

        m_otherwise = new Otherwise;
        m_otherwise->processNode(node);
    }

    else if (node.nodeName() == QLatin1String(WHEN_ITEM)) {
        When *whenElement = new When;
        whenElement->processNode(node);
    }

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
