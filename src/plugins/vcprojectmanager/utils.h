#ifndef VCPROJECTMANAGER_INTERNAL_UTILS_H
#define VCPROJECTMANAGER_INTERNAL_UTILS_H

#include "vcprojectmodel/vcprojectdocument_constants.h"

#include <projectexplorer/projectnodes.h>
#include <utils/fileutils.h>
#include <QString>

namespace VcProjectManager {
namespace Internal {
namespace Utils {

VcDocConstants::DocumentVersion getProjectVersion(const QString &projectPath);
ProjectExplorer::FileType getFileType(const ::Utils::FileName &canonicalFilePath);
QString fileRelativePath(const QString &topPath, const QString &innerPath);

} // namespace Utils
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_UTILS_H
