#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECTFACTORIESX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECTFACTORIESX_H

#include <visualstudiointerfaces/iprojectfactories.h>

namespace VcProjectManager {
namespace Internal {

class VcProjectDocumentX;

namespace VisualStudioProjectX {

class FileFactoryX;

class ProjectFactoriesX : public IProjectFactories
{
public:
    ProjectFactoriesX(VcProjectDocumentX *parentProject);
    ~ProjectFactoriesX();

    // IProjectFactories interface
    IFileFactory *fileFactory() const;

private:
    FileFactoryX *m_fileFactory;
    VcProjectDocumentX *m_parentProject;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECTFACTORIESX_H
