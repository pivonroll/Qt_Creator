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
#include "toolschemawidget.h"
#include "ui_toolschemawidget.h"

#include "../common/vcschemamanager.h"

#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class ToolSchemaTableItem
 * This widget represents an XML tool info item.
 */

/*!
 * \class ToolSchemaWidget
 * This widget displays the tool info XML files that are being used by this plugin.
 */

ToolSchemaTableItem::ToolSchemaTableItem(const QString &text)
    : QTableWidgetItem(text)
{
}

/*!
 * \return a tool key from tool info XML file
 */
QString ToolSchemaTableItem::key() const
{
    return m_key;
}

/*!
 * \brief Set a tool key for tool info item.
 */
void ToolSchemaTableItem::setKey(const QString &key)
{
    m_key = key;
}

ToolSchemaWidget::ToolSchemaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolSchemaWidget)
{
    ui->setupUi(this);
    ui->m_toolXMLTable->setColumnCount(2);
    ui->m_toolXMLTable->verticalHeader()->hide();
    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << tr("Tool Name") << tr("File");
    ui->m_toolXMLTable->setHorizontalHeaderLabels(horizontalHeaderLabels);
    ui->m_toolXMLTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_toolXMLTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->m_toolXMLTable->horizontalHeader()->setSelectionMode(QAbstractItemView::NoSelection);

    VcSchemaManager *vcSM = VcSchemaManager::instance();
    QList<QString> toolXMLPaths = vcSM->toolXMLFilePaths();

    foreach (const QString &toolXMLPath, toolXMLPaths) {
        ToolInfo toolInfo = ToolDescriptionDataManager::readToolInfo(toolXMLPath);
        addToolSetting(toolInfo.m_displayName, toolInfo.m_key, toolXMLPath);
    }

    ui->m_toolSchemaLineEdit->setText(vcSM->toolSchema());

    connect(ui->m_toolSchemaBrowseButton, &QPushButton::clicked, this, &ToolSchemaWidget::onToolSchemaBrowseButton);
    connect(ui->m_addPushButton, &QPushButton::clicked, this, &ToolSchemaWidget::onToolXMLAddButton);
    connect(ui->m_removePushButton, &QPushButton::clicked, this, &ToolSchemaWidget::onToolXMLRemoveButton);
    connect(ui->m_toolXMLTable, &QTableWidget::currentCellChanged, this, &ToolSchemaWidget::onCurrentRowChanged);
    connect(ui->m_editToolPathPushButton, &QPushButton::clicked, this, &ToolSchemaWidget::onToolXMLPathChanged);
}

ToolSchemaWidget::~ToolSchemaWidget()
{
    delete ui;
}

/*!
 * \brief Saves paths to tool info XML files.
 */
void ToolSchemaWidget::saveSettings()
{
    VcSchemaManager *vcSM = VcSchemaManager::instance();

    vcSM->removeToolSchemas();

    if (vcSM) {
        for (int i = 0; i < ui->m_toolXMLTable->rowCount(); ++i) {
            ToolSchemaTableItem *tableItem = static_cast<ToolSchemaTableItem *>(ui->m_toolXMLTable->item(i, 0));
            QTableWidgetItem *item = ui->m_toolXMLTable->item(i, 1);

            if (tableItem && item)
                vcSM->addToolXML(tableItem->key(), item->text());
        }

        vcSM->setToolSchema(ui->m_toolSchemaLineEdit->text());
    }
}

/*!
 * \brief Adds a widget tool info widget to the table of tool info XML files.
 */
void ToolSchemaWidget::addToolSetting(const QString &toolDisplayName, const QString &toolKey, const QString &toolFilePath)
{
    ui->m_toolXMLTable->setRowCount(ui->m_toolXMLTable->rowCount() + 1);

    ToolSchemaTableItem *item = new ToolSchemaTableItem(toolDisplayName);
    item->setKey(toolKey);
    ui->m_toolXMLTable->setItem(ui->m_toolXMLTable->rowCount() - 1, 0, item);

    QTableWidgetItem *item2 = new QTableWidgetItem(toolFilePath);
    ui->m_toolXMLTable->setItem(ui->m_toolXMLTable->rowCount() - 1, 1, item2);
}

/*!
 * \brief Displays a dialog to browse for a tool info schema validation file.
 * After the dialog is acepted selected file's path is set to appropriate line edit widget.
 */
void ToolSchemaWidget::onToolSchemaBrowseButton()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Choose file path for tool..."),
                                                    QLatin1String("."),
                                                    QLatin1String("*.xsd"));
    ui->m_toolSchemaLineEdit->setText(filePath);
}

/*!
 * \brief Displays the dialog to add a tool info XML file.
 * If the tool info XML schema is not set error dialog wil be shown.
 * If the selected tool info XML file is not valid error dialog will be shown.
 */
void ToolSchemaWidget::onToolXMLAddButton()
{
    VcSchemaManager *vcSM = VcSchemaManager::instance();

    if (vcSM->toolSchema().isEmpty()) {
        QMessageBox::warning(this, tr("Error!"), tr("You must set tool schema fist."));
        return;
    }

    QString toolFilePath = QFileDialog::getOpenFileName(this, tr("Choose tool XML file..."), QString(), QLatin1String("*.xml"));

    if (toolFilePath.isEmpty())
        return;

    QString errorMsg;
    int errorLine;
    int errorColumn;
    ToolInfo toolInfo = ToolDescriptionDataManager::readToolInfo(toolFilePath, &errorMsg, &errorLine, &errorColumn);

    if (!toolInfo.isValid()) {
        QMessageBox::warning(this, tr("Error parsing tool file"), tr("Error: %1 \nline: %2 \ncolumn: %3").arg(errorMsg).arg(errorLine).arg(errorColumn));
        return;
    }

    addToolSetting(toolInfo.m_displayName, toolInfo.m_key, toolFilePath);
}

/*!
 * \brief Removes the selected tool info XML file from the list.
 */
void ToolSchemaWidget::onToolXMLRemoveButton()
{
    int currentRow = ui->m_toolXMLTable->currentRow();

    if (0 <= currentRow && currentRow < ui->m_toolXMLTable->rowCount())
        ui->m_toolXMLTable->removeRow(currentRow);
}

/*!
 * \brief When a tool info in the table is selected
 * it updates a name and path labels to show tool info's name
 * and tool info's full path.
 */
void ToolSchemaWidget::onCurrentRowChanged(int row)
{
    QTableWidgetItem *item = ui->m_toolXMLTable->item(row, 0);
    if (item)
        ui->m_toolDisplayNameLabel->setText(item->text());

    item = ui->m_toolXMLTable->item(row, 1);
    if (item)
        ui->m_toolPathLabel->setText(item->text());
}

/*!
 * \brief Displays the file selection dialog in order to choose
 * a tool info XML file path.
 * Selected file's path will be assigned to currently selected
 * tool info in the tool info table.
 */
void ToolSchemaWidget::onToolXMLPathChanged()
{
    QString toolXMLPath = QFileDialog::getOpenFileName(this, tr("Choose tool XML file..."), QString(), QLatin1String("*.xml"));
    int currentRow = ui->m_toolXMLTable->currentRow();
    QTableWidgetItem *item = ui->m_toolXMLTable->item(currentRow, 1);

    if (item)
        item->setText(toolXMLPath);

    ui->m_toolPathLabel->setText(toolXMLPath);
}

} // namespace Internal
} // namespace VcProjectManager
