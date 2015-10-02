#ifndef VCPROJECTMANAGER_INTERNAL_IPROJECTFACTORIES
#define VCPROJECTMANAGER_INTERNAL_IPROJECTFACTORIES

namespace VcProjectManager {
namespace Internal {

class IFileFactory;

class IProjectFactories
{
public:
    virtual ~IProjectFactories() {}
    virtual IFileFactory *fileFactory() const = 0;
};

} // namespace Internal
} // namespace VcProjectManager
#endif // VCPROJECTMANAGER_INTERNAL_IPROJECTFACTORIES

