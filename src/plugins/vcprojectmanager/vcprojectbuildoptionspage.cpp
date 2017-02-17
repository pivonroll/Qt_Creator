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
#include "common/vcschemamanager.h"
#include "ms_build/msbuildversionmanager.h"
#include "vcprojectbuildoptionspage.h"

#include <widgets/pluginwidgets/schemaoptionswidget.h>
#include <widgets/pluginwidgets/toolschemawidget.h>

#include <projectexplorer/projectexplorerconstants.h>

#include <utils/qtcassert.h>

#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QProcess>

namespace VcProjectManager {
namespace Internal {

VcProjectEditMsBuildDialog::VcProjectEditMsBuildDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *filePathLayout = new QHBoxLayout;
    m_pathChooser = new QLineEdit;
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_browseButton = new QPushButton(tr("Browse..."));
    filePathLayout->addWidget(m_pathChooser);
    filePathLayout->addWidget(m_browseButton);
    filePathLayout->setStretch(0, 1);
    mainLayout->addLayout(filePathLayout);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &VcProjectEditMsBuildDialog::accept);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &VcProjectEditMsBuildDialog::reject);
    connect(m_browseButton, &QPushButton::clicked, this, &VcProjectEditMsBuildDialog::showBrowseFileDialog);
}

VcProjectEditMsBuildDialog::~VcProjectEditMsBuildDialog()
{
}

/*!
 * \return path to a selected MS Build executable.
 */
QString VcProjectEditMsBuildDialog::path() const
{
    return m_pathChooser->text();
}

/*!
 * \brief Sets a path in line edit field to a canonical path of MS Build executable.
 */
void VcProjectEditMsBuildDialog::setPath(const QString &path)
{
    m_pathChooser->setText(path);
}

/*!
 * \brief Show the file select dialog used to select MS Build executable.
 * After the executable is selected it sets it's canonical path to line edit field.
 */
void VcProjectEditMsBuildDialog::showBrowseFileDialog()
{
    m_pathChooser->setText(QFileDialog::getOpenFileName(nullptr, tr("Select Ms Build"), QString(), QLatin1String("*.exe")));
}

MsBuildTableItem::MsBuildTableItem()
{
}

MsBuildTableItem::~MsBuildTableItem()
{
}

/*!
 * \return an unique id of a MS Build executable represented by this table item.
 */
Core::Id MsBuildTableItem::msBuildID() const
{
    return m_id;
}

/*!
 * \brief sets an unique id for MS Build executable represented by this table item.
 */
void MsBuildTableItem::setMsBuildID(Core::Id id)
{
    m_id = id;
}

VcProjectBuildOptionsWidget::VcProjectBuildOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QHBoxLayout *msBuildMainLayout = new QHBoxLayout;

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    m_addMsBuildButton = new QPushButton(tr("Add..."));
    m_editBuildButton = new QPushButton(tr("Edit..."));
    m_editBuildButton->setEnabled(false);
    m_deleteBuildButton = new QPushButton(tr("Delete"));
    m_deleteBuildButton->setEnabled(false);
    buttonLayout->addWidget(m_addMsBuildButton);
    buttonLayout->addWidget(m_editBuildButton);
    buttonLayout->addWidget(m_deleteBuildButton);
    buttonLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

    QVBoxLayout *tableLayout = new QVBoxLayout;
    m_buildTableWidget = new QTableWidget();
    m_buildTableWidget->setColumnCount(2);
    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << tr("Executable") << tr("Version");
    m_buildTableWidget->setHorizontalHeaderLabels(horizontalHeaderLabels);
    m_buildTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_buildTableWidget->horizontalHeader()->setSelectionMode(QAbstractItemView::NoSelection);
    m_buildTableWidget->verticalHeader()->hide();
    m_buildTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLayout->addWidget(m_buildTableWidget);

    tableLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

    msBuildMainLayout->addLayout(tableLayout);
    msBuildMainLayout->addLayout(buttonLayout);

    mainlayout->addLayout(msBuildMainLayout);

    m_schemaOptionsWidget = new SchemaOptionsWidget;
    mainlayout->addWidget(m_schemaOptionsWidget);

    m_toolSchemaWidget = new ToolSchemaWidget;
    mainlayout->addWidget(m_toolSchemaWidget);

    mainlayout->setStretchFactor(msBuildMainLayout, 0);
    mainlayout->setStretchFactor(m_schemaOptionsWidget, 0);
    mainlayout->setStretchFactor(m_toolSchemaWidget, 1);

    connect(m_addMsBuildButton, &QPushButton::clicked, this, &VcProjectBuildOptionsWidget::addNewButtonClicked);
    connect(m_editBuildButton, &QPushButton::clicked, this, &VcProjectBuildOptionsWidget::editButtonClicked);
    connect(m_deleteBuildButton, &QPushButton::clicked, this, &VcProjectBuildOptionsWidget::deleteButtonClicked);
    connect(m_buildTableWidget, &QTableWidget::cellClicked, this, &VcProjectBuildOptionsWidget::onTableRowIndexChange);

    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
    QList<MsBuildInformation *> msBuildInfos = msBVM->msBuildInformations();
    foreach (MsBuildInformation *msBuildInfo, msBuildInfos)
        insertMsBuildIntoTable(msBuildInfo);

    connect(msBVM, &MsBuildVersionManager::msBuildAdded, this, &VcProjectBuildOptionsWidget::onMsBuildAdded);
    connect(msBVM, &MsBuildVersionManager::msBuildRemoved, this, &VcProjectBuildOptionsWidget::onMsBuildRemoved);
    connect(msBVM, &MsBuildVersionManager::msBuildReplaced, this, &VcProjectBuildOptionsWidget::onMsBuildReplaced);
}

VcProjectBuildOptionsWidget::~VcProjectBuildOptionsWidget()
{
}

/*!
 * \return an unique id of a currently selected MS Build executable in the table.
 */
Core::Id VcProjectBuildOptionsWidget::currentSelectedBuildId() const
{
    QModelIndex currentIndex = m_buildTableWidget->selectionModel()->currentIndex();

    if (currentIndex.isValid()) {
        MsBuildTableItem *item = static_cast<MsBuildTableItem *>(m_buildTableWidget->item(currentIndex.row(), 0));
        return item->msBuildID(); // select ms build id
    }

    return Core::Id();
}

/*!
 * \return \b true if MS Build executable with \a exePath exists in the table.
 * Otherwise it returns \b false.
 */
bool VcProjectBuildOptionsWidget::exists(const QString &exePath)
{
    for (int i = 0; i < m_buildTableWidget->rowCount(); ++i) {
        QTableWidgetItem *item = m_buildTableWidget->item(i, 0);

        if (item->text() == exePath)
            return true;
    }

    return false;
}

/*!
 * \return \b true if table contains any rows, meaning it has any MS Build executables present in it.
 * Otherwise it returns \b false.
 */
bool VcProjectBuildOptionsWidget::hasAnyBuilds() const
{
    return m_buildTableWidget->rowCount() >= 0 ? true : false;
}

/*!
 * \brief Inserts MS Build contained in \a MsBuildInformation into the table.
 */
void VcProjectBuildOptionsWidget::insertMSBuild(MsBuildInformation *msBuild)
{
    QTC_ASSERT(msBuild, return);
    insertMsBuildIntoTable(msBuild);
    m_newMsBuilds.append(msBuild);
}

/*!
 * \brief Removes MS Build executable with \a msBuildId from the table.
 * If the MS Build executable with given \a msBuildId does not exist nothing happens.
 */
void VcProjectBuildOptionsWidget::removeMsBuild(Core::Id msBuildId)
{
    for (int i = 0; i < m_buildTableWidget->rowCount(); ++i) {
        MsBuildTableItem *item = static_cast<MsBuildTableItem *>(m_buildTableWidget->item(i, 0));

        if (item->msBuildID() == msBuildId) {
            m_buildTableWidget->removeRow(i);
            break;
        }
    }

    foreach (MsBuildInformation *info, m_newMsBuilds) {
        if (info->getId() == msBuildId) {
            m_newMsBuilds.removeAll(info);
            return;
        }
    }

    m_removedMsBuilds.append(msBuildId);
}

/*!
 * \brief Replaces MS Build executable with given \a msBuildId with
 * MS Build executable contained in \a newMsBuild.
 */
void VcProjectBuildOptionsWidget::replaceMsBuild(Core::Id msBuildId, MsBuildInformation *newMsBuild)
{
    // update data in table
    for (int i = 0; i < m_buildTableWidget->rowCount(); ++i) {
        MsBuildTableItem *item = static_cast<MsBuildTableItem *>(m_buildTableWidget->item(i, 0));

        if (item->msBuildID() == msBuildId) {
            item->setText(newMsBuild->m_executable);
            item->setMsBuildID(newMsBuild->getId());

            // update version column item
            QTableWidgetItem *item2 = m_buildTableWidget->item(i, 1);
            item2->setText(newMsBuild->m_versionString);
            break;
        }
    }

    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();

    if (msBVM->msBuildInformation(msBuildId)) {
        m_removedMsBuilds.append(msBuildId);
        m_newMsBuilds.append(newMsBuild);
    } else {
        m_newMsBuilds.append(newMsBuild);
        foreach (MsBuildInformation *info, m_newMsBuilds) {
            if (info->getId() == msBuildId) {
                m_newMsBuilds.removeAll(info);
                break;
            }
        }
    }
}

/*!
 * \brief Saves the data from this option dialog to the MS build version manager.
 * This function is called when "Apply" or "Ok" button from Qt Creator's option dialog are clicked.
 */
void VcProjectBuildOptionsWidget::saveSettings() const
{
    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();

    disconnect(msBVM, &MsBuildVersionManager::msBuildAdded, this, &VcProjectBuildOptionsWidget::onMsBuildAdded);
    disconnect(msBVM, &MsBuildVersionManager::msBuildRemoved, this, &VcProjectBuildOptionsWidget::onMsBuildRemoved);
    disconnect(msBVM, &MsBuildVersionManager::msBuildReplaced, this, &VcProjectBuildOptionsWidget::onMsBuildReplaced);

    if (msBVM) {
        foreach (const Core::Id &id, m_removedMsBuilds)
            msBVM->removeMsBuildInformation(id);

        foreach (MsBuildInformation *info, m_newMsBuilds)
            msBVM->addMsBuildInformation(info);

        msBVM->saveSettings();
    }

    connect(msBVM, &MsBuildVersionManager::msBuildAdded, this, &VcProjectBuildOptionsWidget::onMsBuildAdded);
    connect(msBVM, &MsBuildVersionManager::msBuildRemoved, this, &VcProjectBuildOptionsWidget::onMsBuildRemoved);
    connect(msBVM, &MsBuildVersionManager::msBuildReplaced, this, &VcProjectBuildOptionsWidget::onMsBuildReplaced);

    m_schemaOptionsWidget->saveSettings();
    m_toolSchemaWidget->saveSettings();
}

/*!
 * \return a schema options widget.
 * This widget displays paths to all available XML schema files used
 * for various validations, like project validation or configuration build tool xml validation.
 */
SchemaOptionsWidget *VcProjectBuildOptionsWidget::schemaOptionsWidget() const
{
    return m_schemaOptionsWidget;
}

/*!
 * \brief Called when new MS Build executable is added to MS build version manager.
 * New MS build executable is referenced by it's unique \a msBuildId.
 */
void VcProjectBuildOptionsWidget::onMsBuildAdded(Core::Id msBuildId)
{
    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
    MsBuildInformation *msBuild = msBVM->msBuildInformation(msBuildId);

    QTC_ASSERT(msBuild, return);
    insertMsBuildIntoTable(msBuild);
}

/*!
 * \brief Called when MS Build executable referenced by \a oldMsBuildId
 * is replaced by MS Build referenced by \a newMsBuildId in MS build version manager.
 */
void VcProjectBuildOptionsWidget::onMsBuildReplaced(Core::Id oldMsBuildId, Core::Id newMsBuildId)
{
    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
    MsBuildInformation *newMsBuild = msBVM->msBuildInformation(newMsBuildId);

    // update data in table
    for (int i = 0; i < m_buildTableWidget->rowCount(); ++i) {
        MsBuildTableItem *item = static_cast<MsBuildTableItem *>(m_buildTableWidget->item(i, 0));

        if (item->msBuildID() == oldMsBuildId) {
            item->setText(newMsBuild->m_executable);
            item->setMsBuildID(newMsBuild->getId());

            // update version column item
            QTableWidgetItem *item2 = m_buildTableWidget->item(i, 1);
            item2->setText(newMsBuild->m_versionString);
            break;
        }
    }
}

/*!
 * \brief Called when MS build with \a msBuildId
 * is removed from MS build version manager.
 */
void VcProjectBuildOptionsWidget::onMsBuildRemoved(Core::Id msBuildId)
{
    for (int i = 0; i < m_buildTableWidget->rowCount(); ++i) {
        MsBuildTableItem *item = static_cast<MsBuildTableItem *>(m_buildTableWidget->item(i, 0));

        if (item->msBuildID() == msBuildId) {
            m_buildTableWidget->removeRow(i);
            return;
        }
    }
}

/*!
 * \brief Called when table row selection changes.
 * \a index is an index of a newly selected row.
 */
void VcProjectBuildOptionsWidget::onTableRowIndexChange(int index)
{
    if (0 <= index && index < m_buildTableWidget->rowCount()) {
        m_editBuildButton->setEnabled(true);
        m_deleteBuildButton->setEnabled(true);
    }

    else {
        m_editBuildButton->setEnabled(false);
        m_deleteBuildButton->setEnabled(false);
    }

    emit currentBuildSelectionChanged(index);
}

/*!
 * \brief Handles insertion of a new MS Build into the table.
 */
void VcProjectBuildOptionsWidget::insertMsBuildIntoTable(MsBuildInformation *msBuild)
{
    if (msBuild) {
        MsBuildTableItem *exeTableItem = new MsBuildTableItem();
        exeTableItem->setFlags(exeTableItem->flags() ^ Qt::ItemIsEditable);
        exeTableItem->setText(msBuild->m_executable);
        exeTableItem->setMsBuildID(msBuild->getId());

        QTableWidgetItem *versionTableItem = new QTableWidgetItem();
        versionTableItem->setFlags(versionTableItem->flags() ^ Qt::ItemIsEditable);
        versionTableItem->setText(msBuild->m_versionString);

        m_buildTableWidget->insertRow(m_buildTableWidget->rowCount());
        m_buildTableWidget->setItem(m_buildTableWidget->rowCount() - 1, 0, exeTableItem);
        m_buildTableWidget->setItem(m_buildTableWidget->rowCount() - 1, 1, versionTableItem);

        if (!m_buildTableWidget->selectionModel()->currentIndex().isValid())
            m_buildTableWidget->selectRow(0);

        if (!m_editBuildButton->isEnabled())
            m_editBuildButton->setEnabled(true);

        if (!m_deleteBuildButton->isEnabled())
            m_deleteBuildButton->setEnabled(true);
    }
}

VcProjectBuildOptionsPage::VcProjectBuildOptionsPage() :
    m_optionsWidget(nullptr)
{
    setId(Core::Id("VcProject.MSBuild"));
    setDisplayName(tr("MS Build"));
    setCategory(Core::Id(VcProjectManager::Constants::VC_PROJECT_SETTINGS_CATEGORY));
    setDisplayCategory(QCoreApplication::translate("ProjectExplorer",
                                                   VcProjectManager::Constants::VC_PROJECT_SETTINGS_TR_CATEGORY));

    // TODO(Radovan): create and set proper icon
    setCategoryIcon(QLatin1String(ProjectExplorer::Constants::PROJECTEXPLORER_SETTINGS_CATEGORY_ICON));
}

VcProjectBuildOptionsPage::~VcProjectBuildOptionsPage()
{
}

QWidget *VcProjectBuildOptionsPage::widget()
{
    if (!m_optionsWidget) {
        m_optionsWidget = new VcProjectBuildOptionsWidget;

        connect(m_optionsWidget, &VcProjectBuildOptionsWidget::addNewButtonClicked, this, &VcProjectBuildOptionsPage::addNewMsBuild);
        connect(m_optionsWidget, &VcProjectBuildOptionsWidget::editButtonClicked, this, &VcProjectBuildOptionsPage::editMsBuild);
        connect(m_optionsWidget, &VcProjectBuildOptionsWidget::deleteButtonClicked, this, &VcProjectBuildOptionsPage::deleteMsBuild);
    }
    return m_optionsWidget;
}

void VcProjectBuildOptionsPage::apply()
{
    if (!m_optionsWidget || (m_optionsWidget && !m_optionsWidget->hasAnyBuilds()))
        return;

    saveSettings();
}

void VcProjectBuildOptionsPage::finish()
{
    delete m_optionsWidget;
    m_optionsWidget = nullptr;
}

/*!
 * \brief Saves the settings from MS Build option widget.
 */
void VcProjectBuildOptionsPage::saveSettings()
{
    QTC_ASSERT(m_optionsWidget, return);
    m_optionsWidget->saveSettings();
}

/*!
 * \brief Starts a version check of a MS Build executable.
 */
void VcProjectBuildOptionsPage::startVersionCheck()
{
    QTC_ASSERT(!m_validator.m_executable.isEmpty(), return);

    m_validator.m_process = new QProcess();
    connect(m_validator.m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(versionCheckFinished()));

    m_validator.m_process->start(m_validator.m_executable, QStringList(QLatin1String("/version")));
    m_validator.m_process->waitForStarted();
}

/*!
 * \brief Called when "Add" button is clicked.
 * It displays the file selection dialog used to select MS Build executable.
 */
void VcProjectBuildOptionsPage::addNewMsBuild()
{
    QString newMsBuild = QFileDialog::getOpenFileName(nullptr, tr("Select Ms Build"), QString(), QLatin1String("*.exe"));

    QTC_ASSERT(m_optionsWidget && !newMsBuild.isEmpty(), return);

    if (m_optionsWidget->exists(newMsBuild)) {
        QMessageBox::information(nullptr, tr("Ms Build already present"), tr("Selected MSBuild.exe is already present in the list."));
        return;
    }

    m_validator.m_executable = newMsBuild;
    m_validator.m_requestType = VcProjectValidator::ValidationRequest_Add;
    startVersionCheck();
}

/*!
 * \brief Displays edit dialog for selected MS Build executable in the table.
 * After the dialog is closed if the new MS build executalbe is choosen,
 * version check will be started.
 */
void VcProjectBuildOptionsPage::editMsBuild()
{
    QTC_ASSERT(m_optionsWidget, return);

    MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
    MsBuildInformation *currentSelectedMsBuild = msBVM->msBuildInformation(m_optionsWidget->currentSelectedBuildId());

    if (!currentSelectedMsBuild)
        return;

    m_validator.m_originalMsInfoID = currentSelectedMsBuild->getId();

    VcProjectEditMsBuildDialog editDialog;
    editDialog.setPath(currentSelectedMsBuild->m_executable);

    if (editDialog.exec() == QDialog::Accepted) {
        if (editDialog.path() == currentSelectedMsBuild->m_executable)
            return;

        m_validator.m_requestType = VcProjectValidator::ValidationRequest_Edit;
        m_validator.m_executable = editDialog.path();
        startVersionCheck();
    }
}

/*!
 * \brief Called when "Delete" button is clicked.
 * Selected MS build will be removed from the table.
 */
void VcProjectBuildOptionsPage::deleteMsBuild()
{
    m_optionsWidget->removeMsBuild(m_optionsWidget->currentSelectedBuildId());
}

/*!
 * \brief Called when version check for MS Build is done.
 * It will parse the result of \b MSBuild.exe \b /version command and
 * extract the version of the MS build executable.
 */
void VcProjectBuildOptionsPage::versionCheckFinished()
{
    QTC_ASSERT(m_validator.m_process, return);

    QString response = QVariant(m_validator.m_process->readAll()).toString();
    QStringList splitData = response.split(QLatin1Char('\n'));

    if (m_validator.m_requestType == VcProjectValidator::ValidationRequest_Add) {
        MsBuildInformation *newMsBuild = MsBuildVersionManager::createMsBuildInfo(m_validator.m_executable, splitData.last());
        m_optionsWidget->insertMSBuild(newMsBuild);
    }

    else if (m_validator.m_requestType == VcProjectValidator::ValidationRequest_Edit) {
        MsBuildInformation *newMsBuildInfo = MsBuildVersionManager::createMsBuildInfo(m_validator.m_executable, splitData.last());

        // update table data
        if (m_optionsWidget)
            m_optionsWidget->replaceMsBuild(m_validator.m_originalMsInfoID, newMsBuildInfo);
    }

    m_validator.m_process->deleteLater();
    m_validator.m_process = nullptr;
}

} // namespace Internal
} // namespace VcProjectManager
