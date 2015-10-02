#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILEFACTORY_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILEFACTORY_H

#include "../../interfaces/ifilefactory.h"

namespace VcProjectManager {
namespace Internal {

class VcProjectDocumentX;

namespace VisualStudioProjectX {

class Project;

class FileFactoryX : public IFileFactory
{
public:
    FileFactoryX(VcProjectDocumentX *parentProjectDocument);

    // IFileFactory interface
    IFile *createFile(const QString &relativePath, ProjectExplorer::FileType fileType) const;
    IFileContainer *createFileContainer(const QString &containerPath, const QString &containerType) const;

private:
    VcProjectDocumentX *m_parentProjectDocument;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILEFACTORY_H
