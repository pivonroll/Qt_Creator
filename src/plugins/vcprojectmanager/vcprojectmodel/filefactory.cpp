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
    QTC_ASSERT(m_project != nullptr, return 0);
    File *newFile = new File(m_project);
    newFile->setRelativePath(relativeFilePath);
    return newFile;
}

IFileContainer *FileFactory::createFileContainer(const QString &containerPath, const QString &containerType) const
{
    return 0;
}

} // namespace Internal
} // namespace VcProjectManager

