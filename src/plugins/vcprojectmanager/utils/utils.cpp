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
#include "utils.h"

#include <vcprojectmodel/tools/tool_constants.h>
#include <vcprojectxmodel/tools/tool_constantsx.h>

#include "../common/projectconstants.h"
#include "../common/vcschemamanager.h"

#include <visualstudiointerfaces/iattributecontainer.h>
#include <visualstudiointerfaces/iconfiguration.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/isectioncontainer.h>

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>
#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
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
 * \return \b true if Visual Studio project at \a filePath matches schema.
 * Otherwise it returns \b false.
 */

bool checkIfProjectVersionMathcesSchema(Constants::SchemaVersion schemaVersion, const QString &filePath)
{
    QString vcSchema = VcSchemaManager::instance()->documentSchema(schemaVersion);

    QTC_ASSERT(!vcSchema.isEmpty(), return false);

    QFile schemaFile(vcSchema);
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
DocumentVersion getProjectVersion(const QString &projectPath)
{
    if (checkIfProjectVersionMathcesSchema(Constants::SV_2003, projectPath))
        return DV_MSVC_2003;
    else if (checkIfProjectVersionMathcesSchema(Constants::SV_2005, projectPath))
        return DV_MSVC_2005;
    else if (checkIfProjectVersionMathcesSchema(Constants::SV_2008, projectPath))
        return DV_MSVC_2008;

    return DV_UNRECOGNIZED;
}

ProjectExplorer::FileType getFileType(const ::Utils::FileName &canonicalFilePath)
{
    ::Utils::MimeDatabase mdb;
    QString mimeType = mdb.mimeTypeForFile(canonicalFilePath.toString()).name();

    if (mimeType == QLatin1String(ProjectExplorer::Constants::CPP_SOURCE_MIMETYPE)
            || mimeType == QLatin1String(ProjectExplorer::Constants::C_SOURCE_MIMETYPE))
        return ProjectExplorer::FileType::Source;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::CPP_HEADER_MIMETYPE)
            || mimeType == QLatin1String(ProjectExplorer::Constants::C_HEADER_MIMETYPE))
        return ProjectExplorer::FileType::Header;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::RESOURCE_MIMETYPE))
        return ProjectExplorer::FileType::Resource;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::FORM_MIMETYPE))
        return ProjectExplorer::FileType::Form;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::QML_MIMETYPE))
        return ProjectExplorer::FileType::QML;

    return ProjectExplorer::FileType::Unknown;
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
