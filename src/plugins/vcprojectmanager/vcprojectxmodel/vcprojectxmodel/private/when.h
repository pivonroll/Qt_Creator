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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_WHEN_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_WHEN_H

#include <QString>
#include <QList>

#include "ivisualstudionodex.h"

class QDomElement;

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Choose;
class ItemGroup;
class PropertyGroup;

class When : public IVisualStudioNodeX
{
public:
    When();
    When(const When &other);
    When(When &&other);
    When& operator=(When other);

    ~When();

    QString condition() const;
    void setCondition(const QString &condition);

    Choose* chooseElement(int index) const;
    int chooseElementCount() const;
    void addChooseElement(Choose *choose);
    void removeChoose(Choose *choose);

    ItemGroup* itemGroup(int index) const;
    int itemGroupCount() const;
    void addItemGroup(ItemGroup *itemGroup);
    void removeItemGroup(ItemGroup *itemGroup);

    PropertyGroup* propertyGroup(int index) const;
    int propertyGroupCount() const;
    void addPropertyGroup(PropertyGroup *propertyGroup);
    void removePropertyGroup(PropertyGroup *propertyGroup);

    // IVisualStudioProjectNode interface
    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

private:
    static void swap(When &first, When &second);
    void processNodeAttributes(const QDomElement &nodeElement);
    void processChildNodes(const QDomNode &node);

    QString m_condition;
    QList<Choose *> m_chooseElements;
    QList<ItemGroup *> m_itemGroups;
    QList<PropertyGroup *> m_propertyGroups;

    QList<IVisualStudioNodeX *> m_nodeList;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_WHEN_H
