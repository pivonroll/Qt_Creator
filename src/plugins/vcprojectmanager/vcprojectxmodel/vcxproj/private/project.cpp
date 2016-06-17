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
#include "project.h"

#include "import.h"
#include "importgroup.h"
#include "itemgroup.h"
#include "propertygroup.h"
#include "target.h"
#include "usingtask.h"
#include "projectextensions.h"
#include "choose.h"
#include "itemdefinitiongroup.h"
#include "../vcprojx_constants.h"

#include "../conditionmanipulation.h"

#include <utils/qtcassert.h>
#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Project::Project()
    : m_chooseElement(nullptr),
      m_projectExtensions(nullptr)
{
}

Project::Project(const Project &other)
{
    m_defaultTargets = other.m_defaultTargets;
    m_initialTargets = other.m_initialTargets;
    m_toolsVersion = other.m_toolsVersion;
    m_xmlns = other.m_xmlns;
    m_chooseElement = new Choose(*other.m_chooseElement);
    m_projectExtensions = new ProjectExtensions(*other.m_projectExtensions);

    foreach (Import *import, other.m_importElements)
        addImport(new Import(*import));

    foreach (ImportGroup *import, other.m_importGroups)
        addImportGroup(new ImportGroup(*import));

    foreach (ItemGroup *itemGrp, other.m_itemGroups)
        addItemGroup(new ItemGroup(*itemGrp));

    foreach (PropertyGroup *propGroup, other.m_propertyGroups)
        addPropertyGroup(new PropertyGroup(*propGroup));

    foreach (Target *target, other.m_targets)
        addTarget(new Target(*target));

    foreach (UsingTask *usingTsk, other.m_usingTasks)
        addUsingTask(new UsingTask(*usingTsk));

    foreach (ItemDefinitionGroup *itemDefGrp, other.m_itemDefinitionGroups)
        addItemDefinitionGroup(new ItemDefinitionGroup(*itemDefGrp));
}

Project::Project(Project &&other)
    : Project()
{
    swap(*this, other);
}

Project &Project::operator=(Project other)
{
    swap(*this, other);
    return *this;
}

Project::~Project()
{
    qDeleteAll(m_importElements);
    qDeleteAll(m_itemGroups);
    qDeleteAll(m_propertyGroups);
    qDeleteAll(m_targets);
    qDeleteAll(m_usingTasks);
    qDeleteAll(m_itemDefinitionGroups);
    delete m_projectExtensions;
    delete m_chooseElement;
}

QStringList Project::defaultTargets() const
{
    return m_defaultTargets;
}

void Project::setDefaultTargets(const QStringList &defaultTargets)
{
    m_defaultTargets = defaultTargets;
}

void Project::addDefaultTarget(const QString &target)
{
    if (m_defaultTargets.contains(target))
        return;
    m_defaultTargets.append(target);
}

void Project::removeDefaultTarget(const QString &target)
{
    m_defaultTargets.removeOne(target);
}

QStringList Project::initialTargets() const
{
    return m_initialTargets;
}

void Project::setInitialTargets(const QStringList &initialTargets)
{
    m_initialTargets = initialTargets;
}

void Project::addInitialTargets(const QString &target)
{
    if (m_initialTargets.contains(target))
        return;
    m_initialTargets.append(target);
}

void Project::removeInitialTarget(const QString &target)
{
    m_initialTargets.removeOne(target);
}

QString Project::toolsVersion() const
{
    return m_toolsVersion;
}

void Project::setToolsVersion(const QString &toolsVersion)
{
    m_toolsVersion = toolsVersion;
}

QString Project::xmlns() const
{
    return m_xmlns;
}

void Project::setXmlns(const QString &xmlns)
{
    m_xmlns = xmlns;
}

Choose *Project::chooseElement() const
{
    return m_chooseElement;
}

void Project::setChooseElement(Choose *chooseElement)
{
    m_chooseElement = chooseElement;
}

Import *Project::importElement(int index) const
{
    QTC_ASSERT(0 <= index && index < m_importElements.size(), return nullptr);
    return m_importElements[index];
}

int Project::importElementCount() const
{
    return m_importElements.size();
}

void Project::addImport(Import *import)
{
    if (m_importElements.contains(import))
        return;
    m_importElements.append(import);
    m_nodeList.append(import);
}

void Project::removeImport(Import *import)
{
    m_importElements.removeOne(import);
    m_nodeList.removeOne(import);
}

ItemGroup *Project::itemGroup(int index) const
{
    QTC_ASSERT(0 <= index && index < m_itemGroups.size(), return nullptr);
    return m_itemGroups[index];
}

int Project::itemGroupCount() const
{
    return m_itemGroups.size();
}

void Project::addItemGroup(ItemGroup *itemGroup)
{
    if (m_itemGroups.contains(itemGroup))
        return;
    m_itemGroups.append(itemGroup);
    m_nodeList.append(itemGroup);
}

void Project::removeItemGroup(ItemGroup *itemGroup)
{
    m_itemGroups.removeOne(itemGroup);
    m_nodeList.removeOne(itemGroup);
}

ItemGroup *Project::findItemGroupWithLabel(const QString &label) const
{
    foreach (ItemGroup *item, m_itemGroups) {
        if (item && item->label() == label)
            return item;
    }

    return nullptr;
}

ItemGroup *Project::findItemGroupWithName(const QString &label) const
{
    foreach (ItemGroup *item, m_itemGroups) {
        if (item && item->label() == label)
            return item;
    }

    return nullptr;
}

ImportGroup *Project::importGroup(int index) const
{
    QTC_ASSERT(0 <= index && index < m_importGroups.size(), return nullptr);
    return m_importGroups[index];
}

int Project::importGroupCount() const
{
    return m_importGroups.size();
}

void Project::addImportGroup(ImportGroup *importGroup)
{
    if (m_importGroups.contains(importGroup))
        return;
    m_importGroups.append(importGroup);
    m_nodeList.append(importGroup);
}

void Project::removeImportGroup(ImportGroup *importGroup)
{
    m_importGroups.removeOne(importGroup);
    m_nodeList.removeOne(importGroup);
}

ImportGroup *Project::findImportGroupWithConditionAndLabel(const EvaluateArguments &evalArgs, const QString &label) const
{
    foreach (ImportGroup *importGroup, m_importGroups) {
        if (!importGroup || importGroup->label() != label)
            continue;

        ConditionManipulation condMan(importGroup->condition());

        if (condMan.evaluate(evalArgs))
            return importGroup;
    }

    return nullptr;
}

ProjectExtensions *Project::projectExtensions() const
{
    return m_projectExtensions;
}

void Project::setProjectExtensions(ProjectExtensions *projectExtensions)
{
    m_nodeList.removeOne(m_projectExtensions);
    m_projectExtensions = projectExtensions;
    m_nodeList.append(m_projectExtensions);
}

PropertyGroup *Project::propertyGroup(int index) const
{
    QTC_ASSERT(0 <= index && index < m_propertyGroups.size(), return nullptr);
    return m_propertyGroups[index];
}

int Project::propertyGroupCount() const
{
    return m_propertyGroups.size();
}

void Project::addPropertyGroup(PropertyGroup *propertyGroup)
{
    if (m_propertyGroups.contains(propertyGroup))
        return;
    m_propertyGroups.append(propertyGroup);
    m_nodeList.append(propertyGroup);
}

void Project::removePropertyGroup(PropertyGroup *propertyGroup)
{
    m_propertyGroups.removeOne(propertyGroup);
    m_nodeList.removeOne(propertyGroup);
}

PropertyGroup *Project::findPropertyGroupWithConditionAndLabel(const EvaluateArguments &evalArgs, const QString &label) const
{
    foreach (PropertyGroup *propertyGroup, m_propertyGroups) {
        if (!propertyGroup || propertyGroup->label() != label)
            continue;

        ConditionManipulation condMan(propertyGroup->condition());

        if (condMan.evaluate(evalArgs))
            return propertyGroup;
    }

    return nullptr;
}

Target *Project::target(int index) const
{
    QTC_ASSERT(0 <= index && index < m_targets.size(), return nullptr);
    return m_targets[index];
}

int Project::targetCount() const
{
    return m_targets.size();
}

void Project::addTarget(Target *target)
{
    if (m_targets.contains(target))
        return;
    m_targets.append(target);
    m_nodeList.append(target);
}

void Project::removeTarget(Target *target)
{
    m_targets.removeOne(target);
    m_nodeList.removeOne(target);
}

UsingTask *Project::usingTask(int index) const
{
    QTC_ASSERT(0 <= index && index < m_usingTasks.size(), return nullptr);
    return m_usingTasks[index];
}

int Project::usingTaskCount() const
{
    return m_usingTasks.size();
}

void Project::addUsingTask(UsingTask *usingTask)
{
    if (m_usingTasks.contains(usingTask))
        return;
    m_usingTasks.append(usingTask);
    m_nodeList.append(usingTask);
}

void Project::removeUsingTask(UsingTask *usingTask)
{
    m_usingTasks.removeOne(usingTask);
    m_nodeList.removeOne(usingTask);
}

ItemDefinitionGroup *Project::itemDefinitionGroup(int index) const
{
    QTC_ASSERT(0 <= index && index < m_itemDefinitionGroups.size(), return nullptr);
    return m_itemDefinitionGroups[index];
}

int Project::itemDefinitionGroupCount() const
{
    return m_itemDefinitionGroups.size();
}

void Project::addItemDefinitionGroup(ItemDefinitionGroup *itemDefGrp)
{
    if (m_itemDefinitionGroups.contains(itemDefGrp))
        return;
    m_itemDefinitionGroups.append(itemDefGrp);
    m_nodeList.append(itemDefGrp);
}

void Project::removeItemDefinitionGroup(ItemDefinitionGroup *ItemDefGrp)
{
    m_itemDefinitionGroups.removeOne(ItemDefGrp);
    m_nodeList.removeOne(ItemDefGrp);
}

ItemDefinitionGroup *Project::findItemDefinitionGroupWithCondition(const EvaluateArguments &evalArgs) const
{
    foreach (ItemDefinitionGroup *itemDefinitionGroup, m_itemDefinitionGroups) {
        if (!itemDefinitionGroup)
            continue;

        ConditionManipulation condMan(itemDefinitionGroup->condition());

        if (condMan.evaluate(evalArgs))
            return itemDefinitionGroup;
    }

    return nullptr;
}

void Project::processNode(const QDomNode &node)
{
    if (node.isElement())
        processNodeAttributes(node.toElement());

    if (node.hasChildNodes())
        processChildNodes(node.firstChild());
}

QDomNode Project::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement element = domXMLDocument.createElement(QLatin1String(PROJECT_ATTRIBUTE));

    if (!m_defaultTargets.isEmpty()) {
        QString defTargets = m_defaultTargets.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(DEFAULT_TARGETS_ATTRIBUTE), defTargets);
    }

    if (!m_initialTargets.isEmpty()) {
        QString initTargets = m_initialTargets.join(QLatin1String(";"));
        element.setAttribute(QLatin1String(INITIAL_TARGETS_ATTRIBUTE), initTargets);
    }

    if (!m_toolsVersion.isEmpty())
        element.setAttribute(QLatin1String(TOOLS_VERSION_ATTRIBUTE), m_toolsVersion);

    if (!m_xmlns.isEmpty())
        element.setAttribute(QLatin1String(XMLNS_ATTRIBUTE), m_xmlns);

    QListIterator<IVisualStudioNodeX *> it(m_nodeList);
    while (it.hasNext()) {
        IVisualStudioNodeX *visualStudioNode = it.next();

        if (visualStudioNode) {
            QDomNode node = visualStudioNode->toXMLDomNode(domXMLDocument);
            element.appendChild(node);
        }
    }

    domXMLDocument.appendChild(element);
    return element;
}

QString Project::version() const
{
    if (m_toolsVersion == QLatin1String("10.0"))
        return QLatin1String("2010");
    else if (m_toolsVersion == QLatin1String("11.0"))
        return QLatin1String("2012");
    else if (m_toolsVersion == QLatin1String("12.0"))
        return QLatin1String("2013");
    else if (m_toolsVersion == QLatin1String("13.0"))
        return QLatin1String("2015");

    return QString();
}

void Project::swap(Project &first, Project &second)
{
    std::swap(first.m_chooseElement, second.m_chooseElement);
    std::swap(first.m_defaultTargets, second.m_defaultTargets);
    std::swap(first.m_importElements, second.m_importElements);
    std::swap(first.m_importGroups, second.m_importGroups);
    std::swap(first.m_initialTargets, second.m_initialTargets);
    std::swap(first.m_itemDefinitionGroups, second.m_itemDefinitionGroups);
    std::swap(first.m_itemGroups, second.m_itemGroups);
    std::swap(first.m_nodeList, second.m_nodeList);
    std::swap(first.m_projectExtensions, second.m_projectExtensions);
    std::swap(first.m_propertyGroups, second.m_propertyGroups);
    std::swap(first.m_targets, second.m_targets);
    std::swap(first.m_toolsVersion, second.m_toolsVersion);
    std::swap(first.m_usingTasks, second.m_usingTasks);
    std::swap(first.m_xmlns, second.m_xmlns);
}

void Project::processChildNodes(const QDomNode &node)
{
    if (node.nodeName() == QLatin1String(IMPORT_ITEM)) {
        Import *import = new Import;
        import->processNode(node);
        m_importElements.append(import);
        m_nodeList.append(import);
    }

    else if (node.nodeName() == QLatin1String(ITEM_GROUP_ITEM)) {
        ItemGroup *itemGroup = new ItemGroup;
        itemGroup->processNode(node);
        m_itemGroups.append(itemGroup);
        m_nodeList.append(itemGroup);
    }

    else if (node.nodeName() == QLatin1String(IMPORT_GROUP_ITEM)) {
        ImportGroup *importGroup = new ImportGroup;
        importGroup->processNode(node);
        m_importGroups.append(importGroup);
        m_nodeList.append(importGroup);
    }

    else if (node.nodeName() == QLatin1String(PROPERTY_GROUP_ITEM)) {
        PropertyGroup *propertyGroup = new PropertyGroup;
        propertyGroup->processNode(node);
        m_propertyGroups.append(propertyGroup);
        m_nodeList.append(propertyGroup);
    }

    else if (node.nodeName() == QLatin1String(TARGET_ITEM)) {
        Target *target = new Target;
        target->processNode(node);
        m_targets.append(target);
        m_nodeList.append(target);
    }

    else if (node.nodeName() == QLatin1String(USING_TASK_ITEM)) {
        UsingTask *usingTask = new UsingTask;
        usingTask->processNode(node);
        m_usingTasks.append(usingTask);
        m_nodeList.append(usingTask);
    }

    else if (node.nodeName() == QLatin1String(PROJECT_EXTENSIONS_ITEM)) {
        m_projectExtensions = new ProjectExtensions;
        m_projectExtensions->processNode(node);
        m_nodeList.append(m_projectExtensions);
    }

    else if (node.nodeName() == QLatin1String(CHOOSE_ITEM)) {
        m_chooseElement = new Choose;
        m_chooseElement->processNode(node);
        m_nodeList.append(m_chooseElement);
    }

    else if (node.nodeName() == QLatin1String(ITEM_DEFINITION_GROUP_ITEM)) {
        ItemDefinitionGroup *itemGroup  =new ItemDefinitionGroup;
        itemGroup->processNode(node);
        m_itemDefinitionGroups.append(itemGroup);
        m_nodeList.append(itemGroup);
    }

    QDomNode nextSibling = node.nextSibling();
    if (!nextSibling.isNull())
        processChildNodes(nextSibling);
}

void Project::processNodeAttributes(const QDomElement &nodeElement)
{
    // process attributes
    QDomNamedNodeMap namedNodeMap = nodeElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String(DEFAULT_TARGETS_ATTRIBUTE))
                m_defaultTargets = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(INITIAL_TARGETS_ATTRIBUTE))
                m_initialTargets = domElement.value().split(QLatin1Char(';'));
            else if (domElement.name() == QLatin1String(TOOLS_VERSION_ATTRIBUTE))
                m_toolsVersion = domElement.value();
            else if (domElement.name() == QLatin1String(XMLNS_ATTRIBUTE))
                m_xmlns = domElement.value();
        }
    }
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
