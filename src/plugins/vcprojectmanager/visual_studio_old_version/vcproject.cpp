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
#include "vcproject.h"

#include "../common/vcprojectmanagerconstants.h"
#include "../ms_build/vcmakestep.h"
#include "../vcprojectbuildconfiguration.h"
#include "../vcdocprojectnodes.h"
#include "../vcprojectfile.h"
#include "../vcprojectkitinformation.h"
#include "vcprojectmanager.h"

#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/iconfigurationcontainer.h>
#include <visualstudiointerfaces/iconfigurations.h>
#include <visualstudiointerfaces/iconfiguration.h>
#include <visualstudiointerfaces/ifile.h>
#include <visualstudiointerfaces/ifilecontainer.h>
#include <visualstudiointerfaces/ifiles.h>
#include <visualstudiointerfaces/isectioncontainer.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include "vcprojectmodel/tools/tool_constants.h"
#include "vcprojectmodel/vcdocumentmodel.h"

#include <coreplugin/icontext.h>
#include <cpptools/cppmodelmanager.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/headerpath.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <projectexplorer/kitmanager.h>

#include <utils/qtcassert.h>

#include <QFileInfo>
#include <QFormLayout>
#include <QLabel>

/*!
 * \class VcProject
 * This class represents a Visual Studio's project implementation of \interface ProjectExplorer::Project.
 */

namespace VcProjectManager {
namespace Internal {

VcProject::VcProject(VcProjectManager *projectManager, const QString &projectFilePath, DocumentVersion docVersion)
    : m_projectManager(projectManager)
    , m_projectFile(new VcProjectFile(projectFilePath, docVersion))
{
    if (m_projectFile->visualStudioProject()->documentVersion() == DV_MSVC_2005)
        setProjectContext(Core::Context(Constants::VC_PROJECT_2005_ID));
    else
        setProjectContext(Core::Context(Constants::VC_PROJECT_ID));

    m_rootNode = m_projectFile->createProjectNode();

    if (m_projectFile->visualStudioProject()->documentVersion() != DV_MSVC_2005)
        setId(Core::Id(Constants::VC_PROJECT_ID));
    else
        setId(Core::Id(Constants::VC_PROJECT_2005_ID));
}

VcProject::~VcProject()
{
    m_codeModelFuture.cancel();

    delete m_rootNode;
    delete m_projectFile;
}

QString VcProject::displayName() const
{
    QTC_ASSERT(m_rootNode, return QString());
    return m_rootNode->displayName();
}

Core::IDocument *VcProject::document() const
{
    return m_projectFile;
}

ProjectExplorer::IProjectManager *VcProject::projectManager() const
{
    return m_projectManager;
}

ProjectExplorer::ProjectNode *VcProject::rootProjectNode() const
{
    return m_rootNode;
}

QStringList VcProject::files(Project::FilesMode fileMode) const
{
    Q_UNUSED(fileMode);
    // TODO: respect the mode
    QStringList sl;
    if (m_projectFile && m_projectFile->visualStudioProject())
        allProjectFile(sl);

    return sl;
}

bool VcProject::needsConfiguration() const
{
    return targets().isEmpty() || !activeTarget() || activeTarget()->buildConfigurations().isEmpty();
}

bool VcProject::supportsKit(ProjectExplorer::Kit *k, QString *errorMessage) const
{
    Q_UNUSED(k)

    if (errorMessage)
        *errorMessage = tr("Kit toolchain does not support MSVC 2003, 2005 or 2008 ABI");
    return false;
}

void VcProject::reloadProjectNodes()
{
    delete m_rootNode;
    m_projectFile->reloadVcDoc();
    m_rootNode = m_projectFile->createProjectNode();

    updateCodeModels();

    emit fileListChanged();
}

/*!
 * \brief Updates build configurations for targers in Qt Creator's build system.
 *
 * When build configuration edit dialog for Visual Studio project is closed this
 * function is executed in order to apply new build configuration settings to a project.
 * It first removes all build configurations that are deleted from the project and then
 * it inserts the new ones into the appropriate build targets.
 *
 * \note All changes done to build configurations in edit dialog are first saved to
 * in-memory tree representation of Visual Studio project. This is done just after we
 * click on Ok button in edit dialog. The only thing left is to serialize content of
 * in-memory tree representation for Visual Studio project to an actual file on the hard drive.
 * After that we update Qt Creator's available build targets.
 */
void VcProject::onSettingsDialogAccepted()
{
    m_projectFile->visualStudioProject()->saveToFile(m_projectFile->filePath().toString());
    IConfigurations *configs = m_projectFile->visualStudioProject()->configurations();

    QTC_ASSERT(configs, return);

    QList<ProjectExplorer::Target *> targetList = targets();

    // remove all deleted configurations
    foreach (ProjectExplorer::Target *target, targetList) {
        QTC_ASSERT(target, continue);

        QList<ProjectExplorer::BuildConfiguration *> buildConfigurationList = target->buildConfigurations();

        foreach (ProjectExplorer::BuildConfiguration *bc, buildConfigurationList) {
            VcProjectBuildConfiguration *vcProjectBuildConfiguration = qobject_cast<VcProjectBuildConfiguration *>(bc);

            QTC_ASSERT(vcProjectBuildConfiguration, continue);

            IConfiguration *lookFor = configs->configurationContainer()->configuration(vcProjectBuildConfiguration->displayName());
            if (!lookFor)
                target->removeBuildConfiguration(vcProjectBuildConfiguration);
        }
    }

    // add all new build configurations
    foreach (ProjectExplorer::Target *target, targetList) {
        QTC_ASSERT(target, continue);
        target->updateDefaultBuildConfigurations();
        target->updateDefaultDeployConfigurations();
    }
}

ProjectExplorer::Project::RestoreResult VcProject::fromMap(const QVariantMap &map, QString *errorMessage)
{
    ProjectExplorer::Project::RestoreResult result = ProjectExplorer::Project::fromMap(map, errorMessage);

    if (result == ProjectExplorer::Project::RestoreResult::Ok || needsConfiguration())
        importBuildConfigurations();

    if (result != ProjectExplorer::Project::RestoreResult::Ok)
        return result;

    updateCodeModels();
    return result;
}

/**
 * \brief Visit folder node recursive and accumulate Source and Header files
 */
void VcProject::addCxxModelFiles(const ProjectExplorer::FolderNode *node, QSet<QString> &projectFiles)
{
    foreach (const ProjectExplorer::FileNode *file, node->fileNodes()) {
        if (file->fileType() == ProjectExplorer::FileType::Header || file->fileType() == ProjectExplorer::FileType::Source)
            projectFiles << file->filePath().toString();
    }
    foreach (const ProjectExplorer::FolderNode *subfolder, node->folderNodes())
        addCxxModelFiles(subfolder, projectFiles);
}

bool VcProject::matchesKit(const ProjectExplorer::Kit *k)
{
    MsBuildInformation *msBuildInfo = VcProjectKitInformation::msBuildInfo(k);
    QTC_ASSERT(msBuildInfo && msBuildInfo->m_msBuildVersion <= MsBuildInformation::MSBUILD_V_4_0, return false);
    return true;
}

/**
 * \brief Update editor Code Models
 *
 * Because only language with Code Model in QtCreator and support in VS is C++,
 * this method updates C++ code model.
 * VCProj doesn't support Qt, ObjectiveC and always uses c++11.
 *
 * \note Method should pass some flags for ClangCodeModel plugin: "-fms-extensions"
 * and "-fdelayed-template-parsing", but no interface exists at this moment.
 */
void VcProject::updateCodeModels()
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
            IConfigurationBuildTool *configTool = configModel->tools()->configurationBuildTools()->tool(QLatin1String(ToolConstants::strVCCLCompilerTool));
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
    pPart->languageVersion = CppTools::ProjectPart::CXX98;
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

/*!
 * \brief Imports build configurations available for the selected build toolkit (reffered to as Kit).
 *
 */
void VcProject::importBuildConfigurations()
{
}

/*!
 * \brief builds a list of of available files in the project.
 * List is contained in \a allFiles, and contains canonical file paths to files.
 */
void VcProject::allProjectFile(QStringList &allFiles) const
{
    if (m_projectFile && m_projectFile->visualStudioProject()) {
        for (int i = 0; i < m_projectFile->visualStudioProject()->files()->fileContainerCount(); ++i) {
            IFileContainer *fileContainer = m_projectFile->visualStudioProject()->files()->fileContainer(i);
            fileContainer->allFiles(allFiles);
        }

        for (int i = 0; i < m_projectFile->visualStudioProject()->files()->fileCount(); ++i) {
            IFile *file = m_projectFile->visualStudioProject()->files()->file(i);
            allFiles.append(file->canonicalPath());
        }
    }
}

VcProjectBuildSettingsWidget::VcProjectBuildSettingsWidget()
{
    QFormLayout *f1 = new QFormLayout(this);
    f1->setContentsMargins(0, 0, 0, 0);
    f1->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    QLabel *l = new QLabel(tr("Vcproj Build Configuration widget."));
    f1->addRow(tr("Vc Project"), l);
}

QString VcProjectBuildSettingsWidget::displayName() const
{
    return tr("Vc Project Settings Widget");
}

} // namespace Internal
} // namespace VcProjectManager
