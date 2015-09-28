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
#include "task.h"

#include "output.h"
#include "parameter.h"
#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Task::Task()
{
}

Task::Task(const Task &task)
{
    m_condition = task.condition();
    m_continueOnError = task.continueOnError();
    m_name = task.name();
    m_parameters = task.m_parameters;

    qDeleteAll(m_outputs);

    for (int i = 0; i < task.outputCount(); ++i) {
        Output *output = task.output(i);

        if (output)
            m_outputs.append(new Output(*output));
    }
}

Task::~Task()
{
    qDeleteAll(m_outputs);
}

QString Task::condition() const
{
    return m_condition;
}

void Task::setCondition(const QString &condition)
{
    m_condition = condition;
}

QString Task::continueOnError() const
{
    return m_continueOnError;
}

void Task::setContinueOnError(const QString &continueOnError)
{
    m_continueOnError = continueOnError;
}

Output *Task::output(int index) const
{
    if (0 <= index && index < m_outputs.size())
        return m_outputs[index];
    return 0;
}

int Task::outputCount() const
{
    return m_outputs.size();
}

void Task::addOutput(Output *output)
{
    if (m_outputs.contains(output))
        return;
    m_outputs.append(output);
}

void Task::removeOutput(Output *output)
{
    m_outputs.removeOne(output);
}

QString Task::name() const
{
    return m_name;
}

void Task::setName(const QString &name)
{
    m_name = name;
}

void Task::setParameter(const QString &parameterName, const QString &parameterValue)
{
    m_parameters.insert(parameterName, parameterValue);
}

QString Task::parameterValue(const QString &parameterName) const
{
    return m_parameters.value(parameterName);
}

void Task::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode Task::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(m_name);

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (!m_continueOnError.isEmpty())
        element.setAttribute(QLatin1String(CONTINUE_ON_ERROR_ATTRIBUTE), m_continueOnError);

    QHashIterator<QString, QString> it(m_parameters);
    while (it.hasNext()) {
        it.next();
        element.setAttribute(it.key(), it.value());
    }

    QListIterator<Output *> itOutput(m_outputs);
    while (itOutput.hasNext()) {
        Output *output = itOutput.next();

        if (output) {
            QDomNode node = output->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }
    return element;
}

void Task::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
                m_condition = domElement.value();
            else if (domElement.name() == QLatin1String(CONTINUE_ON_ERROR_ATTRIBUTE))
                m_continueOnError = domElement.value();
            else
                m_parameters.insert(domElement.nodeName(), domElement.value());
        }
    }
    m_name = nodeElement.nodeName();
}

void Task::processChildNodes(const QDomNode &node)
{
    Output *output = new Output;
    output->processNode(node);
    m_outputs.append(output);

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}


} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
