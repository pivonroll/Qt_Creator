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
#include "integertoolattributesettingsitem.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>
#include <visualstudiotoolattributes/toolattributeoption.h>

#include <visualstudiointerfaces/itoolattribute.h>

#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class IntegerToolAttributeSettingsItem
 * Represents an integer attribute's settings item.
 * If the integer attribute is a multi-option attribute
 * it will consist of a combo box widget with added option
 * values that can be set to as an attribute's value.
 * If this is a plain integer number attribute (no options to choose from)
 * then it will consists of a spin box.
 */

IntegerToolAttributeSettingsItem::IntegerToolAttributeSettingsItem(IToolAttribute *toolAttribute)
    : m_comboBox(0),
      m_spinBox(0),
      m_toolAttribute(toolAttribute)
{

    ToolAttributeOption *option = m_toolAttribute->descriptionDataItem()->firstOption();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);

    if (option) {
        m_comboBox = new QComboBox;
        while (option) {
            m_comboBox->addItem(option->description(), option->value());
            if (option->value() == m_toolAttribute->value())
                m_comboBox->setCurrentIndex(m_comboBox->count() - 1);
            option = option->nextOption();
        }
        layout->addWidget(m_comboBox);
    }
    else {
        m_spinBox = new QSpinBox;
        m_spinBox->setValue(m_toolAttribute->value().toInt());
        layout->addWidget(m_spinBox);
    }

    setLayout(layout);
}

IntegerToolAttributeSettingsItem::~IntegerToolAttributeSettingsItem()
{
    if (m_comboBox)
        m_comboBox->deleteLater();
    else
        m_spinBox->deleteLater();
}

/*!
 * \reimp
 */
void IntegerToolAttributeSettingsItem::saveData()
{
    if (m_comboBox)
        m_toolAttribute->setValue(m_comboBox->itemData(m_comboBox->currentIndex()).toString().trimmed());
    else if (m_spinBox)
        m_toolAttribute->setValue(QVariant(m_spinBox->value()).toString());
}

} // namespace Internal
} // namespace VcProjectManager
