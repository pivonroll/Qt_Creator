#ifndef VCPROJECTMANAGER_INTERNAL_UTILS_H
#define VCPROJECTMANAGER_INTERNAL_UTILS_H

#include "vcprojectmodel/vcprojectdocument_constants.h"

#include <QString>

namespace VcProjectManager {
namespace Internal {
namespace Utils {

VcDocConstants::DocumentVersion getProjectVersion(const QString &projectPath);

} // namespace Utils
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_UTILS_H
