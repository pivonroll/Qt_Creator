#include "projectfactories.h"

namespace VcProjectManager {
namespace Internal {

ProjectFactories::ProjectFactories()
{

}

IFileFactory *ProjectFactories::fileFactory() const
{
    return nullptr;
}

} // namespace Internal
} // namespace VcProjectManager

