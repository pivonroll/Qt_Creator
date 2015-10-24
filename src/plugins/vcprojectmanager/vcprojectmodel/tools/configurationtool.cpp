/**************************************************************************
**
** Copyright (c) 2014 Bojan Petrovic
** Copyright (c) 2014 Radovan Zivkovic
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
#include "configurationtool.h"

#include "toolsection.h"

#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>
#include <visualstudiotoolattributes/toolsectioncontainer.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <visualstudiointerfaces/iattributedescriptiondataitem.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {

ConfigurationTool::ConfigurationTool(const IToolDescription *toolDesc)
    : m_toolDesc(toolDesc)
{
    m_sectionContainer = new ToolSectionContainer;
    for (int i = 0; i < toolDesc->sectionDescriptionCount(); ++i) {
        if (toolDesc->sectionDescription(i))
            m_sectionContainer->appendSection(toolDesc->sectionDescription(i)->createToolSection());
    }
}

ConfigurationTool::ConfigurationTool(const ConfigurationTool &tool)
{
    m_toolDesc = tool.m_toolDesc;
    m_sectionContainer = new ToolSectionContainer;
    *m_sectionContainer = *(tool.m_sectionContainer);
}

ConfigurationTool::~ConfigurationTool()
{
    delete m_sectionContainer;
}

void ConfigurationTool::processNode(const QDomNode &node)
{
    if (node.isNull())
        return;

    if (node.nodeType() == QDomNode::ElementNode)
        processNodeAttributes(node.toElement());
}

QDomNode ConfigurationTool::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement toolNode = domXMLDocument.createElement(QLatin1String("Tool"));
    toolNode.setAttribute(QLatin1String("Name"), m_toolDesc->toolKey());
    m_sectionContainer->appendToXMLNode(toolNode);
    return toolNode;
}

const IToolDescription *ConfigurationTool::toolDescription() const
{
    return m_toolDesc;
}

VcNodeWidget *ConfigurationTool::createSettingsWidget()
{
    return new ToolSettingsWidget(this);
}

ISectionContainer *ConfigurationTool::sectionContainer() const
{
    return m_sectionContainer;
}

IConfigurationBuildTool *ConfigurationTool::clone() const
{
    return new ConfigurationTool(*this);
}

bool ConfigurationTool::allAttributesAreDefault() const
{
    for (int i = 0; i < m_sectionContainer->sectionCount(); ++i) {
        IToolSection *section = m_sectionContainer->section(i);

        if (section && section->attributeContainer()) {
            for (int j = 0; j < section->attributeContainer()->toolAttributeCount(); ++j) {
                IToolAttribute *attr = section->attributeContainer()->toolAttribute(j);

                if (attr && attr->value() != attr->descriptionDataItem()->defaultValue())
                    return false;
            }
        }
    }

    return true;
}

void ConfigurationTool::processNodeAttributes(const QDomElement &domElement)
{
    QDomNamedNodeMap namedNodeMap = domElement.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() != QLatin1String("Name")) {
                for (int j = 0; i < m_sectionContainer->sectionCount(); ++j) {
                    IToolSection *toolSection = m_sectionContainer->section(j);
                    if (toolSection) {
                        IToolAttribute *toolAttr = toolSection->attributeContainer()->toolAttribute(domElement.name());

                        if (toolAttr)
                            toolAttr->setValue(domElement.value());
                    }
                }
            }
        }
    }
}

} // namespace Internal
} // namespace VcProjectManager
