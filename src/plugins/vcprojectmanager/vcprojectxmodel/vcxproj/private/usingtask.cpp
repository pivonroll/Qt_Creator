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
#include "usingtask.h"

#include "parametergroup.h"
#include "taskbody.h"
#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

UsingTask::UsingTask()
    : m_parameterGroup(nullptr),
      m_taskBody(nullptr)
{
}

UsingTask::UsingTask(const UsingTask &other)
{
    m_assemblyName = other.m_assemblyName;
    m_assemblyFile = other.m_assemblyFile;
    m_taskFactory = other.m_taskFactory;
    m_taskName = other.m_taskName;
    m_condition = other.m_condition;

    delete m_parameterGroup;
    m_parameterGroup = nullptr;
    if (other.m_parameterGroup)
        m_parameterGroup = new ParameterGroup(*other.m_parameterGroup);

    delete m_taskBody;
    m_taskBody = nullptr;
    if (other.m_taskBody)
        m_taskBody = new TaskBody(*other.m_taskBody);
}

UsingTask::UsingTask(UsingTask &&other)
    : UsingTask()
{
    swap(*this, other);
}

UsingTask &UsingTask::operator=(UsingTask other)
{
    swap(*this, other);
    return *this;
}

UsingTask::~UsingTask()
{
    delete m_taskBody;
    delete m_parameterGroup;
}

QString UsingTask::assemblyName() const
{
    return m_assemblyName;
}

void UsingTask::setAssemblyName(const QString &assemblyName)
{
    m_assemblyName = assemblyName;
}

QString UsingTask::assemblyFile() const
{
    return m_assemblyFile;
}

void UsingTask::setAssemblyFile(const QString &assemblyFile)
{
    m_assemblyFile = assemblyFile;
}

QString UsingTask::taskFactory() const
{
    return m_taskFactory;
}

void UsingTask::setTaskFactory(const QString &taskFactory)
{
    m_taskFactory = taskFactory;
}

QString UsingTask::taskName() const
{
    return m_taskName;
}

void UsingTask::setTaskName(const QString &taskName)
{
    m_taskName = taskName;
}

QString UsingTask::condition() const
{
    return m_condition;
}

void UsingTask::setCondition(const QString &condition)
{
    m_condition = condition;
}

ParameterGroup *UsingTask::parameterGroup() const
{
    return m_parameterGroup;
}

void UsingTask::setParameterGroup(ParameterGroup *parameterGroup)
{
    m_parameterGroup = parameterGroup;
}

TaskBody *UsingTask::taskBody() const
{
    return m_taskBody;
}

void UsingTask::setTaskBody(TaskBody *taskBody)
{
    m_taskBody = taskBody;
}

void UsingTask::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode UsingTask::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String("TaskBody"));

    if (!m_assemblyName.isEmpty())
        element.setAttribute(QLatin1String(ASSEMBLY_NAME_ATTRIBUTE), m_assemblyName);

    if (!m_assemblyFile.isEmpty())
        element.setAttribute(QLatin1String(ASSEMBLY_FILE_ATTRIBUTE), m_assemblyFile);

    if (!m_taskFactory.isEmpty())
        element.setAttribute(QLatin1String(TASK_FACTORY_ATTRIBUTE), m_taskFactory);

    if (!m_taskName.isEmpty())
        element.setAttribute(QLatin1String(TASK_NAME_ATTRIBUTE), m_taskName);

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (m_parameterGroup) {
        QDomNode node = m_parameterGroup->toXMLDomNode(domXMLDocument);
        element.appendChild(node);
    }

    if (m_taskBody) {
        QDomNode node = m_taskBody->toXMLDomNode(domXMLDocument);
        element.appendChild(node);
    }

    return element;
}

void UsingTask::swap(UsingTask &first, UsingTask &second)
{
    std::swap(first.m_assemblyFile, second.m_assemblyFile);
    std::swap(first.m_assemblyName, second.m_assemblyName);
    std::swap(first.m_condition, second.m_condition);
    std::swap(first.m_parameterGroup, second.m_parameterGroup);
    std::swap(first.m_taskBody, second.m_taskBody);
    std::swap(first.m_taskFactory, second.m_taskFactory);
    std::swap(first.m_taskName, second.m_taskName);
}

void UsingTask::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(ASSEMBLY_NAME_ATTRIBUTE))
                m_assemblyName = domElement.value();
            else if (domElement.name() == QLatin1String(ASSEMBLY_FILE_ATTRIBUTE))
                m_assemblyFile = domElement.value();
            else if (domElement.name() == QLatin1String(TASK_FACTORY_ATTRIBUTE))
                m_taskFactory = domElement.value();
            else if (domElement.name() == QLatin1String(TASK_NAME_ATTRIBUTE))
                m_taskName = domElement.value();
            else if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
                m_condition = domElement.value();
        }
    }
}

void UsingTask::processChildNodes(const QDomNode &node)
{
    if (node.nodeName() == QLatin1String(PARAMETER_GROUP_ITEM)) {
        m_parameterGroup = new ParameterGroup;
        m_parameterGroup->processNode(node);
    }

    else if (node.nodeName() == QLatin1String(TASK_BODY_ITEM)) {
        m_taskBody = new TaskBody;
        m_taskBody->processNode(node);
    }

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
