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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECT_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECT_H

#include <QStringList>

#include "ivisualstudionodex.h"
#include "../../condition_parser/expression/evaluatearguments.h"

class QDomElement;

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Choose;
class Import;
class ItemGroup;
class ProjectExtensions;
class PropertyGroup;
class Target;
class UsingTask;
class ItemDefinitionGroup;
class ImportGroup;

class Project : public IVisualStudioNodeX
{
    friend class VisualStudioProjectXDoc;
public:
    Project();
    Project(const Project &other);
    Project(Project &&other);
    Project& operator=(Project other);
    ~Project();

    QStringList defaultTargets() const;
    void setDefaultTargets(const QStringList &defaultTargets);
    void addDefaultTarget(const QString &target);
    void removeDefaultTarget(const QString &target);

    QStringList initialTargets() const;
    void setInitialTargets(const QStringList &initialTargets);
    void addInitialTargets(const QString &target);
    void removeInitialTarget(const QString &target);

    QString toolsVersion() const;
    void setToolsVersion(const QString &toolsVersion);

    QString xmlns() const;
    void setXmlns(const QString &xmlns);

    Choose *chooseElement() const;
    void setChooseElement(Choose *chooseElement);

    Import *importElement(int index) const;
    int importElementCount() const;
    void addImport(Import *import);
    void removeImport(Import *import);

    ItemGroup *itemGroup(int index) const;
    int itemGroupCount() const;
    void addItemGroup(ItemGroup *itemGroup);
    void removeItemGroup(ItemGroup *itemGroup);
    ItemGroup *findItemGroupWithLabel(const QString &label) const;
    ItemGroup *findItemGroupWithName(const QString &label) const;

    ImportGroup *importGroup(int index) const;
    int importGroupCount() const;
    void addImportGroup(ImportGroup *importGroup);
    void removeImportGroup(ImportGroup *importGroup);
    ImportGroup *findImportGroupWithConditionAndLabel(const EvaluateArguments &evalArgs, const QString &label = QString()) const;

    ProjectExtensions *projectExtensions() const;
    void setProjectExtensions(ProjectExtensions *projectExtensions);

    PropertyGroup *propertyGroup(int index) const;
    int propertyGroupCount() const;
    void addPropertyGroup(PropertyGroup *propertyGroup);
    void removePropertyGroup(PropertyGroup *propertyGroup);
    PropertyGroup *findPropertyGroupWithConditionAndLabel(const EvaluateArguments &evalArgs, const QString &label = QString()) const;

    Target *target(int index) const;
    int targetCount() const;
    void addTarget(Target *target);
    void removeTarget(Target *target);

    UsingTask *usingTask(int index) const;
    int usingTaskCount() const;
    void addUsingTask(UsingTask *usingTask);
    void removeUsingTask(UsingTask *usingTask);

    ItemDefinitionGroup *itemDefinitionGroup(int index) const;
    int itemDefinitionGroupCount() const;
    void addItemDefinitionGroup(ItemDefinitionGroup *itemDefGrp);
    void removeItemDefinitionGroup(ItemDefinitionGroup *ItemDefGrp);
    ItemDefinitionGroup *findItemDefinitionGroupWithCondition(const EvaluateArguments &evalArgs) const;

    // IVisualStudioProjectNode interface
    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    QString version() const;

private:
    static void swap(Project &first, Project &second);
    void processChildNodes(const QDomNode &node);
    void processNodeAttributes(const QDomElement &nodeElement);

    QStringList m_defaultTargets;
    QStringList m_initialTargets;
    QString m_toolsVersion;
    QString m_xmlns; // required
    Choose *m_chooseElement;
    ProjectExtensions *m_projectExtensions;
    QList<Import *> m_importElements;
    QList<ImportGroup *> m_importGroups;
    QList<ItemGroup *> m_itemGroups;
    QList<PropertyGroup *> m_propertyGroups;
    QList<Target *> m_targets;
    QList<UsingTask *> m_usingTasks;
    QList<ItemDefinitionGroup *> m_itemDefinitionGroups;
    QList<IVisualStudioNodeX *> m_nodeList; // holds node order

    // IVisualStudioProjectNode interface
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECT_H
