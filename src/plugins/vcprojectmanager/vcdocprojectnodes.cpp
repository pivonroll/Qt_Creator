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
#include "vcprojectmanagerconstants.h"
#include "vcdocprojectnodes.h"

#include <widgets/visualstudiowidgets/filesettingswidget.h>

#include <visualstudiointerfaces/ifile.h>
#include <visualstudiointerfaces/ifilecontainer.h>
#include <visualstudiointerfaces/ifilefactory.h>
#include <visualstudiointerfaces/ifiles.h>
#include <visualstudiointerfaces/iprojectfactories.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>
#include "utils.h"
#include "vcprojectfile.h"

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projecttree.h>

#include <QDir>
#include <QFileInfo>

namespace VcProjectManager {
namespace Internal {

VcFileNode::VcFileNode(const ::Utils::FileName &filePath, ProjectExplorer::FileType fileType, VcDocProjectNode *vcDocProject)
    : ProjectExplorer::FileNode(filePath, fileType, false)
{
    Q_UNUSED(vcDocProject)
    //    connect(this, SIGNAL(settingsDialogAccepted()), vcDocProject, SIGNAL(settingsDialogAccepted()));
}

VcFileNode::~VcFileNode()
{
}

void VcFileNode::showSettingsWidget()
{
    // get IFile model from IVisualStudioProject
    FileSettingsWidget *settingsWidget = new FileSettingsWidget(m_vcFileModel);
    if (settingsWidget) {
//        connect(settingsWidget, SIGNAL(accepted()), this, SIGNAL(settingsDialogAccepted()));
        settingsWidget->show();
    }
}

void VcFileNode::readChildren(VcDocProjectNode *vcDocProj)
{
    Q_UNUSED(vcDocProj)
}

VcFileContainerNode::VcFileContainerNode(const ::Utils::FileName &filePath, VcDocProjectNode *vcDocProjNode)
    : ProjectExplorer::FolderNode(filePath),
      m_parentVcDocProjNode(vcDocProjNode)
{
    m_vcContainerType = VcContainerType_Filter;
    if (m_vcFileContainerModel->containerType() == QLatin1String(Constants::VC_PROJECT_FILE_CONTAINER_FOLDER))
        m_vcContainerType = VcContainerType_Folder;

    setProjectNode(vcDocProjNode);
    readChildren();
}

VcFileContainerNode::~VcFileContainerNode()
{
}

bool VcFileContainerNode::showInSimpleTree() const
{
    return true;
}

void VcFileContainerNode::addFileNode(const QString &filePath)
{
    VcFileNode *fileNode = new VcFileNode(::Utils::FileName::fromString(filePath),
                                          VisualStudioUtils::getFileType(::Utils::FileName::fromString(filePath),
                                          m_parentVcDocProjNode);

    if (!appendFileNode(fileNode)) {
        delete file;
        delete fileNode;
    }
}

bool VcFileContainerNode::appendFileNode(VcFileNode *fileNode)
{
    // check if a file already exists
    if (m_fileNodes.contains(fileNode))
        return false;
    else {
        foreach (ProjectExplorer::FileNode *fNode, m_fileNodes) {
            if (fNode->path() == fileNode->path())
                return false;
        }
    }

    // if file does not exist, add it
    QString relativeFilePath = VisualStudioUtils::fileRelativePath(m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()
                                                                   ->filePath(), fileNode->path());
    IFile *file = m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()->
            projectFactories()->fileFactory()->createFile(relativeFilePath, fileNode->fileType());

    if (!file)
        return false;

    file->setRelativePath(relativeFilePath);

    // find a proper IFileContainer which is represented by this node
    IFileContainer *fileContainer = findFileContainer();

    fileContainer->addFile(file);
    QList<ProjectExplorer::FileNode *> vcFileNodes;
    vcFileNodes << fileNode;
    m_parentVcDocProjNode->addFileNodes(vcFileNodes);
    m_parentVcDocProjNode->m_vcProjectFile->
            visualStudioProject()->saveToFile(m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()->filePath());
    return true;
}

void VcFileContainerNode::addFileContainerNode(const QString &name, VcContainerType type)
{
    QString containerType;

    if (type == VcContainerType_Filter)
        containerType = QLatin1String(Constants::VC_PROJECT_FILE_CONTAINER_FILTER);
    else if (type == VcContainerType_Folder)
        containerType = QLatin1String(Constants::VC_PROJECT_FILE_CONTAINER_FOLDER);

    if (containerType.isEmpty())
        return;

    VcFileContainerNode *folderNode = new VcFileContainerNode(::Utils::FileName::fromString(name), m_parentVcDocProjNode);

    QStringList folderPath;
    readFolderPathInsideProjectTree(folderPath);
    folderNode->setPath(folderPath.join(QLatin1String("\\")) + QLatin1String("\\") + name);

    if (!appendFileContainerNode(folderNode))
        delete folderNode;
}

bool VcFileContainerNode::appendFileContainerNode(VcFileContainerNode *fileContainer)
{
    if (m_subFolderNodes.contains(fileContainer))
        return false;
    else {
        foreach (ProjectExplorer::FolderNode *fNode, m_subFolderNodes) {
            if (fNode->displayName() == fileContainer->displayName())
                return false;
        }
    }

    IFileContainer *fileContainerModel = m_parentVcDocProjNode->m_vcProjectFile->
            visualStudioProject()->projectFactories()->fileFactory()->
            createFileContainer(fileContainer->path(), m_vcContainerType);

    fileContainerModel->setDisplayName(fileContainer->displayName());

    IFileContainer *fileCont = findFileContainer();

    fileCont->addFileContainer(fileContainerModel);
    QList<ProjectExplorer::FolderNode *> vcFolderNodes;
    vcFolderNodes << fileContainer;
    addFolderNodes(vcFolderNodes);
    m_parentVcDocProjNode->m_vcProjectFile->
            visualStudioProject()->saveToFile(m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()->filePath());
    return true;
}

void VcFileContainerNode::removeFileContainerNode(VcFileContainerNode *fileContainer)
{
    IFileContainer *fileContainerModel = findFileContainer();

    QList<ProjectExplorer::FolderNode *> folderNodesToRemove;
    folderNodesToRemove << fileContainer;
    m_parentVcDocProjNode->removeFolderNodes(folderNodesToRemove);

    m_vcFileContainerModel->removeFileContainer(fileContainerModel);
    m_parentVcDocProjNode->m_vcProjectFile
            ->visualStudioProject()->saveToFile(m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()->filePath());
}

void VcFileContainerNode::removeFileNode(VcFileNode *fileNode)
{
    IFileContainer *fileContainerModel = findFileContainer();

    IFile *file = m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()
            ->files()->findFile(fileNode->path());

    QList<ProjectExplorer::FileNode *> fileNodesToRemove;
    fileNodesToRemove << fileNode;
    m_parentVcDocProjNode->removeFileNodes(fileNodesToRemove);

    fileContainerModel->removeFile(file);
    m_parentVcDocProjNode->m_vcProjectFile
            ->visualStudioProject()->saveToFile(m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()->filePath());
}

void VcFileContainerNode::readChildren()
{
    QList<ProjectExplorer::FolderNode *> vcFolderNodes;
    IFileContainer *fileContainerModel = findFileContainer();

    for (int i = 0; i < fileContainerModel->childCount(); ++i) {
        IFileContainer *fileCont = fileContainerModel->fileContainer(i);

        if (fileCont) {
            VcFileContainerNode *newVcFileCont = new VcFileContainerNode(::Utils::FileName::fromString(fileCont->relativePath(), m_parentVcDocProjNode);
            newVcFileCont->setDisplayName(fileCont->displayName());
            vcFolderNodes.append(newVcFileCont);
        }
    }

    addFolderNodes(vcFolderNodes);

    QList<ProjectExplorer::FileNode *> vcFileNodes;

    for (int i = 0; i < fileContainerModel->fileCount(); ++i) {
        IFile *file = fileContainerModel->file(i);

        if (file)
            vcFileNodes.append(new VcFileNode(::Utils::FileName::fromString(file->canonicalPath()), m_parentVcDocProjNode));
    }

    addFileNodes(vcFileNodes);
}

void VcFileContainerNode::readFolderPathInsideProjectTree(QStringList &path) const
{
    path.push_front(displayName());

    FolderNode *folderNode = parentFolderNode();

    if (folderNode) {
        VcFileContainerNode *fileContNode = dynamic_cast<VcFileContainerNode *>(folderNode);
        fileContNode->readFolderPathInsideProjectTree(path);
    }
}

IFileContainer *VcFileContainerNode::findFileContainer() const
{
    QStringList folderPath;
    readFolderPathInsideProjectTree(folderPath);
    IFileContainer *fileCont = m_parentVcDocProjNode->m_vcProjectFile->visualStudioProject()
                                ->files()->findFileContainer(folderPath);
    return fileCont;
}

VcFileNode *VcFileContainerNode::findFileNode(const QString &filePath)
{
    VcFileNode *fileNode = nullptr;
    foreach (ProjectExplorer::FileNode *f, fileNodes()) {
        // There can be one match only here!
        if (f->path().toString() == filePath)
            return static_cast<VcFileNode *>(f);
    }

    foreach (ProjectExplorer::FolderNode *folderNode, m_subFolderNodes) {
        VcFileContainerNode *containerNode = static_cast<VcFileContainerNode *>(folderNode);
        fileNode = containerNode->findFileNode(filePath);

        if (fileNode)
            return fileNode;
    }

    return nullptr;
}

VcDocProjectNode::VcDocProjectNode(VcProjectFile *vcProjectFile)
    : ProjectExplorer::ProjectNode(::Utils::FileName::fromString(vcProjectFile->visualStudioProject()->filePath())),
      m_vcProjectFile(vcProjectFile)
{
    if (!m_vcProjectFile->visualStudioProject()->files())
        return;

    QList<ProjectExplorer::FolderNode *> vcFolderNodes;
    for (int i = 0; i < m_vcProjectFile->visualStudioProject()->files()->fileContainerCount(); ++i) {
        IFileContainer *fileContainer = m_vcProjectFile->visualStudioProject()->files()->fileContainer(i);
        if (fileContainer) {
            VcFileContainerNode *newVcFileContNode = new VcFileContainerNode(::Utils::FileName::fromString(fileContainer->relativePath()), this);
            newVcFileContNode->setDisplayName(fileContainer->displayName());
            vcFolderNodes.append(newVcFileContNode);
        }
    }
    addFolderNodes(vcFolderNodes);

    QList<ProjectExplorer::FileNode *> vcFileNodes;
    for (int i = 0; i < m_vcProjectFile->visualStudioProject()->files()->fileCount(); ++i) {
        IFile *file = m_vcProjectFile->visualStudioProject()->files()->file(i);
        if (file)
            vcFileNodes.append(new VcFileNode(::Utils::FileName::fromString(file->canonicalPath()), this));
    }
    addFileNodes(vcFileNodes);
}

VcDocProjectNode::~VcDocProjectNode()
{
}

QList<ProjectExplorer::ProjectAction> VcDocProjectNode::supportedActions(ProjectExplorer::Node *node) const
{
    QList<ProjectExplorer::ProjectAction> actions;

    actions << ProjectExplorer::AddNewFile
            << ProjectExplorer::AddExistingFile;

    ProjectExplorer::FileNode *fileNode = dynamic_cast<ProjectExplorer::FileNode *>(node);
    if (fileNode && fileNode->fileType() != ProjectExplorer::ProjectFileType) {
        actions << ProjectExplorer::Rename;
        actions << ProjectExplorer::RemoveFile;
    }

    return actions;
}

bool VcDocProjectNode::canAddSubProject(const QString &proFilePath) const
{
    Q_UNUSED(proFilePath)
    return false;
}

bool VcDocProjectNode::addSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

bool VcDocProjectNode::removeSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

bool VcDocProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    // add files like in VS
    //    if ()

    // add files to the node which called it
    ProjectExplorer::Node *node = ProjectExplorer::ProjectTree::currentNode();

    QStringList filesNotAdded;
    bool anyFileAdded = false;

    if (node->nodeType() == ProjectExplorer::FolderNodeType) {
        VcFileContainerNode *vcContainerNode = static_cast<VcFileContainerNode *>(node);

        if (vcContainerNode) {
            foreach (const QString &filePath, filePaths) {
                QString relativeFilePath = VisualStudioUtils::fileRelativePath(m_vcProjectFile->visualStudioProject()->filePath(), filePath);

                // if file is already in the project don't add it
                if (m_vcProjectFile->visualStudioProject()->files()->fileExists(relativeFilePath))
                    filesNotAdded << filePath;

                else {
                    vcContainerNode->addFileNode(filePath);
                    anyFileAdded = true;
                }
            }
        }
    }

    if (node->nodeType() == ProjectExplorer::ProjectNodeType) {
        VcDocProjectNode *projectNode = static_cast<VcDocProjectNode *>(node);

        if (projectNode) {
            foreach (const QString &filePath, filePaths) {
                QString relativeFilePath = VisualStudioUtils::fileRelativePath(m_vcProjectFile->visualStudioProject()->filePath(), filePath);

                // if file is already in the project don't add it
                if (m_vcProjectFile->visualStudioProject()->files()->fileExists(relativeFilePath))
                    filesNotAdded << filePath;

                else {
                    projectNode->addFileNode(relativeFilePath);
                    anyFileAdded = true;
                }
            }
        }
    }

    if (notAdded)
        *notAdded = filesNotAdded;

    if (anyFileAdded)
        m_vcProjectFile->visualStudioProject()->saveToFile(m_vcProjectFile->visualStudioProject()->filePath());

    return filesNotAdded.isEmpty();
}

bool VcDocProjectNode::removeFiles(const QStringList &filePaths, QStringList *notRemoved)
{
    QStringList filesNotRemoved;

    foreach (const QString &filePath, filePaths) {
        QString relativeFilePath = VisualStudioUtils::fileRelativePath(m_vcProjectFile->visualStudioProject()->filePath(), filePath);

        if (m_vcProjectFile->visualStudioProject()->files()->fileExists(relativeFilePath)) {
            VcFileNode *fileNode = static_cast<VcFileNode *>(findFileNode(filePath));

            if (fileNode) {
                ProjectExplorer::FolderNode *parentNode = fileNode->parentFolderNode();

                if (parentNode && parentNode->nodeType() == ProjectExplorer::FolderNodeType) {
                    VcFileContainerNode *containerNode = static_cast<VcFileContainerNode *>(parentNode);

                    if (containerNode)
                        containerNode->removeFileNode(fileNode);
                }

                else if (parentNode && parentNode->nodeType() == ProjectExplorer::ProjectNodeType) {
                    VcDocProjectNode *projectNode = static_cast<VcDocProjectNode *>(parentNode);

                    if (projectNode)
                        projectNode->removeFileNode(fileNode);
                }
            }
        }

        else
            filesNotRemoved << filePath;
    }

    if (notRemoved)
        *notRemoved = filesNotRemoved;

    return filesNotRemoved.isEmpty();
}

bool VcDocProjectNode::deleteFiles(const QStringList &filePaths)
{
    Q_UNUSED(filePaths)
    return false;
}

bool VcDocProjectNode::renameFile(const QString &filePath, const QString &newFilePath)
{
    Q_UNUSED(filePath)
    Q_UNUSED(newFilePath)
    return false;
}

/**
 * @brief VcDocProjectNode::projectDirectory
 * @return absoolute directory path to project file (.vcproj).
 */

QString VcDocProjectNode::projectDirectory() const
{
    QFileInfo fileInfo(path().toString());
    return fileInfo.canonicalPath();
}

void VcDocProjectNode::addFileNode(const QString &filePath)
{
    VcFileNode *fileNode = new VcFileNode(::Utils::FileName(filePath), this);

    if (!appendFileNode(fileNode)) {
        delete fileNode;
    }
}

void VcDocProjectNode::addFileContainerNode(const QString &name, VcFileContainerNode::VcContainerType type)
{
    QString containerType;

    if (type == VcFileContainerNode::VcContainerType_Filter)
        containerType = QLatin1String(Constants::VC_PROJECT_FILE_CONTAINER_FILTER);
    else if (type == VcFileContainerNode::VcContainerType_Folder)
        containerType = QLatin1String(Constants::VC_PROJECT_FILE_CONTAINER_FOLDER);

    if (containerType.isEmpty())
        return;

    VcFileContainerNode *folderNode = new VcFileContainerNode(::Utils::FileName::fromString(name), this);
    folderNode->setDisplayName(name);

    if (!appendFileContainerNode(folderNode)) {
        delete folderNode;
    }
}

bool VcDocProjectNode::appendFileContainerNode(VcFileContainerNode *fileContainerNode)
{
    if (!m_vcProjectFile->visualStudioProject() || !fileContainerNode)
        return false;

    if (m_subFolderNodes.contains(fileContainerNode))
        return false;
    else {
        foreach (ProjectExplorer::FolderNode *fNode, m_subFolderNodes) {
            VcFileContainerNode *vcFileContainerNode = dynamic_cast<VcFileContainerNode *>(fNode);

            if(fNode->displayName() == fileContainerNode->displayName())
                return false;
        }
    }

    IFileContainer *fileContainerModel = m_vcProjectFile->visualStudioProject()->
            projectFactories()->fileFactory()->createFileContainer(fileContainerNode->displayName(), containerType);

    m_vcProjectFile->visualStudioProject()->files()->addFileContainer(fileContainerModel);

    QList<ProjectExplorer::FolderNode *> vcFolderNodes;
    vcFolderNodes << fileContainerNode;
    addFolderNodes(vcFolderNodes);
    m_vcProjectFile->visualStudioProject()->saveToFile(m_vcProjectFile->visualStudioProject()->filePath());
    return true;
}

bool VcDocProjectNode::appendFileNode(VcFileNode *fileNode)
{
    if (m_fileNodes.contains(fileNode))
        return false;
    else {
        foreach (ProjectExplorer::FileNode *fNode, m_fileNodes) {
            if (fNode->path() == fileNode->path())
                return false;
        }
    }

    QString relativeFilePath = VisualStudioUtils::fileRelativePath(m_vcProjectFile->visualStudioProject()->filePath(), filePath);
    IFile *file = m_vcProjectFile->visualStudioProject()->projectFactories()->
            fileFactory()->createFile(relativeFilePath, VisualStudioUtils::getFileType(::Utils::FileName::fromString(filePath)));

    if (!file)
        return;

    file->setRelativePath(relativeFilePath);

    m_vcProjectFile->visualStudioProject()->files()->addFile(file);
    QList<ProjectExplorer::FileNode *> vcFileNodes;
    vcFileNodes << fileNode;
    addFileNodes(vcFileNodes);
    return true;
}

void VcDocProjectNode::removeFileNode(VcFileNode *fileNode)
{
    QString relativePath = VisualStudioUtils::fileRelativePath(m_vcProjectFile->visualStudioProject()->filePath(), fileNode->path());

    QList<ProjectExplorer::FileNode *> fileNodesToRemove;
    fileNodesToRemove << fileNode;
    removeFileNodes(fileNodesToRemove);

    IFile *filePtr = m_vcProjectFile->visualStudioProject()->files()->file(relativePath);

    m_vcProjectFile->visualStudioProject()->files()->removeFile(filePtr);
    delete filePtr;
    m_vcProjectFile->visualStudioProject()->saveToFile(m_vcProjectFile->visualStudioProject()->filePath());
}

void VcDocProjectNode::removeFileContainerNode(VcFileContainerNode *fileContainerNode)
{
    IFileContainer *fileContainer = m_vcProjectFile->visualStudioProject()->files()->findFileContainer(fileContainer->displayName());

    QList<ProjectExplorer::FolderNode *> folderNodesToRemove;
    folderNodesToRemove << fileContainerNode;
    removeFolderNodes(folderNodesToRemove);

    m_vcProjectFile->visualStudioProject()->files()->removeFileContainer(fileContainer);
    m_vcProjectFile->visualStudioProject()->saveToFile(m_vcProjectFile->visualStudioProject()->filePath());
}

void VcDocProjectNode::showSettingsDialog()
{
    //    if (m_vcProjectModel) {
    //        VcProjectDocumentWidget *settingsWidget = static_cast<VcProjectDocumentWidget *>(m_vcProjectModel->createSettingsWidget());

    //        if (settingsWidget) {
    //            settingsWidget->show();
    ////            connect(settingsWidget, SIGNAL(accepted()), this, SIGNAL(settingsDialogAccepted()));
    //        }
    //    }
}

VcFileNode *VcDocProjectNode::findFileNode(const QString &filePath)
{
    VcFileNode *fileNode = nullptr;
    foreach (ProjectExplorer::FileNode *f, fileNodes()) {
        // There can be one match only here!
        if (f->path().toString() == filePath)
            return static_cast<VcFileNode *>(f);
    }

    foreach (ProjectExplorer::FolderNode *folderNode, m_subFolderNodes) {
        VcFileContainerNode *containerNode = static_cast<VcFileContainerNode *>(folderNode);
        fileNode = containerNode->findFileNode(filePath);

        if (fileNode)
            return fileNode;
    }

    return nullptr;
}

} // namespace Internal
} // namespace VcProjectManager
