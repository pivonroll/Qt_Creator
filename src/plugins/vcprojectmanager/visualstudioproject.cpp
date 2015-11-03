#include "visualstudioproject.h"

#include "visualstudiosolutionfile.h"
#include "visualstudiosolutionmanager.h"
#include "visualstudiosolutionnode.h"

namespace VcProjectManager {
namespace Internal {

VisualStudioProject::VisualStudioProject(VisualStudioSolutionManager *projectManager, const QString &filePath)
    : m_visualStudioFile(new VisualStudioSolutionFile(filePath))
    , m_projectManager(projectManager)
    , m_filePath(filePath)
{
    m_rootNode = m_visualStudioFile->createSolutionNode();
}

VisualStudioProject::~VisualStudioProject()
{
    VisualStudioSolutionNode *temp = m_rootNode;
    m_rootNode = nullptr;
    delete temp;
    delete m_visualStudioFile;
}

QString VisualStudioProject::displayName() const
{
    return QFileInfo(m_filePath).fileName();
}

Core::IDocument *VisualStudioProject::document() const
{
    return m_visualStudioFile;
}

ProjectExplorer::IProjectManager *VisualStudioProject::projectManager() const
{
    return m_projectManager;
}

ProjectExplorer::ProjectNode *VisualStudioProject::rootProjectNode() const
{
    return m_rootNode;
}

QStringList VisualStudioProject::files(ProjectExplorer::Project::FilesMode fileMode) const
{
    Q_UNUSED(fileMode)
    return QStringList();
}

} // namespace Internal
} // namespace VcProjectManager

