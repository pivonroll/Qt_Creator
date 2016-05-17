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
#include "filesettingswidget.h"
#include "projectsettingswidget.h"
#include <visualstudiointerfaces/ifile.h>
#include <visualstudiowidgets/fileconfigurationseditwidget.h>

#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class FileSettingsWidget
 * A wrapper container for file build configuration widgets.
 */

FileSettingsWidget::FileSettingsWidget(IFile *file, QWidget *parent)
    : VcNodeWidget(parent)
{
    ProjectSettingsWidget *projectSettingsWidget = new ProjectSettingsWidget(this);

    // add Configurations
    m_fileConfigurationsWidget = static_cast<FileConfigurationsEditWidget *>(file->createSettingsWidget());
    projectSettingsWidget->addWidget(tr("Configurations"), m_fileConfigurationsWidget);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(projectSettingsWidget);
    setLayout(layout);

    connect(projectSettingsWidget, SIGNAL(okButtonClicked()), this, SLOT(onOkButtonClicked()));
    connect(projectSettingsWidget, SIGNAL(cancelButtonClicked()), this, SLOT(onCancelButtonClicked()));
}

/*!
 * \reimp
 */
void FileSettingsWidget::saveData()
{
    m_fileConfigurationsWidget->saveData();
}

/*!
 * \brief Called when Ok button is clicked.
 */
void FileSettingsWidget::onOkButtonClicked()
{
    saveData();
    hide();
    emit accepted();
    deleteLater();
}

/*!
 * \brief Called when Cancel button is clicked.
 */
void FileSettingsWidget::onCancelButtonClicked()
{
    hide();
    deleteLater();
}

} // namespace Internal
} // namespace VcProjectManager
