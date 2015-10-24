#ifndef VCPROJECTMANAGER_INTERNAL_PROJECTFACTORIES_H
#define VCPROJECTMANAGER_INTERNAL_PROJECTFACTORIES_H

#include <visualstudiointerfaces/iprojectfactories.h>

namespace VcProjectManager {
namespace Internal {

class ProjectFactories : public IProjectFactories
{
public:
    ProjectFactories();

    // IProjectFactories interface
    IFileFactory *fileFactory() const;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_PROJECTFACTORIES_H
