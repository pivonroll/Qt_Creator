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
#include "ui_vcenternamewidget.h"
#include "vcenternamedialog.h"

#include <QMessageBox>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class VcEnterNameDialog
 * A dialog that enables a user to enter a name in the line edit field.
 * If the field is empty it will display an error message.
 */
VcEnterNameDialog::VcEnterNameDialog(const QString &containerType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VcEnterNameWidget)
{
    ui->setupUi(this);
    ui->m_containerTypeLabel->setText(containerType);
    connect(ui->m_buttonBox, &QDialogButtonBox::accepted, this, &VcEnterNameDialog::okButtonClicked);
}

VcEnterNameDialog::~VcEnterNameDialog()
{
    delete ui;
}

/*!
 * \return a name entered in the dialog's edit line.
 */
QString VcEnterNameDialog::containerName() const
{
    return ui->m_containerName->text();
}

/*!
 * \brief Sets a string \a name in the edit field in the dialog.
 */
void VcEnterNameDialog::setContainerName(const QString &name)
{
    ui->m_containerName->setText(name);
}

/*!
 * \brief called when user clicks the Ok button.
 */
void VcEnterNameDialog::okButtonClicked()
{
    if (ui->m_containerName->text().isEmpty())
        QMessageBox::warning(this, tr("Error"), tr("Name field is empty."));
    else
        accept();
}

} // namespace Internal
} // namespace VcProjectManager
