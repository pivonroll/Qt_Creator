#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONNODE_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONNODE_H

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
    bool addSubProjects(const QStringList &proFilePaths) final;
    bool removeSubProjects(const QStringList &proFilePaths) final;

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

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONNODE_H
