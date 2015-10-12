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
#include "configurationbasewidget.h"
#include "configurationswidget.h"
#include "newconfigitemdialog.h"
#include "ui_configurationswidget.h"
#include "vcenternamedialog.h"

namespace VcProjectManager {
namespace Internal {

/*!
 * \class ConfigurationsWidget
 * A widget which is used as a container for all build configurations.
 * It is used by \a ConfigurationsEditWidget and \a FileConfigurationsEditWidget in order to store
 * widgets for build configurations from project or files.
 *
 * \fn void addConfiguration(const QString &configName, QWidget *configWidget);
 * Adds a configuration's widget to a list of build configration widgets stored in this widget.
 *
 * \fn VcNodeWidget *configWidget(const QString &configName);
 * Returns a build configuration widget which represents build configuration with name \a configName.
 *
 * \fn void removeConfiguration(const QString &configNameWithPlatform);
 * Removes a build configuration's widget for build configuration with name \a configNameWithPlatform.
 *
 * \fn void renameConfiguration(const QString &newconfigNameWithPlatform, const QString &oldConfigNameWithPlatform);
 * Renames a build configuration which represents build configuration with name \a oldConfigNameWithPlatform.
 * New name is set to \a newconfigNameWithPlatform.
 *
 * \fn QList<VcNodeWidget *> configWidgets();
 * Returns a list of build configuration widgets contained in this widget.
 */

ConfigurationsWidget::ConfigurationsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigurationsWidget)
{
    ui->setupUi(this);
    connect(ui->m_configurationComboBox, SIGNAL(currentIndexChanged(int)),
            ui->m_configurationsStackWidget, SLOT(setCurrentIndex(int)));

    connect(ui->m_addNewConfigPushButton, SIGNAL(clicked()), this, SLOT(onAddNewConfig()));
    connect(ui->m_removeConfigPushButton, SIGNAL(clicked()), this, SLOT(onRemoveConfig()));
    connect(ui->m_renameConfigPushButton, SIGNAL(clicked()), this, SLOT(onRenameConfig()));
}

ConfigurationsWidget::~ConfigurationsWidget()
{
    delete ui;
}

void ConfigurationsWidget::addConfiguration(const QString &configName, QWidget *configWidget)
{
    if (!configWidget)
        return ;

    ui->m_configurationComboBox->addItem(configName);
    ui->m_configurationsStackWidget->addWidget(configWidget);
}

VcNodeWidget *ConfigurationsWidget::configWidget(const QString &configName)
{
    for (int i = 0; i < ui->m_configurationComboBox->count(); ++i) {
        QString name = ui->m_configurationComboBox->itemText(i);
        if (ui->m_configurationComboBox->itemText(i) == configName)
            return qobject_cast<VcNodeWidget *>(ui->m_configurationsStackWidget->widget(i));
    }

    return 0;
}

void ConfigurationsWidget::removeConfiguration(const QString &configNameWithPlatform)
{
    int index = indexOfConfig(configNameWithPlatform);
    QWidget *configW = configWidget(configNameWithPlatform);

    if (0 <= index && index < ui->m_configurationComboBox->count() && configW) {
        ui->m_configurationComboBox->removeItem(index);
        ui->m_configurationsStackWidget->removeWidget(configW);
    }
}

void ConfigurationsWidget::renameConfiguration(const QString &newConfigNameWithPlatform, const QString &oldConfigNameWithPlatform)
{
    int index = indexOfConfig(oldConfigNameWithPlatform);
    if (0 <= index && index < ui->m_configurationComboBox->count())
            ui->m_configurationComboBox->setItemText(index, newConfigNameWithPlatform);
}

QList<VcNodeWidget *> ConfigurationsWidget::configWidgets()
{
    QList<VcNodeWidget *> configWidgets;

    for (int i = 0; i < ui->m_configurationsStackWidget->count(); ++i) {
        VcNodeWidget *w = qobject_cast<VcNodeWidget *>(ui->m_configurationsStackWidget->widget(i));

        if (w)
            configWidgets.append(w);
    }

    return configWidgets;
}

void ConfigurationsWidget::onAddNewConfig()
{
    NewConfigItemDialog dlg(this);
    QString emptyString = tr("Empty");

    dlg.addConfigItem(emptyString);

    for (int i = 0; i < ui->m_configurationComboBox->count(); ++i)
        dlg.addConfigItem(ui->m_configurationComboBox->itemText(i));

    if (dlg.exec() == QDialog::Accepted) {
        if (dlg.copyFrom() == emptyString)
            emit addNewConfigSignal(dlg.name(), QString());
        else
            emit addNewConfigSignal(dlg.name(), dlg.copyFrom());
    }
}

void ConfigurationsWidget::onRenameConfig()
{
    QStringList splits = ui->m_configurationComboBox->currentText().split(QLatin1Char('|'));

    if (splits.isEmpty())
        return;

    VcEnterNameDialog dlg(tr("New name:"));
    dlg.setWindowTitle(tr("Rename Configuration"));

    dlg.setContainerName(splits[0]);

    if (dlg.exec() == QDialog::Accepted &&
            !ui->m_configurationComboBox->currentText().isEmpty() &&
            ui->m_configurationComboBox->currentText() != dlg.contanerName())
        emit renameConfigSignal(dlg.contanerName(), ui->m_configurationComboBox->currentText());
}

void ConfigurationsWidget::onRemoveConfig()
{
    if (ui->m_configurationComboBox->currentText().isEmpty())
        return;
    emit removeConfigSignal(ui->m_configurationComboBox->currentText());
}

/*!
 * \return an index of build configuration's widget with configuration name \a configNameWithPlatform
 * in this widget.
 */
int ConfigurationsWidget::indexOfConfig(const QString &configNameWithPlatform)
{
    for (int i = 0; i < ui->m_configurationComboBox->count(); ++i) {
        if (ui->m_configurationComboBox->itemText(i) == configNameWithPlatform)
            return i;
    }

    return -1;
}

} // namespace Internal
} // namespace VcProjectManager
