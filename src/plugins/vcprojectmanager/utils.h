#ifndef VCPROJECTMANAGER_INTERNAL_UTILS_H
#define VCPROJECTMANAGER_INTERNAL_UTILS_H

#include "vcprojectmodel/vcprojectdocument_constants.h"

#include <projectexplorer/projectnodes.h>
#include <utils/fileutils.h>
#include <QString>

namespace VcProjectManager {
namespace Internal {

class IConfiguration;
class IConfigurationBuildTool;
class IToolSection;

namespace VisualStudioUtils {

VcDocConstants::DocumentVersion getProjectVersion(const QString &projectPath);
ProjectExplorer::FileType getFileType(const ::Utils::FileName &canonicalFilePath);
QString fileRelativePath(const QString &topPath, const QString &innerPath);

void leaveOnlyCppTool(IConfiguration *config);
void cleanUpConfigAttributes(IConfiguration *config, IConfiguration *projectConfig);
void cleanUpConfigTools(IConfiguration *config, IConfiguration *projectConfig);
void cleanUpConfigTool(IConfigurationBuildTool *tool, IConfigurationBuildTool *projTool);
void cleanUpConfigToolSection(IToolSection *toolSection, IToolSection *projToolSection);

} // namespace Utils
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_UTILS_H
