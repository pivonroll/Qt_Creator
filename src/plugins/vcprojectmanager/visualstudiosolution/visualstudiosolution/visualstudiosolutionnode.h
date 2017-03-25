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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUAL_STUDIO_SOLUTION_NODE_H
#define VCPROJECTMANAGER_INTERNAL_VISUAL_STUDIO_SOLUTION_NODE_H

#include <projectexplorer/projectnodes.h>
#include "visualstudiosolution/visualstudiosolutionparser/projectreference.h"

namespace VisualStudioProjectNS {
namespace Internal {
    struct FolderReference;
    class VisualStudioSolutionParser;
} // namespace Internal
} // namespace VisualStudioProjectNS

namespace VcProjectManager {
namespace Internal {

class VisualStudioSolutionFile;
class VcDocProjectNode;

class VisualStudioSolutionFolder : public ProjectExplorer::ProjectNode
{
    friend class VisualStudioSolutionNode;
public:
    VisualStudioSolutionFolder(const VisualStudioProjectNS::Internal::FolderReference &folderReference);
    ~VisualStudioSolutionFolder();

    bool canAddSubProject(const QString &proFilePath) const final;
    bool addSubProjects(const QStringList &proFilePaths);
    bool removeSubProjects(const QStringList &proFilePaths);

private:
    VisualStudioProjectNS::Internal::FolderReference m_folderReference;
};

class VisualStudioSolutionNode : public ProjectExplorer::ProjectNode
{
public:
    VisualStudioSolutionNode(VisualStudioSolutionFile *visualStudioSolutionFile);
    ~VisualStudioSolutionNode();

    /*!
     * \return a list of supported actions.
     */
    QList<ProjectExplorer::ProjectAction> supportedActions(Node *node) const;

    /*!
     * \brief Does nothing.
     * \return \b true.
     */
    bool canAddSubProject(const QString &proFilePath) const;

    /*!
     * \brief Does nothing.
     * \return \b true.
     */
    bool addSubProjects(const QStringList &proFilePaths);

    /*!
     * \brief Does nothing.
     * \return \b true.
     */
    bool removeSubProjects(const QStringList &proFilePaths);

    /*!
     * \brief Creates file nodes for files in a \a filePaths and
     * adds those nodes to this project node.
     * \return \b true if any of the files in  \a filePaths is added to the project node.
     * Otherwise it returns \b false.
     */
    bool addFiles(const QStringList &filePaths, QStringList *notAdded);

    /*!
     * \brief Removes files in \a filePaths from this project.
     * \return \b true if all files are removed.
     * Otherwise it returns \b false.
     */
    bool removeFiles(const QStringList &filePaths, QStringList *notRemoved);

    /*!
     * \brief Does nothing.
     * \return \b false.
     */
    bool deleteFiles(const QStringList &filePaths);

    /*!
     * \brief Does nothing.
     * \return \b false.
     */
    bool renameFile(const QString &filePath, const QString &newFilePath);

private:
    void addProjects();
    VisualStudioSolutionFolder *findFolderNode(const QString &nodeId, const QList<VisualStudioSolutionFolder *> &nodes) const;
    ProjectExplorer::ProjectNode *findProjectNode(const QString &nodeId, const QMap<QString, ProjectExplorer::ProjectNode *> &nodes) const;

    VisualStudioSolutionFile *m_visualStudioSolutionFile;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUAL_STUDIO_SOLUTION_NODE_H
