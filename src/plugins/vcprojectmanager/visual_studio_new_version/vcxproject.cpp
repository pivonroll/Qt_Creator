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
#include "vcxproject.h"

#include "../vcdocprojectnodes.h"
#include "../vcprojectfile.h"
#include "../vcprojectbuildconfiguration.h"
#include "../vcprojectkitinformation.h"

#include <vcprojectxmodel/tools/tool_constantsx.h>

#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/iconfigurationcontainer.h>
#include <visualstudiointerfaces/iconfigurations.h>
#include <visualstudiointerfaces/iconfiguration.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/isectioncontainer.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include <cpptools/cppmodelmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/headerpath.h>
#include <projectexplorer/target.h>

#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {

VcXProject::VcXProject(const Utils::FileName &fileName)
{
    m_projectFile = new VcProjectFile(fileName.toString(), DocumentVersion::DV_MSVC_2010);
    m_rootNode = m_projectFile->createProjectNode();
}

VcXProject::~VcXProject()
{
    m_codeModelFuture.cancel();
    VcDocProjectNode *temp = m_rootNode;
    m_rootNode = nullptr;
    delete temp;
    delete m_projectFile;
}

QString VcXProject::displayName() const
{
    if (m_rootNode)
        return m_rootNode->displayName();

    return QString();
}

Core::IDocument *VcXProject::document() const
{
    return m_projectFile;
}

ProjectExplorer::ProjectNode *VcXProject::rootProjectNode() const
{
    return m_rootNode;
}

QStringList VcXProject::files(ProjectExplorer::Project::FilesMode fileMode) const
{
    Q_UNUSED(fileMode)
    return QStringList();
}

ProjectExplorer::Project::RestoreResult VcXProject::fromMap(const QVariantMap &map, QString *errorMessage)
{
    ProjectExplorer::Project::RestoreResult result = Project::fromMap(map, errorMessage);

    if (result == Project::RestoreResult::Ok || needsConfiguration())
        importBuildConfigurations();

    if (result != ProjectExplorer::Project::RestoreResult::Ok)
        return result;

    updateCodeModels();
    return result;
}

/*!
 * \brief Imports build configurations available for the selected build toolkit (reffered to as Kit).
 *
 */
void VcXProject::importBuildConfigurations()
{
    ProjectExplorer::Kit *kit = ProjectExplorer::KitManager::defaultKit();

    removeTarget(target(kit));
    addTarget(createTarget(kit));
    if (!activeTarget() && kit)
        addTarget(createTarget(kit));
}

void VcXProject::addCxxModelFiles(const ProjectExplorer::FolderNode *node, QSet<QString> &projectFiles)
{
    foreach (const ProjectExplorer::FileNode *file, node->fileNodes()) {
        if (file->fileType() == ProjectExplorer::FileType::Header || file->fileType() == ProjectExplorer::FileType::Source)
            projectFiles << file->filePath().toString();
    }
    foreach (const ProjectExplorer::FolderNode *subfolder, node->folderNodes())
        addCxxModelFiles(subfolder, projectFiles);
}

bool VcXProject::matchesKit(const ProjectExplorer::Kit *k)
{
    MsBuildInformation *msBuildInfo = VcProjectKitInformation::msBuildInfo(k);
    QTC_ASSERT(msBuildInfo && msBuildInfo->m_msBuildVersion > MsBuildInformation::MSBUILD_V_4_0, return false);
    return true;
}

void VcXProject::updateCodeModels()
{
    ProjectExplorer::Kit *k = activeTarget() ? activeTarget()->kit() : ProjectExplorer::KitManager::defaultKit();
    QTC_ASSERT(k, return);
    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(k, ProjectExplorer::Constants::CXX_LANGUAGE_ID);
    QTC_ASSERT(tc, return);
    CppTools::CppModelManager *modelmanager = CppTools::CppModelManager::instance();
    QTC_ASSERT(modelmanager, return);
    CppTools::ProjectInfo pinfo = modelmanager->projectInfo(this);

    CppTools::ProjectPart::Ptr pPart(new CppTools::ProjectPart());

    ProjectExplorer::BuildConfiguration *bc = activeTarget() ? activeTarget()->activeBuildConfiguration() : NULL;
    if (bc) {
        VcProjectBuildConfiguration *vbc = qobject_cast<VcProjectBuildConfiguration*>(bc);
        QTC_ASSERT(vbc, return);

        QString configName = vbc->displayName();
        IConfiguration *configModel = m_projectFile->visualStudioProject()->configurations()->configurationContainer()->configuration(configName);

        if (configModel) {
            IConfigurationBuildTool *configTool = configModel->tools()->configurationBuildTools()->tool(QLatin1String(ToolConstantsx::TOOL_CL_COMPILE));
            if (configTool) {
                for (int i = 0; i < configTool->sectionContainer()->sectionCount(); ++i) {
                    IToolSection *toolSection = configTool->sectionContainer()->section(i);

                    QTC_ASSERT(toolSection, continue);

                    IToolAttribute *toolAttr = toolSection->attributeContainer()->toolAttribute(QLatin1String("PreprocessorDefinitions"));

                    if (toolAttr) {
                        toolAttr->descriptionDataItem();
                        QStringList preprocDefs = toolAttr->value().split(toolAttr->descriptionDataItem()->optionalValue(QLatin1String("separator")));

                        pPart->projectDefines += preprocDefs.join(QLatin1String("\n")).toLatin1();
                    }
                }
            }
        }
    }

    // VS 2005-2008 has poor c++11 support, see http://wiki.apache.org/stdcxx/C%2B%2B0xCompilerSupport
    pPart->languageVersion = CppTools::ProjectPart::CXX11;
    pPart->qtVersion = CppTools::ProjectPart::NoQt;
    pPart->projectDefines += tc->predefinedMacros(QStringList());

    QStringList cxxFlags;
    foreach (const ProjectExplorer::HeaderPath &path, tc->systemHeaderPaths(cxxFlags, Utils::FileName())) {
        if (path.kind() != ProjectExplorer::HeaderPath::FrameworkHeaderPath)
            pPart->headerPaths << CppTools::ProjectPartHeaderPath(path.path(), CppTools::ProjectPartHeaderPath::FrameworkPath);
    }
    QSet<QString> files;
    addCxxModelFiles(m_rootNode, files);

    foreach (const QString &file, files)
        pPart->files << CppTools::ProjectFile(file, CppTools::ProjectFile::CXXSource);

    if (!pPart->files.isEmpty())
        pinfo.appendProjectPart(pPart);

    modelmanager->updateProjectInfo(pinfo);
    m_codeModelFuture = modelmanager->updateSourceFiles(files);
    setProjectLanguage(ProjectExplorer::Constants::CXX_LANGUAGE_ID, !pPart->files.isEmpty());
}

} // namespace Internal
} // namespace VcProjectManager

