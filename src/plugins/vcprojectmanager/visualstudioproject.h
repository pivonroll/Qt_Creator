#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECT_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECT_H

#include <projectexplorer/project.h>

namespace VcProjectManager {
namespace Internal {

class VisualStudioSolutionFile;
class VisualStudioSolutionManager;
class VisualStudioSolutionNode;

class VisualStudioProject : public ProjectExplorer::Project
{
public:
    VisualStudioProject(VisualStudioSolutionManager *projectManager, const QString &filePath);
    ~VisualStudioProject();

    // Project interface
    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;

private:
    VisualStudioSolutionFile *m_visualStudioFile;
    VisualStudioSolutionManager *m_projectManager;
    QString m_filePath;
    VisualStudioSolutionNode *m_rootNode;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECT_H
