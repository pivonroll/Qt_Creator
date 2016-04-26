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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMDEFINITION_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMDEFINITION_H

#include <QString>
#include <QList>

#include "ivisualstudionodex.h"

class QDomElement;

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class ItemMetaData;

class ItemDefinition : public IVisualStudioNodeX
{
public:
    ItemDefinition();
    ItemDefinition(const ItemDefinition &other);
    ItemDefinition(ItemDefinition &&other);
    ItemDefinition& operator=(ItemDefinition other);
    ~ItemDefinition();

    ItemMetaData *metaData(int index) const;
    ItemMetaData *findItemMetaData(const QString &name) const;
    int metaDataCount() const;
    void addMetaData(ItemMetaData *metaData);
    void removeMetaData(ItemMetaData *metaData);

    QString name() const;
    void setName(const QString &name);

    // IVisualStudioProjectNode interface
    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

private:
    static void swap(ItemDefinition &first, ItemDefinition &second);
    void processNodeAttributes(const QDomElement &domElement);
    void processChildNodes(const QDomNode &node);

    QList<ItemMetaData *> m_metaData;
    QString m_name;

};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_ITEMDEFINITION_H
