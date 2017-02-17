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
#include "visualstudiosolutionnode.h"

#include "visualstudiosolutionfile.h"
#include "../vcdocprojectnodes.h"
#include "visualstudiosolutionparser/visualstudiosolutionparser.h"

namespace VcProjectManager {
namespace Internal {

VisualStudioSolutionFolder::VisualStudioSolutionFolder(const VisualStudioProjectNS::Internal::FolderReference &folderReference)
    : ProjectExplorer::ProjectNode(Utils::FileName::fromString(folderReference.m_displayName))
    , m_folderReference(folderReference)
{
}

VisualStudioSolutionFolder::~VisualStudioSolutionFolder()
{
}

bool VisualStudioSolutionFolder::canAddSubProject(const QString &proFilePath) const
{
    Q_UNUSED(proFilePath)
    return false;
}

bool VisualStudioSolutionFolder::addSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

bool VisualStudioSolutionFolder::removeSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

VisualStudioSolutionNode::VisualStudioSolutionNode(VisualStudioSolutionFile *visualStudioSolutionFile)
    : ProjectExplorer::ProjectNode(visualStudioSolutionFile->filePath())
    , m_visualStudioSolutionFile(visualStudioSolutionFile)
{
    addProjects();
}

VisualStudioSolutionNode::~VisualStudioSolutionNode()
{
}

QList<ProjectExplorer::ProjectAction> VisualStudioSolutionNode::supportedActions(ProjectExplorer::Node *node) const
{
    Q_UNUSED(node)
    QList<ProjectExplorer::ProjectAction> actions;
    actions << ProjectExplorer::ProjectAction::AddSubProject;
    actions << ProjectExplorer::ProjectAction::RemoveSubProject;
    return actions;
}

bool VisualStudioSolutionNode::canAddSubProject(const QString &proFilePath) const
{
    Q_UNUSED(proFilePath)
    return false;
}

bool VisualStudioSolutionNode::addSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

bool VisualStudioSolutionNode::removeSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

bool VisualStudioSolutionNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    Q_UNUSED(filePaths)
    Q_UNUSED(notAdded)
    return false;
}

bool VisualStudioSolutionNode::removeFiles(const QStringList &filePaths, QStringList *notRemoved)
{
    Q_UNUSED(filePaths)
    Q_UNUSED(notRemoved)
    return false;
}

bool VisualStudioSolutionNode::deleteFiles(const QStringList &filePaths)
{
    Q_UNUSED(filePaths)
    return false;
}

bool VisualStudioSolutionNode::renameFile(const QString &filePath, const QString &newFilePath)
{
    Q_UNUSED(filePath)
    Q_UNUSED(newFilePath)
    return false;
}

void VisualStudioSolutionNode::addProjects()
{
//    // create project nodes
//    QMap<QString, ProjectExplorer::ProjectNode *> projectNodes;
//    QMap<QString, IVisualStudioProject *> projects = m_visualStudioSolutionFile->m_visualStudioProjects;
//    QMapIterator<QString, IVisualStudioProject *> it(projects);

//    while (it.hasNext()) {
//        it.next();
//        VcDocProjectNode *newVisualStudioProjectNode = new VcDocProjectNode(it.value());
//        projectNodes[it.key()] = newVisualStudioProjectNode;
//    }

//    // create folder nodes
//    QList<VisualStudioSolutionFolder *> folderNodes;
//    QList<VisualStudioProjectNS::Internal::FolderReference> folders = m_visualStudioSolutionFile->m_visualStudioParser->m_folderReferences;
//    foreach (const VisualStudioProjectNS::Internal::FolderReference &folder, folders) {
//        VisualStudioSolutionFolder *newVisualStudioFolderNode = new VisualStudioSolutionFolder(folder);
//        folderNodes << newVisualStudioFolderNode;
//    }

//    VisualStudioProjectNS::Internal::GlobalSection nestedProjectsSection;

//    // find nested projects section
//    foreach (const VisualStudioProjectNS::Internal::GlobalSection &globalSection, m_visualStudioSolutionFile->m_visualStudioParser->m_globals.m_sections) {
//        if (globalSection.m_name == QLatin1String("NestedProjects")) {
//            nestedProjectsSection = globalSection;
//            break;
//        }
//    }

//    if (!nestedProjectsSection.m_name.isEmpty()) {
//        // move projects into folder nodes
//        foreach (const VisualStudioProjectNS::Internal::NestedProject &nestedProjectEntry, nestedProjectsSection.m_nestedProjects) {
//            ProjectExplorer::ProjectNode *project = findProjectNode(nestedProjectEntry.m_idKey, projectNodes);
//            VisualStudioSolutionFolder *folder = findFolderNode(nestedProjectEntry.m_targetId, folderNodes);

//            if (project && folder) {
//                QList<ProjectExplorer::ProjectNode *> projNodes;
//                projNodes << project;
//                folder->addProjectNodes(projNodes);

//                projectNodes.remove(nestedProjectEntry.m_idKey);
//            }
//        }

//        // move child folders into parent folders
//        foreach (const VisualStudioProjectNS::Internal::NestedProject &nestedProjectEntry, nestedProjectsSection.m_nestedProjects) {
//            VisualStudioSolutionFolder *folderChild = findFolderNode(nestedProjectEntry.m_idKey, folderNodes);
//            VisualStudioSolutionFolder *folderParent = findFolderNode(nestedProjectEntry.m_targetId, folderNodes);

//            if (folderChild && folderParent) {
//                QList<ProjectExplorer::ProjectNode *> projNodes;
//                projNodes << folderChild;
//                folderParent->addProjectNodes(projNodes);

//                folderNodes.removeAll(folderChild);
//            }
//        }
//    }

//    // append remaining project nodes
//    QList<ProjectExplorer::ProjectNode *> allTopLevelNodes;

//    foreach (VisualStudioSolutionFolder *folder, folderNodes)
//        allTopLevelNodes << folder;

//    allTopLevelNodes << projectNodes.values();
//    addProjectNodes(allTopLevelNodes);
}

VisualStudioSolutionFolder *VisualStudioSolutionNode::findFolderNode(const QString &nodeId, const QList<VisualStudioSolutionFolder *> &nodes) const
{
    foreach (VisualStudioSolutionFolder *folderNode, nodes) {
        if (folderNode->m_folderReference.m_referenceId == nodeId)
            return folderNode;
    }

    return nullptr;
}

ProjectExplorer::ProjectNode *VisualStudioSolutionNode::findProjectNode(const QString &nodeId, const QMap<QString, ProjectExplorer::ProjectNode *> &nodes) const
{
    QMapIterator<QString, ProjectExplorer::ProjectNode *> it(nodes);
    while (it.hasNext()) {
        it.next();

        if (it.key() == nodeId)
            return it.value();
    }

    return nullptr;
}

} // namespace Internal
} // namespace VcProjectManager

