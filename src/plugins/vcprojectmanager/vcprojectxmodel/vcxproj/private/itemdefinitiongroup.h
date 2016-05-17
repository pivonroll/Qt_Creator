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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMDEFINITIONGROUP_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMDEFINITIONGROUP_H

#include <QList>
#include <QString>

#include "ivisualstudionodex.h"

class QDomElement;

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class ItemDefinition;

class ItemDefinitionGroup : public IVisualStudioNodeX
{
public:
    ItemDefinitionGroup();
    ItemDefinitionGroup(const ItemDefinitionGroup &other);
    ItemDefinitionGroup(ItemDefinitionGroup &&other);
    ItemDefinitionGroup& operator=(ItemDefinitionGroup other);
    ~ItemDefinitionGroup();

    ItemDefinition *itemDefinition(int index) const;
    int itemDefinitionCount() const;
    void addItem(ItemDefinition *item);
    void removeItem(ItemDefinition *item);
    ItemDefinition *findItemDefinition(const QString &name) const;

    QString condition() const;
    void setCondition(const QString &condition);

    // IVisualStudioProjectNode interface
    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

private:
    static void swap(ItemDefinitionGroup &first, ItemDefinitionGroup &second);
    void processNodeAttributes(const QDomElement &nodeElement);
    void processChildNodes(const QDomNode &node);

    QList<ItemDefinition *> m_items;
    QString m_condition;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMDEFINITIONGROUP_H
