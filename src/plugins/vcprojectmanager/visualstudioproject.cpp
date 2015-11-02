#include "visualstudioproject.h"

#include "visualstudiosolutionfile.h"
#include "visualstudiosolutionmanager.h"

namespace VcProjectManager {
namespace Internal {

VisualStudioProject::VisualStudioProject(VisualStudioSolutionManager *projectManager, const QString &filePath)
    : m_visualStudioFile(new VisualStudioSolutionFile(filePath))
    , m_projectManager(projectManager)
    , m_filePath(filePath)
{
}

VisualStudioProject::~VisualStudioProject()
{
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
    return nullptr;
}

QStringList VisualStudioProject::files(ProjectExplorer::Project::FilesMode fileMode) const
{
    Q_UNUSED(fileMode)
    return QStringList();
}

} // namespace Internal
} // namespace VcProjectManager

