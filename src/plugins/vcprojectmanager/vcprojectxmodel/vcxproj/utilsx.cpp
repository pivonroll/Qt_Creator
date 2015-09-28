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
#include "utilsx.h"
#include "private/project.h"
#include "private/item.h"
#include "private/itemmetadata.h"
#include "private/itemgroup.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ItemGroup *Utils::findItemGroupWithName(const QString &itemName, Project *docProject)
{
    if (docProject) {
        for (int i = 0; i < docProject->itemGroupCount(); ++i) {
            ItemGroup *itemGroup = docProject->itemGroup(i);

            if (itemGroup && itemGroupContainsItemName(itemName, itemGroup))
                return itemGroup;
        }
    }

    return 0;
}

ItemGroup *Utils::findItemGroupContainingItem(Item *item, Project *docProject)
{
    if (item && docProject) {
        for (int i = 0; i < docProject->itemGroupCount(); ++i) {
            ItemGroup *itemGroup = docProject->itemGroup(i);

            if (itemGroup && groupContainsItem(item, itemGroup))
                return itemGroup;
        }
    }

    return 0;
}

bool Utils::groupContainsItem(Item *item, ItemGroup *itemGroup)
{
    if (item && itemGroup) {
        for (int i = 0; i < itemGroup->itemCount(); ++i) {
            Item *it = itemGroup->item(i);

            if (it && it->name() == item->name() && it->include() == item->include())
                return true;
        }
    }

    return false;
}

Item *Utils::findItemInGroup(const QString &incl, ItemGroup *itemGroup)
{
    if (itemGroup) {
        for (int i = 0; i < itemGroup->itemCount(); ++i) {
            Item *it = itemGroup->item(i);

            if (it && it->include() == incl)
                return it;
        }
    }

    return 0;
}

bool Utils::itemGroupContainsItemName(const QString &itemName, ItemGroup *itemGroup)
{
    if (itemGroup) {
        for (int i = 0; i < itemGroup->itemCount(); ++i) {
            Item *item = itemGroup->item(i);

            if (item && item->name() == itemName)
                return true;
        }
    }

    return false;
}

ItemMetaData *Utils::findItemMetaWithName(const QString &name, const Item *findInItem)
{
    if (!findInItem)
        return 0;

    for (int i = 0; i < findInItem->itemMetaDataCount(); ++i) {
        ItemMetaData *itemMeta = findInItem->itemMetaData(i);

        if (itemMeta && itemMeta->name() == name)
            return itemMeta;
    }

    return 0;
}

ItemGroup *Utils::findItemGroupWithLabel(const QString &label, Project *project)
{
    if (project) {
        for (int i = 0; i < project->itemGroupCount(); ++i) {
            ItemGroup *itemGroup = project->itemGroup(i);

            if (itemGroup && itemGroup->label() == label)
                return itemGroup;
        }
    }

    return 0;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

