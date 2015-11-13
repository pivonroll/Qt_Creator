#include "vcprojkitmatcher.h"

#include "msbuildversionmanager.h"
#include "vcprojectkitinformation.h"

#include <projectexplorer/kit.h>
#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {

bool VCProjKitMatcher::matches(const ProjectExplorer::Kit *k) const
{
    MsBuildInformation *msBuild = VcProjectKitInformation::msBuildInfo(k);
    QTC_ASSERT(msBuild, return false);
    return true;
}

} // namespace Internal
} // namespace VcProjectManager

