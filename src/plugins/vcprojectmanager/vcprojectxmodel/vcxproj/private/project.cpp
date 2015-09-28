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

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

Project::Project()
    : m_chooseElement(0),
      m_projectExtensions(0)
{
}

Project::Project(const Project &project)
{
    m_defaultTargets = project.defaultTargets();
    m_initialTargets = project.initialTargets();
    m_toolsVersion = project.toolsVersion();
    m_xmlns = project.xmlns();
    m_chooseElement = new Choose(*project.chooseElement());
    m_projectExtensions = new ProjectExtensions(*project.projectExtensions());

    for (int i = 0; i < project.importElementCount(); ++i) {
        Import *import = project.importElement(i);

        if (import)
            addImport(new Import(*import));
    }

    for (int i = 0; i < project.importGroupCount(); ++i) {
        ImportGroup *import = project.importGroup(i);

        if (import)
            addImportGroup(new ImportGroup(*import));
    }

    for (int i = 0; i < project.itemGroupCount(); ++i) {
        ItemGroup *itemGrp = project.itemGroup(i);

        if (itemGrp)
            addItemGroup(new ItemGroup(*itemGrp));
    }

    for (int i = 0; i < project.propertyGroupCount(); ++i) {
        PropertyGroup *propGroup = project.propertyGroup(i);

        if (propGroup)
            addPropertyGroup(new PropertyGroup(*propGroup));
    }

    for (int i = 0; i < project.targetCount(); ++i) {
        Target *target = project.target(i);

        if (target)
            addTarget(new Target(*target));
    }

    for (int i = 0; i < project.usingTaskCount(); ++i) {
        UsingTask *usingTsk = project.usingTask(i);

        if (usingTsk)
            addUsingTask(new UsingTask(*usingTsk));
    }

    for (int i = 0; i < project.itemDefinitionGroupCount(); ++i) {
        ItemDefinitionGroup *itemDefGrp = project.itemDefinitionGroup(i);

        if (itemDefGrp)
            addItemDefinitionGroup(new ItemDefinitionGroup(*itemDefGrp));
    }
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
    if (0 <= index && index < m_importElements.size())
        return m_importElements[index];
    return 0;
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
    if (0 <= index && index < m_itemGroups.size())
        return m_itemGroups[index];
    return 0;
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

ImportGroup *Project::importGroup(int index) const
{
    if (0 <= index && index < m_importGroups.size())
        return m_importGroups[index];
    return 0;
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
    if (0 <= index && index < m_propertyGroups.size())
        return m_propertyGroups[index];
    return 0;
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

Target *Project::target(int index) const
{
    if (0 <= index && index < m_targets.size())
        return m_targets[index];
    return 0;
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
    if (0 <= index && index < m_usingTasks.size())
        return m_usingTasks[index];
    return 0;
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
    if (0 <= index && index < m_itemDefinitionGroups.size())
        return m_itemDefinitionGroups[index];
    return 0;
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
