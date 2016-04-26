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
#include "basicconfigurationwidget.h"

#include <QHeaderView>
#include <QLabel>
#include <QSplitter>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class BasicConfigurationWidget
 * Basis configuration widget that displays configuration's options.
 * Options are displayed in a table as table rows.
 *
 * \fn void insertTableRow(const QString &column0, QWidget *widget, const QString &description)
 * Inserts a row in the table.
 */

BasicConfigurationWidget::BasicConfigurationWidget(QWidget *parent) :
    QWidget(parent)
{
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(2);
    m_tableWidget->horizontalHeader()->hide();
    m_tableWidget->verticalHeader()->hide();
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->setCornerButtonEnabled(false);
    m_tableWidget->setColumnWidth(0, 200);

    m_purposeLineLabel = new QLabel(this);
    m_purposeLineLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_purposeDescription = new QLabel(this);
    m_purposeDescription->setFixedHeight(50);
    m_purposeDescription->setWordWrap(true);
    m_purposeDescription->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QSplitter *horizontalSplitter = new QSplitter(Qt::Vertical, this);
    QVBoxLayout *commandLineLayout = new QVBoxLayout;
    commandLineLayout->setMargin(0);
    commandLineLayout->addWidget(m_purposeLineLabel);
    commandLineLayout->addWidget(m_purposeDescription);
    QWidget *commandWidget = new QWidget;
    commandWidget->setLayout(commandLineLayout);

    horizontalSplitter->addWidget(m_tableWidget);
    horizontalSplitter->addWidget(commandWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(horizontalSplitter);
    setLayout(mainLayout);

    horizontalSplitter->setStretchFactor(0, 1);

    connect(m_tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(onTableCellClicked(int, int)));
}

BasicConfigurationWidget::~BasicConfigurationWidget()
{
}

/*!
 * Inserts an new row in the table.
 * \a column0 is a option's display text.
 * \a widget represents a option's settings widget throught which option's value can be changed.
 * \a description is a option's description text.
 */
void BasicConfigurationWidget::insertTableRow(const QString &column0, QWidget *widget, const QString &description)
{
    m_tableWidget->insertRow(m_tableWidget->rowCount());
    QTableWidgetItem *columntItem0 = new QTableWidgetItem;
    columntItem0->setText(column0);

    m_tableWidget->setItem(m_tableWidget->rowCount() - 1, 0, columntItem0);
    m_tableWidget->setCellWidget(m_tableWidget->rowCount() - 1, 1, widget);
    m_tableWidget->resizeColumnsToContents();

    m_labelDescription.insert(column0, description);
}

/*!
 * \brief Called when table's cell is clicked in order to update
 * description text and purpose line field.
 */
void BasicConfigurationWidget::onTableCellClicked(int row, int column)
{
    Q_UNUSED(column);

    QTableWidgetItem *item = m_tableWidget->item(row, 0);

    if (item) {
        m_purposeLineLabel->setText(item->text());
        m_purposeDescription->setText(m_labelDescription[item->text()]);
    }
}

/*!
 * \brief Sets a purpose line's text.
 */
void BasicConfigurationWidget::setLineText(const QString &text)
{
    m_purposeLineLabel->setText(text);
}

/*!
 * \brief Sets a description line's text.
 */
void BasicConfigurationWidget::setLineDescription(const QString &desc)
{
    m_purposeDescription->setText(desc);
}

} // namespace Internal
} // namespace VcProjectManager
