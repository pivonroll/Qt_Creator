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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEM_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEM_H

#include <QList>
#include <QString>

#include "ivisualstudionodex.h"

class QDomElement;

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class ItemMetaData;

class Item : public IVisualStudioNodeX
{
public:
    Item();
    Item(const QString &include);
    Item(const Item &other);
    Item(Item &&other);
    Item& operator=(Item other);
    virtual ~Item();

    ItemMetaData *itemMetaData(int index) const;
    int itemMetaDataCount() const;
    void addItemMetaData(ItemMetaData *metaData);
    void removeMetaData(ItemMetaData *metaData);
    ItemMetaData *findMetaDataWithName(const QString &name) const;

    QString include() const;
    void setInclude(const QString &include);

    QString exclude() const;
    void setExclude(const QString &exclude);

    QString remove() const;
    void setRemove(const QString &remove);

    QString condition() const;
    void setCondition(const QString &condition);

    QString name() const;
    void setName(const QString &name);

    // IVisualStudioProjectNode interface
    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

private:
    static void swap(Item &first, Item &second);
    void processAttributes(const QDomElement &nodeElement);
    void processChildNodes(const QDomNode &node);

    QList<ItemMetaData *> m_itemMetaData;
    QString m_include; // required
    QString m_exclude;
    QString m_remove;
    QString m_condition;
    QString m_name;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEM_H
