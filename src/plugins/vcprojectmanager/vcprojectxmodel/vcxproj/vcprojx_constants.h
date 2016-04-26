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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_VCPROJX_CONSTANTS_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_VCPROJX_CONSTANTS_H

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

const char CONFIGURATION [] = "Configuration";
const char CONFIGURATION_VARIABLE [] = "$(Configuration)";
const char PLATFORM [] = "Platform";
const char PLATFORM_VARIABLE [] = "$(Platform)";
const char PROJECT_CONFIGURATION[] = "ProjectConfiguration";
const char PROPERTY_SHEETS [] = "PropertySheets";
const char CONFIGURATION_PLATFORM_DELIMITER [] = "|";

const char FILE_ITEM_NONE [] = "None";
const char FILE_ITEM_CL_INCLUDE [] = "ClInclude";
const char FILE_ITEM_CL_COMPILE [] = "ClCompile";
const char FILE_ITEM_RESOURCE_COMPILE [] = "ResourceCompile";

const char UNIQUE_IDENTIFIER [] = "UniqueIdentifier";
const char FILTER_ITEM [] = "Filter";
const char CHOOSE_ITEM [] = "Choose";
const char OTHERWISE_ITEM [] = "Otherwise";
const char WHEN_ITEM [] = "When";
const char IMPORT_ITEM [] = "Import";
const char IMPORT_GROUP_ITEM [] = "ImportGroup";
const char ITEM_DEFINITION_GROUP_ITEM [] = "ItemDefinitionGroup";
const char ITEM_GROUP_ITEM [] = "ItemGroup";
const char ON_ERROR_ITEM [] = "OnError";
const char PROPERTY_GROUP_ITEM [] = "PropertyGroup";
const char PARAMETER_GROUP_ITEM [] = "ParameterGroup";
const char PROJECT_EXTENSIONS_ITEM [] = "ProjectExtensions";
const char USING_TASK_ITEM [] = "UsingTask";
const char TARGET_ITEM [] = "Target";
const char TASK_ITEM [] = "Task";
const char TASK_BODY_ITEM [] = "TaskBody";

const char ITEM_NAME_ATTRIBUTE [] = "ItemName";
const char EXECUTE_TARGETS_ATTRIBUTE [] = "ExecuteTargets";
const char PARAMETER_TYPE_ATTRIBUTE [] = "ParameterType";
const char REQUIRED_ATTRIBUTE [] = "Required";
const char TASK_PARAMETER_ATTRIBUTE [] = "TaskParameter";
const char PROPERTY_NAME_ATTRIBUTE [] = "PropertyName";
const char OUTPUT_ATTRIBUTE [] = "Output";
const char EXCLUDE_ATTRIBUTE [] = "Exclude";
const char REMOVE_ATTRIBUTE [] = "Remove";
const char INCLUDE_ATTRIBUTE [] = "Include";
const char PROJECT_ATTRIBUTE [] = "Project";
const char LABEL_ATTRIBUTE [] = "Label";
const char CONDITION_ATTRIBUTE [] = "Condition";
const char XMLNS_ATTRIBUTE [] = "xmlns";
const char TOOLS_VERSION_ATTRIBUTE [] = "ToolsVersion";
const char INITIAL_TARGETS_ATTRIBUTE [] = "InitialTargets";
const char DEFAULT_TARGETS_ATTRIBUTE [] = "DefaultTargets";
const char KEEP_DUPLICATE_OUTPUTS_ATTRIBUTE [] = "KeepDuplicateOutputs";
const char DEPENDS_ON_TARGETS_ATTRIBUTE [] = "DependsOnTargets";
const char AFTER_TARGETS_ATTRIBUTE [] = "AfterTargets";
const char BEFORE_TARGETS_ATTRIBUTE [] = "BeforeTargets";
const char RETURNS_ATTRIBUTE [] = "Returns";
const char OUTPUTS_ATTRIBUTE [] = "Outputs";
const char INPUTS_ATTRIBUTE [] = "Inputs";
const char NAME_ATTRIBUTE [] = "Name";
const char CONTINUE_ON_ERROR_ATTRIBUTE [] = "ContinueOnError";
const char EVALUATE_ATTRIBUTE [] = "Evaluate";
const char ASSEMBLY_NAME_ATTRIBUTE [] = "AssemblyName";
const char ASSEMBLY_FILE_ATTRIBUTE [] = "AssemblyFile";
const char TASK_FACTORY_ATTRIBUTE [] = "TaskFactory";
const char TASK_NAME_ATTRIBUTE [] = "TaskName";

const char KEEP_DUPLICATE_OUTPUTS_FALSE [] = "false";
const char FILTERS_EXTENSION [] = ".filters";

const char CL_INCLUDE[] = "ClInclude";
const char CL_COMPILE[] = "ClCompile";

const char PROJECT_CONFIGURATIONS[] = "ProjectConfigurations";

} // VisualStudioProjectX
} // Internal
} // VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_VCPROJX_CONSTANTS_H
