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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMGROUP_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMGROUP_H

#include <QList>
#include <QString>

#include "ivisualstudionodex.h"

class QDomElement;

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Item;

class ItemGroup : public IVisualStudioNodeX
{
public:
    ItemGroup();
    ItemGroup(const ItemGroup &other);
    ItemGroup(ItemGroup &&other);
    ItemGroup& operator=(ItemGroup other);
    ~ItemGroup();

    Item *item(int index) const;
    int itemCount() const;
    void addItem(Item *item);
    void removeItem(Item *item);
    Item *findItemWithInclude(const QString &include) const;
    Item *findItemWithName(const QString &name) const;

    QString condition() const;
    void setCondition(const QString &condition);

    // IVisualStudioProjectNode interface
    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    QString label() const;
    void setLabel(const QString &label);

private:
    static void swap(ItemGroup &first, ItemGroup &second);
    void processNodeAttributes(const QDomElement &nodeElement);
    void processChildNodes(const QDomNode &node);

    QList<Item *> m_items;
    QString m_condition;
    QString m_label;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMGROUP_H
