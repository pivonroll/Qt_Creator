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
#include "vcprojectfile.h"

#include "vcdocprojectnodes.h"
#include "../common/vcprojectmanagerconstants.h"
#include "vcprojectmodel/vcprojectdocument.h"
#include "utils/utils.h"

#include <vcdebuging.h>

#include <QFileInfo>

namespace VcProjectManager {
namespace Internal {

VcProjectFile::VcProjectFile(const QString &filePath, DocumentVersion docVersion)
    : m_tempModel(nullptr)
{
    setFilePath(Utils::FileName::fromString(filePath));
    m_documentModel = new VcProjectDocument(filePath, docVersion);
}

VcProjectFile::~VcProjectFile()
{
    delete m_documentModel;
}

bool VcProjectFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    Q_UNUSED(errorString)
    Q_UNUSED(fileName)
    Q_UNUSED(autoSave)
    // TODO: obvious
    return false;
}

QString VcProjectFile::defaultPath() const
{
    return m_documentModel->filePath();
}

QString VcProjectFile::suggestedFileName() const
{
    return QFileInfo(m_documentModel->filePath()).fileName();
}

bool VcProjectFile::isModified() const
{
    // TODO: obvious
    return false;
}

bool VcProjectFile::isSaveAsAllowed() const
{
    return false;
}

bool VcProjectFile::reload(QString *errorString, Core::IDocument::ReloadFlag flag, Core::IDocument::ChangeType type)
{
    Q_UNUSED(errorString);
    Q_UNUSED(flag);
    Q_UNUSED(type);
    return false;
}

/*!
 * \brief Creates a node tree for Visual Studio project
 * used to display file content of project in Qt Creator's project explorer.
 * \return root node of a tree representation for project.
 */
VcDocProjectNode *VcProjectFile::createProjectNode()
{
    QTC_ASSERT(m_documentModel, return nullptr);
    return new VcDocProjectNode(this);
}

/*!
 * \brief Deletes structure of a currently loaded project and creates a new one.
 */
void VcProjectFile::reloadVcDoc()
{
    DocumentVersion docVersion = m_documentModel->documentVersion();
    delete m_documentModel;
    m_documentModel = new VcProjectDocument(filePath().toString(), docVersion);
}

IVisualStudioProject *VcProjectFile::visualStudioProject() const
{
    return m_documentModel;
}

void VcProjectFile::showSettingsDialog()
{
    m_tempModel = m_documentModel->clone();
    VcNodeWidget *settingsWidget = m_tempModel->createSettingsWidget();

    QTC_ASSERT(settingsWidget, return);
    connect(settingsWidget, &VcNodeWidget::accepted, this, &VcProjectFile::onSettingsDialogAccepted);
    connect(settingsWidget, &VcNodeWidget::cancelled, this, &VcProjectFile::onSettingDislogCancelled);
    settingsWidget->show();
}

void VcProjectFile::showFileSettingsDialog(const QString &canonicalFilePath)
{
    FILE_EXISTS_ASSERT(canonicalFilePath);

    m_tempModel = m_documentModel->clone();
    IFile *file = m_tempModel->files()->findFile(canonicalFilePath);

    if (!file) {
        VS_DEBUG_PRINT(QLatin1String("File: ") + canonicalFilePath + QLatin1String(" not found!"));
        delete m_tempModel;
        m_tempModel = nullptr;
        return;
    }

    VcNodeWidget *settingsWidget = file->createSettingsWidget();
    if (settingsWidget) {
        connect(settingsWidget, &VcNodeWidget::accepted, this, &VcProjectFile::onSettingsDialogAccepted);
        connect(settingsWidget, &VcNodeWidget::cancelled, this, &VcProjectFile::onSettingDislogCancelled);
        settingsWidget->show();
    }
}

void VcProjectFile::onSettingsDialogAccepted()
{
    std::swap(m_documentModel, m_tempModel);
    delete m_tempModel;
    m_tempModel = nullptr;
}

void VcProjectFile::onSettingDislogCancelled()
{
    delete m_tempModel;
    m_tempModel = nullptr;
}

} // namespace Internal
} // namespace VcProjectManager
