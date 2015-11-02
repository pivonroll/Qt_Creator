#include "visualstudiosolutionmanager.h"

#include "visualstudioproject.h"

namespace VcProjectManager {
namespace Internal {

VisualStudioSolutionManager::VisualStudioSolutionManager()
{

}

QString VisualStudioSolutionManager::mimeType() const
{
    return QLatin1String("text/vs-solution");
}

ProjectExplorer::Project *VisualStudioSolutionManager::openProject(const QString &filePath, QString *errorString)
{
    Q_UNUSED(errorString)
    return new VisualStudioProject(this, filePath);
}

} // namespace Internal
} // namespace VcProjectManager

