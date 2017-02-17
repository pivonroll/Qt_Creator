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
#include "utilsx.h"

#include "conditionmanipulation.h"

#include "private/project.h"
#include "private/item.h"
#include "private/itemdefinition.h"
#include "private/itemmetadata.h"
#include "private/itemgroup.h"
#include "private/itemdefinitiongroup.h"
#include "private/propertygroup.h"
#include "private/property.h"
#include "vcprojx_constants.h"
#include "../condition_parser/expression/evaluatearguments.h"

#include <QMap>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ItemGroup *Utils::findItemGroupWhichContainsItemWithName(const QString &itemName, Project *docProject)
{
    if (docProject) {
        for (int i = 0; i < docProject->itemGroupCount(); ++i) {
            ItemGroup *itemGroup = docProject->itemGroup(i);

            if (itemGroup && itemGroupContainsItemWithName(itemName, itemGroup))
                return itemGroup;
        }
    }

    return nullptr;
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

    return nullptr;
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

bool Utils::itemGroupContainsItemWithName(const QString &itemName, ItemGroup *itemGroup)
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

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

