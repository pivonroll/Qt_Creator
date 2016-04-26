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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TARGET_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TARGET_H

#include <QStringList>
#include <QList>

#include "ivisualstudionodex.h"

class QDomElement;

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Task;
class PropertyGroup;
class ItemGroup;
class OnError;

class Target : public IVisualStudioNodeX
{
public:
    Target();
    Target(const Target &other);
    Target(Target &&other);
    Target& operator=(Target other);
    ~Target();

    QString name() const;
    void setName(const QString &name);

    QString condition() const;
    void setCondition(const QString &condition);

    QStringList inputs() const;
    void setInputs(const QStringList &inputs);
    void addInput(const QString &input);
    void removeInput(const QString &input);

    QStringList outputs() const;
    void setOutputs(const QStringList &outputs);
    void addOutput(const QString &output);
    void removeOutput(const QString &output);

    QStringList returns() const;
    void setReturns(const QStringList &returns);
    void addReturn(const QString &ret);
    void removeReturn(const QString &ret);

    QStringList beforeTargets() const;
    void setBeforeTargets(const QStringList &beforeTargets);
    void addBeforeTarget(const QString &target);
    void removeBeforeTarget(const QString &target);

    QStringList afterTargets() const;
    void setAfterTargets(const QStringList &afterTargets);
    void addAfterTarget(const QString &target);
    void removeAfterTarget(const QString &target);

    QStringList dependsOnTargets() const;
    void setDependsOnTargets(const QStringList &dependsOnTargets);
    void addDependsOnTarget(const QString &target);
    void removeDependsOnTarget(const QString &target);

    QString label() const;
    void setLabel(const QString &label);

    Task* task(int index) const;
    int taskCount() const;
    void addTask(Task *task);
    void removeTask(Task *task);

    PropertyGroup* propertyGroup(int index) const;
    int propertyGroupCount() const;
    void addPropertyGroup(PropertyGroup *propertyGroup);
    void removePropertyGroup(PropertyGroup *propertyGroup);

    ItemGroup* itemGroup(int index) const;
    int itemGroupCount() const;
    void addItemGroup(ItemGroup *itemGroup);
    void removeItemGroup(ItemGroup *itemGroup);

    OnError* onErrorElement(int index) const;
    int onErrorElementCount() const;
    void addOnErrorElement(OnError *onError);
    void removeOnErrorElement(OnError *onError);

    // IVisualStudioProjectNode interface
    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    QString keepDuplicateOutputs() const;
    void setKeepDuplicateOutputs(const QString &keepDuplicateOutputs);

private:
    static void swap(Target &first, Target &second);
    void processNodeAttributes(const QDomElement &nodeElement);
    void processChildNodes(const QDomNode &node);

    QString m_name; // required
    QString m_condition;
    QStringList m_inputs;
    QStringList m_outputs;
    QStringList m_returns;
    QString m_keepDuplicateOutputs;
    QStringList m_beforeTargets;
    QStringList m_afterTargets;
    QStringList m_dependsOnTargets;
    QString m_label;
    QList<Task *> m_tasks;
    QList<PropertyGroup *> m_propertyGroups;
    QList<ItemGroup *> m_itemGroups;
    QList<OnError *> m_onErrorElements;

    QList<IVisualStudioNodeX *> m_nodeList;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_TARGET_H
