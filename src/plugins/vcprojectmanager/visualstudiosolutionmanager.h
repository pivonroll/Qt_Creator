#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONMANAGER_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONMANAGER_H

#include <projectexplorer/iprojectmanager.h>

namespace VcProjectManager {
namespace Internal {

class VisualStudioSolutionManager : public ProjectExplorer::IProjectManager
{
public:
    VisualStudioSolutionManager();

    QString mimeType() const;
    ProjectExplorer::Project *openProject(const QString &filePath, QString *errorString);
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONMANAGER_H
