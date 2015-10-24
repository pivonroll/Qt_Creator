#ifndef VCPROJECTMANAGER_INTERNAL_FILEFACTORY_H
#define VCPROJECTMANAGER_INTERNAL_FILEFACTORY_H

#include <visualstudiointerfaces/ifilefactory.h>

namespace VcProjectManager {
namespace Internal {

class IVisualStudioProject;

class FileFactory : public IFileFactory
{
public:
    FileFactory(IVisualStudioProject *project);

    // IFileFactory interface
    IFile *createFile(const QString &relativeFilePath, ProjectExplorer::FileType fileType) const;
    IFileContainer *createFileContainer(const QString &containerPath, const QString &containerType) const;

private:
    IVisualStudioProject *m_project;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_FILEFACTORY_H
