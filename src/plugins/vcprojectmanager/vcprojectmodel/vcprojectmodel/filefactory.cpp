#include "filefactory.h"

#include "file.h"

#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {

FileFactory::FileFactory(IVisualStudioProject *project)
    : m_project(project)
{

}

IFile *FileFactory::createFile(const QString &relativeFilePath, ProjectExplorer::FileType fileType) const
{
    Q_UNUSED(fileType)

    QTC_ASSERT(m_project != nullptr, return nullptr);
    File *newFile = new File(m_project);
    newFile->setRelativePath(relativeFilePath);
    return newFile;
}

IFileContainer *FileFactory::createFileContainer(const QString &containerPath, const QString &containerType) const
{
    Q_UNUSED(containerPath)
    Q_UNUSED(containerType)
    return nullptr;
}

} // namespace Internal
} // namespace VcProjectManager

