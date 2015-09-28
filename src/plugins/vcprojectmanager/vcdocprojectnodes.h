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
#ifndef VCPROJECTMANAGER_INTERNAL_VCDOCPROJECTNODE_H
#define VCPROJECTMANAGER_INTERNAL_VCDOCPROJECTNODE_H

#include <projectexplorer/projectnodes.h>

namespace VcProjectManager {
namespace Internal {

class IVisualStudioProject;
class IFileContainer;
class IFile;
class VcDocProjectNode;

QString fileRelativePath(const QString &fullProjectPath, const QString &fullFilePath);

class VcFileNode : public ProjectExplorer::FileNode
{
    friend class VcDocProjectNode;
    friend class VcFileContainerNode;

public:
    VcFileNode(IFile *fileModel, VcDocProjectNode *vcDocProject);
    ~VcFileNode();

    /*!
     * \brief Shows a settings widget for a file node.
     */
    void showSettingsWidget();

protected:
    /*!
     * \brief Does nothing.
     */
    void readChildren(VcDocProjectNode *vcDocProj);

signals:
    /*!
     * \brief Emitted when settings dialog is accepted.
     */
    void settingsDialogAccepted();

private:
    IFile *m_vcFileModel;
};

class VcFileContainerNode : public ProjectExplorer::FolderNode
{
    friend class VcFileNode;
    friend class VcDocProjectNode;

public:
    enum VcContainerType {
        VcContainerType_Filter,
        VcContainerType_Folder
    };

    VcFileContainerNode(IFileContainer *fileContainerModel, VcDocProjectNode *vcDocProjNode);
    ~VcFileContainerNode();

    bool showInSimpleTree() const;

    /*!
     * \return a type of container.
     */
    VcContainerType containerType() const;

    /*!
     * \brief Adds a file node to this file container.
     */
//    void addFileNode(const QString &filePath);

    /*!
     * \brief Appends a file node to the list of file nodes in this container.
     * \return \b true if adding of a file node is successful,
     * otherwise it returns \b false.
     */
    bool appendFileNode(VcFileNode *fileNode);

    /*!
     * \brief Adds a file container node to this file container.
     */
//    void addFileContainerNode(const QString &name, VcContainerType type = VcContainerType_Filter);

    /*!
     * \brief Appends a file container node to the list of file container
     * nodes in this container.
     * \return \b true if adding of a file container node is successful,
     * otherwise it returns \b false.
     */
    bool appendFileContainerNode(VcFileContainerNode *fileContainer);

    /*!
     * \brief Removes a file container node from the list of file container nodes.
     */
    void removeFileContainerNode(VcFileContainerNode *filterNode);

    /*!
     * \brief Removes a file node from the list of file nodes.
     */
    void removeFileNode(VcFileNode *fileNode);

    /*!
     * \return a file node which file path is \a filePath.
     */
    VcFileNode *findFileNode(const QString &filePath);
protected:
    /*!
     * \brief Reads files and file containers from project's file container
     * and creates and then adds file or file container nodes to this file container node.
     */
    void readChildren();

private:
    IFileContainer *m_vcFileContainerModel;
    VcContainerType m_vcContainerType;
    VcDocProjectNode *m_parentVcDocProjNode;
};

class VcDocProjectNode : public ProjectExplorer::ProjectNode
{
    friend class VcFileNode;
    friend class VcFileContainerNode;

public:
    VcDocProjectNode(IVisualStudioProject* vcProjectModel);
    ~VcDocProjectNode();

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

    /*!
     * \return a canonical path to the directory where .vcproj of .vcprojx project is.
     */
    QString projectDirectory() const;

//    void addFileNode(const QString &filePath);
//    void addFileContainerNode(const QString &name, VcFileContainerNode::VcContainerType type);
    bool appendFileContainerNode(VcFileContainerNode *fileContainerNode);
    bool appendFileNode(VcFileNode *fileNode);
    void removeFileNode(VcFileNode *fileNode);
    void removeFileContainerNode(VcFileContainerNode *fileContainerNode);
    void showSettingsDialog();

signals:
    void settingsDialogAccepted();

private:
    VcFileNode *findFileNode(const QString &filePath);

    IVisualStudioProject *m_vcProjectModel;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VCDOCPROJECTNODE_H
