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
#include "debuggertool.h"
#include "deploymenttool.h"
#include "filebuildconfiguration.h"
#include "generalattributecontainer.h"
#include "tools.h"
#include "tools/configurationtool.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <visualstudiointerfaces/iconfiguration.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/iconfigurationcontainer.h>
#include <visualstudiointerfaces/iconfigurations.h>
#include <visualstudiointerfaces/idebuggertools.h>
#include <visualstudiointerfaces/ideploymenttools.h>
#include <visualstudiointerfaces/isectioncontainer.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>
#include <widgets/visualstudiowidgets/fileconfigurationsettingswidget.h>

#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <utils/qtcassert.h>

#include <QDomElement>

namespace VcProjectManager {
namespace Internal {

FileBuildConfiguration::FileBuildConfiguration(IVisualStudioProject *parentProject)
    : Configuration(QLatin1String("FileConfiguration")),
      m_parentProjectDoc(parentProject)
{
}

FileBuildConfiguration::FileBuildConfiguration(const FileBuildConfiguration &other)
    : Configuration(other)
{
    m_parentProjectDoc = other.m_parentProjectDoc;
}

FileBuildConfiguration::FileBuildConfiguration(FileBuildConfiguration &&other)
    : FileBuildConfiguration()
{
    swap(*this, other);
}

FileBuildConfiguration &FileBuildConfiguration::operator=(FileBuildConfiguration other)
{
    swap(*this, other);
    return *this;
}

FileBuildConfiguration *FileBuildConfiguration::createFromProjectConfig(Configuration *config, IVisualStudioProject *parentProject)
{
    FileBuildConfiguration *newFileConfig = new FileBuildConfiguration(parentProject);
    *(static_cast<GeneralAttributeContainer *>(newFileConfig->attributeContainer())) = *(static_cast<GeneralAttributeContainer *>(config->attributeContainer()));
    newFileConfig->setFullName(config->fullName());
    *(static_cast<Tools *>(newFileConfig->tools())) = *(static_cast<Tools *>(config->tools()));

    return newFileConfig;
}

VcNodeWidget *FileBuildConfiguration::createSettingsWidget()
{
    return new FileConfigurationSettingsWidget(this, m_parentProjectDoc);
}

QDomNode FileBuildConfiguration::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    if (tools()->configurationBuildTools()->toolCount()) {
        IConfigurations *configs = m_parentProjectDoc->configurations();

        QDomElement configurationNode = domXMLDocument.createElement(m_nodeName);

        if (configs) {
            IConfigurationContainer *configContainer = m_parentProjectDoc->configurations()->configurationContainer();

            if (configContainer) {
                IConfiguration *projectConfig  = configContainer->configuration(fullName());
                toXMLNode(projectConfig, configurationNode, domXMLDocument);
            }
        }

        if (configurationNode.childNodes().size() || configurationNode.attributes().size()) {
            configurationNode.setAttribute(QLatin1String("Name"), m_fullName);
            return configurationNode;
        }
    }

    return QDomNode();
}

void FileBuildConfiguration::processToolNode(const QDomNode &toolNode)
{
    if (toolNode.nodeName() == QLatin1String("Tool")) {
        QDomNamedNodeMap namedNodeMap = toolNode.toElement().attributes();

        for (int i = 0; i < namedNodeMap.size(); ++i) {
            QDomNode domNode = namedNodeMap.item(i);

            if (domNode.nodeType() == QDomNode::AttributeNode) {
                QDomAttr domAttribute = domNode.toAttr();
                if (domAttribute.name() == QLatin1String("Name")) {
                    ToolDescriptionDataManager *tDDM = ToolDescriptionDataManager::instance();
                    ToolDescription *toolDesc = tDDM->toolDescription(domAttribute.value());

                    if (toolDesc) {
                        IConfigurationBuildTool *toolConf = new ConfigurationTool(toolDesc);
                        toolConf->processNode(toolNode);
                        m_tools->configurationBuildTools()->addTool(toolConf);
                    }
                    break;
                }
            }
        }
    }

    else if (toolNode.nodeName() == QLatin1String("DeploymentTool")) {
        DeploymentTool* deplTool = new DeploymentTool;
        deplTool->processNode(toolNode);
        m_tools->deploymentTools()->addTool(deplTool);
    }

    else {
        DebuggerTool* deplTool = new DebuggerTool;
        deplTool->processNode(toolNode);
        m_tools->debuggerTools()->addTool(deplTool);
    }

    // process next sibling
    QDomNode nextSibling = toolNode.nextSibling();
    if (!nextSibling.isNull())
        processToolNode(nextSibling);
}

void FileBuildConfiguration::swap(FileBuildConfiguration &first, FileBuildConfiguration &second)
{
    Configuration::swap(first, second);
    std::swap(first.m_parentProjectDoc, second.m_parentProjectDoc);
}

void FileBuildConfiguration::toXMLNode(IConfiguration *projectConfig, QDomElement &configurationNode, QDomDocument &domXMLDocument) const
{
    QTC_ASSERT(projectConfig, return);

    writeAttributes(projectConfig, configurationNode);

    // write tools
    ITools *projectTools = projectConfig->tools();

    if (projectTools && projectTools->configurationBuildTools()) {
        IConfigurationBuildTool *tool = tools()->configurationBuildTools()->tool(0);

        if (!tool)
            return;

        IConfigurationBuildTool *projectTool = projectTools->configurationBuildTools()->tool(tool->toolDescription()->toolKey());

        if (projectTool && projectTool->toolDescription())
            writeTools(projectTool, tool, configurationNode, domXMLDocument);
    }
}

void FileBuildConfiguration::writeTools(IConfigurationBuildTool *projectConfigTool, IConfigurationBuildTool *tool,
                                        QDomElement &configurationNode, QDomDocument &domXMLDocument) const
{
    QTC_ASSERT(projectConfigTool && tool, return);

    ISectionContainer *projSecContainer = projectConfigTool->sectionContainer();
    ISectionContainer *toolSecContainer = tool->sectionContainer();

    bool isNodeCreated = false;
    QDomElement toolNode;

    if (!toolSecContainer || !projSecContainer)
        return;

    for (int i = 0; i < toolSecContainer->sectionCount(); ++i) {
        IToolSection *toolSec = toolSecContainer->section(i);

        if (!toolSec)
            continue;
        IToolSection *projSec = projSecContainer->section(toolSec->sectionDescription()->displayName());

        if (!projSec)
            continue;
        IToolAttributeContainer *toolAttrContainer = toolSec->attributeContainer();
        IToolAttributeContainer *projAttrContainer = projSec->attributeContainer();

        for (int j = 0; j < toolAttrContainer->toolAttributeCount(); ++j) {
            IToolAttribute *toolAttr = toolAttrContainer->toolAttribute(j);

            if (!toolAttr || !toolAttr->descriptionDataItem())
                continue;

            IToolAttribute *projToolAttr = projAttrContainer->toolAttribute(toolAttr->descriptionDataItem()->key());

            if (!projToolAttr || projToolAttr->value() != toolAttr->value())
                continue;

            if (!isNodeCreated) {
                toolNode = domXMLDocument.createElement(QLatin1String("Tool"));
                toolNode.setAttribute(QLatin1String("Name"), tool->toolDescription()->toolKey());
                configurationNode.appendChild(toolNode);
                isNodeCreated = true;
            }

            toolNode.setAttribute(toolAttr->descriptionDataItem()->key(),
                                  toolAttr->value());
        }
    }
}

void FileBuildConfiguration::writeAttributes(IConfiguration *projectConfig, QDomElement &configurationNode) const
{
    IAttributeContainer *projConfigAttrContainer = projectConfig->attributeContainer();

    for (int i = 0; i < m_attributeContainer->getAttributeCount(); ++i) {
        QString attrName = m_attributeContainer->getAttributeName(i);
        if (projConfigAttrContainer) {
            if (projConfigAttrContainer->attributeValue(attrName) != m_attributeContainer->attributeValue(attrName))
                configurationNode.setAttribute(attrName, m_attributeContainer->attributeValue(attrName));
        } else
            configurationNode.setAttribute(attrName, m_attributeContainer->attributeValue(attrName));
    }
}

IConfiguration *FileBuildConfiguration::clone() const
{
    return new FileBuildConfiguration(*this);
}

} // namespace Internal
} // namespace VcProjectManager
