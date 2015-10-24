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
#include "import.h"

#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Import::Import()
{
}

Import::Import(const Import &import)
{
    m_project = import.project();
    m_condition = import.condition();
}

Import::~Import()
{
}

QString Import::condition() const
{
    return m_condition;
}

void Import::setCondition(const QString &condition)
{
    m_condition = condition;
}

QString Import::project() const
{
    return m_project;
}

void Import::setProject(const QString &project)
{
    m_project = project;
}

void Import::processNode(const QDomNode &node)
{
    if (node.isNull() || !node.isElement())
        return;

    // process attributes
    QDomNamedNodeMap namedNodeMap = node.toElement().attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
                m_condition = domElement.value();

            else if (domElement.name() == QLatin1String(PROJECT_ATTRIBUTE))
                m_project = domElement.value();
        }
    }
}

QDomNode Import::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(IMPORT_ITEM));

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (!m_project.isEmpty())
        element.setAttribute(QLatin1String(PROJECT_ATTRIBUTE), m_project);

    return element;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
