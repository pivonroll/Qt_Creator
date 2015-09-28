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
#include "newconfigitemdialog.h"
#include "ui_newconfigitemdialog.h"

namespace VcProjectManager {
namespace Internal {

/*!
 * \class NewConfigItemDialog
 * This class represents a dialog that is used when new configuration is created.
 * User can choose a name for a new configuration and from which configuration to
 * copy settings from.
 */
NewConfigItemDialog::NewConfigItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConfigItemDialog)
{
    ui->setupUi(this);
}

NewConfigItemDialog::~NewConfigItemDialog()
{
    delete ui;
}

/*!
 * \return a name of a new configuration.
 */
QString NewConfigItemDialog::name() const
{
    return ui->m_newItemNameLineEdit->text();
}

/*!
 * \return a name of a configuration from which to copy settings from.
 */
QString NewConfigItemDialog::copyFrom() const
{
    return ui->m_copyFromComboBox->currentText();
}

/*!
 * \brief Adds a configuration to a list of configurations from which to copy settings from.
 */
void NewConfigItemDialog::addConfigItem(const QString &item)
{
    ui->m_copyFromComboBox->addItem(item);
}

} // namespace Internal
} // namespace VcProjectManager
