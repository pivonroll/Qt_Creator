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
#ifndef VCPROJECTMANAGER_INTERNAL_VCXPROJECT_H
#define VCPROJECTMANAGER_INTERNAL_VCXPROJECT_H

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <QFuture>

namespace VcProjectManager {
namespace Internal {

class VcXProjectManager;
class VcDocProjectNode;
class VcXProjectFile;

class VcXProject : public ProjectExplorer::Project
{
    Q_OBJECT
public:
    VcXProject(VcXProjectManager *projectManager, const QString &canonicalFilePath);
    ~VcXProject();

    // Project interface
    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;

private:
    void addCxxModelFiles(const ProjectExplorer::FolderNode *node, QSet<QString> &projectFiles);
    void updateCodeModels();

    VcXProjectManager *m_projectManager;
    QString m_filePath;
    VcDocProjectNode *m_rootNode;
    VcXProjectFile *m_projectFile;
    QFuture<void> m_codeModelFuture;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VCXPROJECT_H
