#include "visualstudiosolutionnode.h"

#include "visualstudiosolutionfile.h"
#include "vcdocprojectnodes.h"

namespace VcProjectManager {
namespace Internal {

VisualStudioSolutionNode::VisualStudioSolutionNode(VisualStudioSolutionFile *visualStudioSolutionFile, const Utils::FileName &filePath)
    : ProjectExplorer::ProjectNode(filePath)
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
    QList<IVisualStudioProject *> projects = m_visualStudioSolutionFile->m_visualStudioProjects;

    QList<ProjectExplorer::ProjectNode *> projectNodes;

    foreach (IVisualStudioProject *project, projects) {
        VcDocProjectNode *newVisualStudioProjectNode = new VcDocProjectNode(project);
        projectNodes << newVisualStudioProjectNode;
    }

    addProjectNodes(projectNodes);
}

} // namespace Internal
} // namespace VcProjectManager

