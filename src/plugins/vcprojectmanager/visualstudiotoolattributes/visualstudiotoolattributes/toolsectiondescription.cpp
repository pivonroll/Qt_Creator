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
#include "toolsectiondescription.h"

#include <vcprojectmodel/tools/toolsection.h>
#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

namespace VcProjectManager {
namespace Internal {

ToolSectionDescription::ToolSectionDescription()
{
}

QString ToolSectionDescription::displayName() const
{
    return m_name;
}

void ToolSectionDescription::setDisplayName(const QString &name)
{
    m_name = name;
}

AttributeDescriptionDataItem *ToolSectionDescription::attributeDescription(const QString &attributeKey, QString version) const
{
    foreach (AttributeDescriptionDataItem *descDataItem, m_toolAttributeDescs) {
        if (descDataItem->key() == attributeKey)
            return descDataItem;
    }
    return nullptr;
}

AttributeDescriptionDataItem *ToolSectionDescription::attributeDescription(int index) const
{
    if (0 <= index && index <= m_toolAttributeDescs.size())
        return m_toolAttributeDescs[index];
    return nullptr;
}

int ToolSectionDescription::attributeDescriptionCount() const
{
    return m_toolAttributeDescs.size();
}

void ToolSectionDescription::addAttributeDescription(AttributeDescriptionDataItem *attributeDesc)
{
    if (!attributeDesc || m_toolAttributeDescs.contains(attributeDesc))
        return;

    foreach (AttributeDescriptionDataItem *descDataItem, m_toolAttributeDescs) {
        if (descDataItem->key() == attributeDesc->key())
            return;
    }
    m_toolAttributeDescs.append(attributeDesc);
}

void ToolSectionDescription::removeAttributeDescription(AttributeDescriptionDataItem *attributeDesc)
{
    if (!attributeDesc)
        return;

    foreach (AttributeDescriptionDataItem *descDataItem, m_toolAttributeDescs) {
        if (descDataItem->key() == attributeDesc->key()) {
            m_toolAttributeDescs.removeOne(descDataItem);
            return;
        }
    }
}

void ToolSectionDescription::removeAttributeDescription(const QString &attributeKey)
{
    foreach (AttributeDescriptionDataItem *descDataItem, m_toolAttributeDescs) {
        if (descDataItem->key() == attributeKey) {
            m_toolAttributeDescs.removeOne(descDataItem);
            return;
        }
    }
}

QString ToolSectionDescription::version() const
{
    return m_version;
}

void ToolSectionDescription::setVersion(const QString &version)
{
    m_version = version;
}

} // namespace Internal
} // namespace VcProjectManager
