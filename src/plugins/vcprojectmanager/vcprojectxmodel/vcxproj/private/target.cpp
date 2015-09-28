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
#include "target.h"

#include "task.h"
#include "propertygroup.h"
#include "itemgroup.h"
#include "onerror.h"
#include "../vcprojx_constants.h"

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Target::Target()
    : m_keepDuplicateOutputs(QLatin1String(KEEP_DUPLICATE_OUTPUTS_FALSE))
{
}

Target::Target(const Target &target)
{
    m_name = target.name(); // required
    m_condition = target.condition();
    m_inputs = target.inputs();
    m_outputs = target.outputs();
    m_returns = target.returns();
    m_keepDuplicateOutputs = target.keepDuplicateOutputs();
    m_beforeTargets = target.beforeTargets();
    m_afterTargets = target.afterTargets();
    m_dependsOnTargets = target.dependsOnTargets();
    m_label = target.label();

    qDeleteAll(m_tasks);
    qDeleteAll(m_propertyGroups);
    qDeleteAll(m_itemGroups);
    qDeleteAll(m_onErrorElements);

    m_nodeList.clear();

    for (int i = 0; i < target.taskCount(); ++i) {
        Task *task = target.task(i);

        if (task)
            addTask(new Task(*task));
    }
    for (int i = 0; i < target.propertyGroupCount(); ++i) {
        PropertyGroup *propertyGroup = target.propertyGroup(i);

        if (propertyGroup)
            addPropertyGroup(new PropertyGroup(*propertyGroup));
    }
    for (int i = 0; i < target.itemGroupCount(); ++i) {
        ItemGroup *itemGroup = target.itemGroup(i);

        if (itemGroup)
            addItemGroup(new ItemGroup(*itemGroup));
    }
    for (int i = 0; i < target.onErrorElementCount(); ++i) {
        OnError *onError = target.onErrorElement(i);

        if (onError)
            addOnErrorElement(new OnError(*onError));
    }
}

Target::~Target()
{
    qDeleteAll(m_tasks);
    qDeleteAll(m_propertyGroups);
    qDeleteAll(m_itemGroups);
    qDeleteAll(m_onErrorElements);
}

QString Target::name() const
{
    return m_name;
}

void Target::setName(const QString &name)
{
    m_name = name;
}

QString Target::condition() const
{
    return m_condition;
}

void Target::setCondition(const QString &condition)
{
    m_condition = condition;
}

QStringList Target::inputs() const
{
    return m_inputs;
}

void Target::setInputs(const QStringList &inputs)
{
    m_inputs = inputs;
}

void Target::addInput(const QString &input)
{
    if (m_inputs.contains(input))
        return;
    m_inputs.append(input);
}

void Target::removeInput(const QString &input)
{
    m_inputs.removeOne(input);
}

QStringList Target::outputs() const
{
    return m_outputs;
}

void Target::setOutputs(const QStringList &outputs)
{
    m_outputs = outputs;
}

void Target::addOutput(const QString &output)
{
    if (m_outputs.contains(output))
        return;
    m_outputs.append(output);
}

void Target::removeOutput(const QString &output)
{
    m_outputs.removeOne(output);
}

QStringList Target::returns() const
{
    return m_returns;
}

void Target::setReturns(const QStringList &returns)
{
    m_returns = returns;
}

void Target::addReturn(const QString &ret)
{
    if (m_returns.contains(ret) && (m_keepDuplicateOutputs) == QLatin1String(KEEP_DUPLICATE_OUTPUTS_FALSE))
        return;
    m_returns.append(ret);
}

void Target::removeReturn(const QString &ret)
{
    m_returns.removeOne(ret);
}

QStringList Target::beforeTargets() const
{
    return m_beforeTargets;
}

void Target::setBeforeTargets(const QStringList &beforeTargets)
{
    m_beforeTargets = beforeTargets;
}

void Target::addBeforeTarget(const QString &target)
{
    if (m_beforeTargets.contains(target))
        return;
    m_beforeTargets.append(target);
}

void Target::removeBeforeTarget(const QString &target)
{
    m_beforeTargets.removeOne(target);
}

QStringList Target::afterTargets() const
{
    return m_afterTargets;
}

void Target::setAfterTargets(const QStringList &afterTargets)
{
    m_afterTargets = afterTargets;
}

void Target::addAfterTarget(const QString &target)
{
    if (m_afterTargets.contains(target))
        return;
    m_afterTargets.append(target);
}

void Target::removeAfterTarget(const QString &target)
{
    m_afterTargets.removeOne(target);
}

QStringList Target::dependsOnTargets() const
{
    return m_dependsOnTargets;
}

void Target::setDependsOnTargets(const QStringList &dependsOnTargets)
{
    m_dependsOnTargets = dependsOnTargets;
}

void Target::addDependsOnTarget(const QString &target)
{
    if (m_dependsOnTargets.contains(target))
        return;
    m_dependsOnTargets.append(target);
}

void Target::removeDependsOnTarget(const QString &target)
{
    m_dependsOnTargets.removeOne(target);
}

QString Target::label() const
{
    return m_label;
}

void Target::setLabel(const QString &label)
{
    m_label = label;
}

Task *Target::task(int index) const
{
    if (0 <= index && index < m_tasks.size())
        return m_tasks[index];
    return 0;
}

int Target::taskCount() const
{
    return m_tasks.size();
}

void Target::addTask(Task *task)
{
    if (m_tasks.contains(task))
        return;
    m_tasks.append(task);
    m_nodeList.append(task);
}

void Target::removeTask(Task *task)
{
    m_tasks.removeOne(task);
    m_nodeList.removeOne(task);
}

PropertyGroup *Target::propertyGroup(int index) const
{
    if (0 <= index && index < m_propertyGroups.size())
        return m_propertyGroups[index];
    return 0;
}

int Target::propertyGroupCount() const
{
    return m_propertyGroups.size();
}

void Target::addPropertyGroup(PropertyGroup *propertyGroup)
{
    if (m_propertyGroups.contains(propertyGroup))
        return;
    m_propertyGroups.append(propertyGroup);
    m_nodeList.append(propertyGroup);
}

void Target::removePropertyGroup(PropertyGroup *propertyGroup)
{
    m_propertyGroups.removeOne(propertyGroup);
    m_nodeList.removeOne(propertyGroup);
}

ItemGroup *Target::itemGroup(int index) const
{
    if (0 <= index && index < m_itemGroups.size())
        return m_itemGroups[index];
    return 0;
}

int Target::itemGroupCount() const
{
    return m_itemGroups.size();
}

void Target::addItemGroup(ItemGroup *itemGroup)
{
    if (m_itemGroups.contains(itemGroup))
        return;
    m_itemGroups.append(itemGroup);
    m_nodeList.append(itemGroup);
}

void Target::removeItemGroup(ItemGroup *itemGroup)
{
    m_itemGroups.removeOne(itemGroup);
    m_nodeList.removeOne(itemGroup);
}

OnError *Target::onErrorElement(int index) const
{
    if (0 <= index && index < m_onErrorElements.size())
        return m_onErrorElements[index];
    return 0;
}

int Target::onErrorElementCount() const
{
    return m_onErrorElements.size();
}

void Target::addOnErrorElement(OnError *onError)
{
    if (m_onErrorElements.contains(onError))
        return;
    m_onErrorElements.append(onError);
    m_nodeList.append(onError);
}

void Target::removeOnErrorElement(OnError *onError)
{
    m_onErrorElements.removeOne(onError);
    m_nodeList.removeOne(onError);
}

void Target::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());
    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode Target::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(TARGET_ITEM));

    element.setAttribute(QLatin1String(NAME_ATTRIBUTE), m_name);

    if (!m_condition.isEmpty())
        element.setAttribute(QLatin1String(CONDITION_ATTRIBUTE), m_condition);

    if (!m_label.isEmpty())
        element.setAttribute(QLatin1String(LABEL_ATTRIBUTE), m_label);

    if (!m_keepDuplicateOutputs.isEmpty())
        element.setAttribute(QLatin1String(KEEP_DUPLICATE_OUTPUTS_ATTRIBUTE), m_keepDuplicateOutputs);

    if (!m_inputs.isEmpty()) {
        QString inputs = m_inputs.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(INPUTS_ATTRIBUTE), inputs);
    }

    if (!m_outputs.isEmpty()) {
        QString outputs = m_outputs.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(OUTPUTS_ATTRIBUTE), outputs);
    }

    if (!m_returns.isEmpty()) {
        QString returns = m_returns.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(RETURNS_ATTRIBUTE), returns);
    }

    if (!m_beforeTargets.isEmpty()) {
        QString beforeTargets = m_beforeTargets.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(BEFORE_TARGETS_ATTRIBUTE), beforeTargets);
    }

    if (!m_afterTargets.isEmpty()) {
        QString afterTargets = m_afterTargets.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(AFTER_TARGETS_ATTRIBUTE), afterTargets);
    }

    if (!m_dependsOnTargets.isEmpty()) {
        QString dependsTargets = m_dependsOnTargets.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(DEPENDS_ON_TARGETS_ATTRIBUTE), dependsTargets);
    }

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

void Target::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String("Name"))
                m_name = domElement.value();
            else if (domElement.name() == QLatin1String(CONDITION_ATTRIBUTE))
                m_condition = domElement.value();
            else if (domElement.name() == QLatin1String(INPUTS_ATTRIBUTE))
                m_inputs = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(OUTPUTS_ATTRIBUTE))
                m_outputs = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(RETURNS_ATTRIBUTE))
                m_returns = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(BEFORE_TARGETS_ATTRIBUTE))
                m_beforeTargets = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(AFTER_TARGETS_ATTRIBUTE))
                m_afterTargets = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(DEPENDS_ON_TARGETS_ATTRIBUTE))
                m_dependsOnTargets = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(LABEL_ATTRIBUTE))
                m_label = domElement.value();
            else if (domElement.name() == QLatin1String(KEEP_DUPLICATE_OUTPUTS_ATTRIBUTE))
                m_keepDuplicateOutputs = domElement.value();
        }
    }
}

void Target::processChildNodes(const QDomNode &node)
{
    if (node.nodeName() == QLatin1String(TASK_ITEM)) {
        Task *task = new Task;
        task->processNode(node);
        m_tasks.append(task);
        m_nodeList.append(task);
    }

    else if (node.nodeName() == QLatin1String(PROPERTY_GROUP_ITEM)) {
        PropertyGroup *propertyGroup = new PropertyGroup;
        propertyGroup->processNode(node);
        m_propertyGroups.append(propertyGroup);
        m_nodeList.append(propertyGroup);
    }

    else if (node.nodeName() == QLatin1String(ITEM_GROUP_ITEM)) {
        ItemGroup *itemGroup = new ItemGroup;
        itemGroup->processNode(node);
        m_itemGroups.append(itemGroup);
        m_nodeList.append(itemGroup);
    }

    else if (node.nodeName() == QLatin1String(ON_ERROR_ITEM)) {
        OnError *onError = new OnError;
        onError->processNode(node);
        m_onErrorElements.append(onError);
        m_nodeList.append(onError);
    }

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}
QString Target::keepDuplicateOutputs() const
{
    return m_keepDuplicateOutputs;
}

void Target::setKeepDuplicateOutputs(const QString &keepDuplicateOutputs)
{
    m_keepDuplicateOutputs = keepDuplicateOutputs;
}


} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
