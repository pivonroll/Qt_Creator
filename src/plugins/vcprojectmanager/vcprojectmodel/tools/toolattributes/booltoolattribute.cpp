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
#include "booltoolattribute.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include <visualstudiowidgets/toolwidgets/booltoolattributesettingsitem.h>

namespace VcProjectManager {
namespace Internal {

BoolToolAttribute::BoolToolAttribute(const AttributeDescriptionDataItem *descDataItem)
    : m_descDataItem(descDataItem),
      m_isUsed(false)
{
}

BoolToolAttribute::BoolToolAttribute(const BoolToolAttribute &other)
{
    m_isUsed = other.m_isUsed;
    m_descDataItem = other.m_descDataItem;
    m_attributeValue = other.m_attributeValue;
}

BoolToolAttribute::BoolToolAttribute(BoolToolAttribute &&other)
    : BoolToolAttribute()
{
    swap(*this, other);
}

BoolToolAttribute &BoolToolAttribute::operator=(BoolToolAttribute other)
{
    swap(*this, other);
    return *this;
}

const AttributeDescriptionDataItem *BoolToolAttribute::descriptionDataItem() const
{
    return m_descDataItem;
}

IToolAttributeSettingsWidget *BoolToolAttribute::createSettingsItem()
{
    return new BoolToolAttributeSettingsItem(this);
}

QString BoolToolAttribute::value() const
{
    if (m_isUsed)
        return m_attributeValue;
    return m_descDataItem->defaultValue();
}

void BoolToolAttribute::setValue(const QString &value)
{
    if (!m_isUsed && value == m_descDataItem->defaultValue())
        return;

    m_attributeValue = value;
    m_isUsed = true;
}

bool BoolToolAttribute::isUsed() const
{
    return m_isUsed;
}

IToolAttribute *BoolToolAttribute::clone() const
{
    return new BoolToolAttribute(*this);
}

void BoolToolAttribute::swap(BoolToolAttribute &first, BoolToolAttribute &second)
{
    std::swap(first.m_attributeValue, second.m_attributeValue);
    std::swap(first.m_descDataItem, second.m_descDataItem);
    std::swap(first.m_isUsed, second.m_isUsed);
}

} // namespace Internal
} // namespace VcProjectManager
