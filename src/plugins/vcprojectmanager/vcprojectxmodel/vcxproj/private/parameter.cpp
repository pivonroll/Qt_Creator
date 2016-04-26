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
#include "parameter.h"

#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Parameter::Parameter()
{
}

Parameter::Parameter(const Parameter &other)
{
    m_name = other.m_name;
    m_output = other.m_output;
    m_parameterType = other.m_parameterType;
    m_required = other.m_required;
}

Parameter::Parameter(Parameter &&other)
    : Parameter()
{
    swap(*this, other);
}

Parameter &Parameter::operator=(Parameter other)
{
    swap(*this, other);
    return *this;
}

Parameter::~Parameter()
{
}

QString Parameter::parameterType() const
{
    return m_parameterType;
}

void Parameter::setParameterType(const QString &parameterType)
{
    m_parameterType = parameterType;
}

QString Parameter::output() const
{
    return m_output;
}

void Parameter::setOutput(const QString &output)
{
    m_output = output;
}

QString Parameter::required() const
{
    return m_required;
}

void Parameter::setRequired(const QString &required)
{
    m_required = required;
}

QString Parameter::name() const
{
    return m_name;
}

void Parameter::setName(const QString &name)
{
    m_name = name;
}

void Parameter::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
}

QDomNode Parameter::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(m_name);

    if (!m_output.isEmpty())
        element.setAttribute(QLatin1String(OUTPUT_ATTRIBUTE), m_output);

    if (!m_parameterType.isEmpty())
        element.setAttribute(QLatin1String(PARAMETER_TYPE_ATTRIBUTE), m_parameterType);

    if (!m_required.isEmpty())
        element.setAttribute(QLatin1String(REQUIRED_ATTRIBUTE), m_required);

    return element;
}

void Parameter::swap(Parameter &first, Parameter &second)
{
    std::swap(first.m_name, second.m_name);
    std::swap(first.m_output, second.m_output);
    std::swap(first.m_parameterType, second.m_parameterType);
    std::swap(first.m_required, second.m_required);
}

void Parameter::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(PARAMETER_TYPE_ATTRIBUTE))
                m_parameterType = domElement.value();
            else if (domElement.name() == QLatin1String(OUTPUT_ATTRIBUTE))
                m_output = domElement.value();
            else if (domElement.name() == QLatin1String(REQUIRED_ATTRIBUTE))
                m_required = domElement.value();
        }
    }

    m_name = nodeElement.nodeName();
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
