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
#include "visualstudiosolutionfile.h"

#include "../common/projectconstants.h"
#include "../common/vcprojectmanagerconstants.h"
#include "visualstudiosolutionnode.h"
#include "visualstudiosolutionparser/visualstudiosolutionparser.h"
#include "utils.h"

#include <vcprojectmodel/vcprojectdocument.h>
#include <vcprojectxmodel/vcprojectdocumentx.h>

#include <utils/fileutils.h>
#include <utils/mimetypes/mimedatabase.h>

#include <QFileInfo>

namespace VcProjectManager {
namespace Internal {

VisualStudioSolutionFile::VisualStudioSolutionFile(const QString &filePath)
{
    m_visualStudioParser = new VisualStudioProjectNS::Internal::VisualStudioSolutionParser(filePath);

    setFilePath(Utils::FileName::fromString(filePath));

    Utils::MimeDatabase mdb;

    QString canonicalSolutionDirectoryPath = QFileInfo(filePath).absolutePath();
    foreach (const VisualStudioProjectNS::Internal::ProjectReference &projectReference, m_visualStudioParser->m_projectReferences) {
        QString fullProjectPath = canonicalSolutionDirectoryPath + QLatin1Char('/') + projectReference.m_relativeProjectPath;

        Utils::MimeType projectMimeType = mdb.mimeTypeForFile(fullProjectPath);
        IVisualStudioProject *newProject = nullptr;

        if (projectMimeType.matchesName(QLatin1String(Constants::VC_X_PROJ_MIMETYPE)) && QFileInfo(fullProjectPath).exists()) {
            newProject = new VcProjectDocumentX(fullProjectPath);
        }
        else if (projectMimeType.matchesName(QLatin1String(Constants::VCPROJ_MIMETYPE))) {
            // function getProjectVersion will only return DV_MSVC_2003, DV_MSVC_2005 or DV_MSVC_2008,
            DocumentVersion docVersion = VisualStudioUtils::getProjectVersion(fullProjectPath);

            if (docVersion != DV_UNRECOGNIZED)
                newProject = new VcProjectDocument(fullProjectPath, docVersion);
        }

        m_visualStudioProjects[projectReference.m_referenceId] = newProject;
    }
}

VisualStudioSolutionFile::~VisualStudioSolutionFile()
{
    qDeleteAll(m_visualStudioProjects);
    delete m_visualStudioParser;
}

bool VisualStudioSolutionFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    Q_UNUSED(errorString)
    Q_UNUSED(fileName)
    Q_UNUSED(autoSave)
    return false;
}

QString VisualStudioSolutionFile::defaultPath() const
{
    return QString();
}

QString VisualStudioSolutionFile::suggestedFileName() const
{
    return QFileInfo(m_visualStudioParser->m_filePath).fileName();
}

bool VisualStudioSolutionFile::isModified() const
{
    return false;
}

bool VisualStudioSolutionFile::isSaveAsAllowed() const
{
    return false;
}

bool VisualStudioSolutionFile::reload(QString *errorString, Core::IDocument::ReloadFlag flag, Core::IDocument::ChangeType type)
{
    Q_UNUSED(errorString)
    Q_UNUSED(flag)
    Q_UNUSED(type)
    return false;
}

VisualStudioSolutionNode *VisualStudioSolutionFile::createSolutionNode()
{
    return new VisualStudioSolutionNode(this);
}

} // namespace Internal
} // namespace VcProjectManager

