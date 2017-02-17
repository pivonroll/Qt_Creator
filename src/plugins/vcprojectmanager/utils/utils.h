/**************************************************************************
**
** Copyright (c) 2016 Bojan Petrovic
** Copyright (c) 2016 Radovan Zivkovic
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/
#ifndef VCPROJECTMANAGER_INTERNAL_VISUAL_STUDIO_UTILS_H
#define VCPROJECTMANAGER_INTERNAL_VISUAL_STUDIO_UTILS_H

#include "vcprojectmodel/vcprojectdocument_constants.h"
#include "../common/projectconstants.h"

#include <projectexplorer/projectnodes.h>
#include <utils/fileutils.h>
#include <utils/qtcassert.h>
#include <QString>

namespace VcProjectManager {
namespace Internal {

class IConfiguration;
class IConfigurationBuildTool;
class IToolSection;

namespace VisualStudioUtils {

DocumentVersion getProjectVersion(const QString &projectPath);
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

#endif // VCPROJECTMANAGER_INTERNAL_VISUAL_STUDIO_UTILS_H
