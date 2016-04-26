#ifndef VCPROJECTMANAGER_INTERNAL_VC_PROJ_KIT_MATCHER_H
#define VCPROJECTMANAGER_INTERNAL_VC_PROJ_KIT_MATCHER_H

#include <projectexplorer/kitmanager.h>

namespace ProjectExplorer {
class Kit;
}

namespace VcProjectManager {
namespace Internal {

class VCProjKitMatcher : public ProjectExplorer::KitMatcher
{
public:
    bool matches(const ProjectExplorer::Kit *k) const;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VC_PROJ_KIT_MATCHER_H
