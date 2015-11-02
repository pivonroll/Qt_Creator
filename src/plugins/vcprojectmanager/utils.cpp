#include "utils.h"

#include "vcprojectmanager/vcprojectmodel/tools/tool_constants.h"
#include "vcprojectxmodel/vcxproj/tools/tool_constantsx.h"

#include "vcschemamanager.h"

#include <visualstudiointerfaces/iattributecontainer.h>
#include <visualstudiointerfaces/iattributedescriptiondataitem.h>
#include <visualstudiointerfaces/iconfiguration.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itooldescription.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/itoolsectiondescription.h>
#include <visualstudiointerfaces/isectioncontainer.h>

#include <projectexplorer/projectexplorerconstants.h>
#include <utils/mimetypes/mimedatabase.h>

#include <utils/qtcassert.h>

#include <QFile>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QFileInfo>
#include <QDir>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioUtils {

/*!
 * \return \b true if Visual Studio project at \a filePath is a Visual Studio 2003 project.
 * Otherwise it returns \b false.
 */
bool checkIfVersion2003(const QString &filePath)
{
    VcSchemaManager *schemaMgr = VcSchemaManager::instance();
    QString vc2003Schema = schemaMgr->documentSchema(Constants::SV_2003);

    if (vc2003Schema.isEmpty())
        return false;

    QFile schemaFile(vc2003Schema);
    schemaFile.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
            return true;
    }

    return false;
}

/*!
 * \return \b true if Visual Studio project at \a filePath is a Visual Studio 2005 project.
 * Otherwise it returns \b false.
 */
bool checkIfVersion2005(const QString &filePath)
{
    VcSchemaManager *schemaMgr = VcSchemaManager::instance();
    QString vc2005Schema = schemaMgr->documentSchema(Constants::SV_2005);
    if (vc2005Schema.isEmpty())
        return false;

    QFile schemaFile(vc2005Schema);
    schemaFile.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
            return true;
    }

    return false;
}

/*!
 * \return \b true if Visual Studio project at \a filePath is a Visual Studio 2008 project.
 * Otherwise it returns \b false.
 */
bool checkIfVersion2008(const QString &filePath)
{
    VcSchemaManager *schemaMgr = VcSchemaManager::instance();
    QString vc2008Schema = schemaMgr->documentSchema(Constants::SV_2008);
    if (vc2008Schema.isEmpty())
        return false;

    QFile schemaFile(vc2008Schema);
    schemaFile.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
            return true;
    }

    return false;
}

/*!
 * \return version of Visual Studio Project. Project is passed as the \a projectPath.
 */
VcDocConstants::DocumentVersion getProjectVersion(const QString &projectPath)
{
    if (checkIfVersion2003(projectPath))
        return VcDocConstants::DV_MSVC_2003;
    else if (checkIfVersion2005(projectPath))
        return VcDocConstants::DV_MSVC_2005;
    else if (checkIfVersion2008(projectPath))
        return VcDocConstants::DV_MSVC_2008;

    return VcDocConstants::DV_UNRECOGNIZED;
}

ProjectExplorer::FileType getFileType(const ::Utils::FileName &canonicalFilePath)
{
    ::Utils::MimeDatabase mdb;
    QString mimeType = mdb.mimeTypeForFile(canonicalFilePath.toString()).name();

    if (mimeType == QLatin1String(ProjectExplorer::Constants::CPP_SOURCE_MIMETYPE)
            || mimeType == QLatin1String(ProjectExplorer::Constants::C_SOURCE_MIMETYPE))
        return ProjectExplorer::SourceType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::CPP_HEADER_MIMETYPE)
            || mimeType == QLatin1String(ProjectExplorer::Constants::C_HEADER_MIMETYPE))
        return ProjectExplorer::HeaderType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::RESOURCE_MIMETYPE))
        return ProjectExplorer::ResourceType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::FORM_MIMETYPE))
        return ProjectExplorer::FormType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::QML_MIMETYPE))
        return ProjectExplorer::QMLType;

    return ProjectExplorer::UnknownFileType;
}

QString fileRelativePath(const QString &topPath, const QString &innerPath)
{
    QString relativePath = QFileInfo(topPath).absoluteDir().relativeFilePath(innerPath).replace(QLatin1String("/"), QLatin1String("\\"));

    if (!relativePath.startsWith(QLatin1String("..")))
        relativePath.prepend(QLatin1String(".\\"));

    return relativePath;
}

void leaveOnlyCppTool(IConfiguration *config)
{
    QTC_ASSERT(config && config->tools() && config->tools()->configurationBuildTools(), return);

    int i = 0;
    while (config->tools()->configurationBuildTools()->toolCount() > 1) {
        IConfigurationBuildTool *tool = config->tools()->configurationBuildTools()->tool(i);

        if ((config->version() == IConfiguration::ConfigurationVersion_pre2010
                && tool->toolDescription()->toolKey() != QLatin1String(ToolConstants::strVCCLCompilerTool))
            || (config->version() == IConfiguration::ConfigurationVersion_pre2010
                && tool->toolDescription()->toolKey() != QLatin1String(ToolConstantsx::TOOL_CL_COMPILE))) {
            config->tools()->configurationBuildTools()->removeTool(tool);
            delete tool;
        }

        else
            ++i;
    }
}

void cleanUpConfigAttributes(IConfiguration *config, IConfiguration *projectConfig)
{
    QTC_ASSERT(config && projectConfig, return);

    IAttributeContainer *configAttrCont = config->attributeContainer();
    IAttributeContainer *projConfigAttrCont = projectConfig->attributeContainer();

    QTC_ASSERT(configAttrCont && projConfigAttrCont, return);

    for (int i = 0; i < configAttrCont->getAttributeCount();) {
        QString attrName = configAttrCont->getAttributeName(i);

        if (configAttrCont->attributeValue(attrName) == projConfigAttrCont->attributeValue(attrName))
            configAttrCont->removeAttribute(attrName);
        else
            ++i;
    }
}

void cleanUpConfigTools(IConfiguration *config, IConfiguration *projectConfig)
{
    QTC_ASSERT(config && projectConfig, return);

    ITools *tools = config->tools();
    ITools *projectTools = projectConfig->tools();

    QTC_ASSERT(tools && projectTools && tools->configurationBuildTools() && projectTools->configurationBuildTools(), return);

    IConfigurationBuildTool *tool = tools->configurationBuildTools()->tool(0);

    if (tool->toolDescription()) {
        IConfigurationBuildTool *projTool = projectTools->configurationBuildTools()->tool(tool->toolDescription()->toolKey());

        if (projTool)
            cleanUpConfigTool(tool, projTool);

        if (tool->allAttributesAreDefault())
            tools->configurationBuildTools()->removeTool(tool);
    }
}

void cleanUpConfigTool(IConfigurationBuildTool *tool, IConfigurationBuildTool *projTool)
{
    QTC_ASSERT(tool && projTool, return);
    QTC_ASSERT(tool->sectionContainer() && projTool->sectionContainer(), return);

    ISectionContainer *secCont = tool->sectionContainer();
    ISectionContainer *projSecCont = projTool->sectionContainer();

    for (int i = 0; i < secCont->sectionCount(); ++i) {
        IToolSection *section = secCont->section(i);

        if (section && section->sectionDescription()) {
            IToolSection *projSection = projSecCont->section(section->sectionDescription()->displayName());
            cleanUpConfigToolSection(section, projSection);
        }
    }
}

void cleanUpConfigToolSection(IToolSection *toolSection, IToolSection *projToolSection)
{
    QTC_ASSERT(toolSection && projToolSection, return);

    IToolAttributeContainer *attrCont = toolSection->attributeContainer();
    IToolAttributeContainer *projAttrCont = projToolSection->attributeContainer();

    QTC_ASSERT(attrCont && projAttrCont, return);

    for (int i = 0; i < attrCont->toolAttributeCount();) {
        IToolAttribute *attr = attrCont->toolAttribute(i);

        if (attr && attr->descriptionDataItem()) {
            IToolAttribute *projAttr = projAttrCont->toolAttribute(attr->descriptionDataItem()->key());

            if (projAttr && attr->value() == projAttr->value())
                attrCont->removeToolAttribute(attr);
            else
                ++i;
        }
        else
            ++i;
    }
}

} // namespace VisualStudioUtils
} // namespace Internal
} // namespace VcProjectManager
