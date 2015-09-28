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
#include "parametergroup.h"

#include "parameter.h"
#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ParameterGroup::ParameterGroup()
{
}

ParameterGroup::ParameterGroup(const ParameterGroup &parameterGroup)
{
    qDeleteAll(m_parameters);

    for (int i = 0; i < parameterGroup.parameterCount(); ++i) {
        Parameter *parameter = parameterGroup.parameter(i);

        if (parameter)
            m_parameters.append(new Parameter(*parameter));
    }
}

ParameterGroup::~ParameterGroup()
{
    qDeleteAll(m_parameters);
}

Parameter *ParameterGroup::parameter(int index) const
{
    if (0 <= index && index < m_parameters.size())
        return m_parameters[index];
    return 0;
}

int ParameterGroup::parameterCount() const
{
    return m_parameters.size();
}

void ParameterGroup::addParameter(Parameter *parameter)
{
    if (m_parameters.contains(parameter))
        return;
    m_parameters.append(parameter);
}

void ParameterGroup::removeParameter(Parameter *parameter)
{
    m_parameters.removeOne(parameter);
}

void ParameterGroup::processNode(const QDomNode &node)
{
    if (node.hasChildNodes())
        processChildNodes(node);
}

QDomNode ParameterGroup::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(PARAMETER_GROUP_ITEM));

    QListIterator<Parameter *> it(m_parameters);
    while (it.hasNext()) {
        Parameter *parameter = it.next();

        if (parameter) {
            QDomNode node = parameter->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    return element;
}

void ParameterGroup::processChildNodes(const QDomNode &node)
{
    Parameter *parameter = new Parameter;
    parameter->processNode(node);
    m_parameters.append(parameter);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
