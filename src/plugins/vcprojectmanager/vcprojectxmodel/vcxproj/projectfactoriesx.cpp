#include "projectfactoriesx.h"

#include "filefactoryx.h"

namespace VcProjectManager {
namespace Internal { 
namespace VisualStudioProjectX {

ProjectFactoriesX::ProjectFactoriesX(VcProjectDocumentX *parentProject)
    : m_parentProject(parentProject)
{
    m_fileFactory = new FileFactoryX(m_parentProject);
}

ProjectFactoriesX::~ProjectFactoriesX()
{
    delete m_fileFactory;
}

IFileFactory *ProjectFactoriesX::fileFactory() const
{
    return m_fileFactory;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

