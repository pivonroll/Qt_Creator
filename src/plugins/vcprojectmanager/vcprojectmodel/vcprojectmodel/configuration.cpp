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
#include "configuration.h"
#include "configurationbuildtools.h"
#include "debuggertool.h"
#include "debuggertools.h"
#include "deploymenttool.h"
#include "deploymenttools.h"
#include "generalattributecontainer.h"
#include "tools.h"
#include "tools/configurationtool.h"
#include "tools/tool_constants.h"
#include "vcprojectdocument.h"

#include <visualstudiotoolattributes/tooldescription.h>
#include <widgets/visualstudiowidgets/configurationbasewidget.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>
#include <visualstudiointerfaces/iprojectfactories.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {

Configuration::Configuration(const QString &nodeName)
    : m_nodeName(nodeName),
      m_attributeContainer(new GeneralAttributeContainer),
      m_tools(new Tools)
{
}

Configuration::Configuration(const Configuration &other)
    : m_attributeContainer(new GeneralAttributeContainer),
      m_tools(new Tools)
{
    *m_attributeContainer = *other.m_attributeContainer;
    *m_tools = *other.m_tools;
    m_nodeName = other.m_nodeName;
    m_fullName = other.m_fullName;
    m_platformName = other.m_platformName;
    m_configurationName = other.m_configurationName;
}

Configuration::Configuration(Configuration &&other)
    : Configuration()
{
    swap(*this, other);
}

Configuration &Configuration::operator=(Configuration other)
{
    swap(*this, other);
    return *this;
}

Configuration::~Configuration()
{
    delete m_attributeContainer;
    delete m_tools;
}

IConfiguration::ConfigurationVersion Configuration::version() const
{
    return ConfigurationVersion_pre2010;
}

void Configuration::processNode(const QDomNode &node)
{
    if (node.isNull())
        return;

    if (node.nodeType() == QDomNode::ElementNode)
        processNodeAttributes(node.toElement());

    if (node.hasChildNodes()) {
        QDomNode firstChild = node.firstChild();
        if (!firstChild.isNull())
            processToolNode(firstChild);
    }
}

void Configuration::processNodeAttributes(const QDomElement &element)
{
    QDomNamedNodeMap namedNodeMap = element.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String("Name")) {
                m_fullName = domElement.value();
            }

            else
                m_attributeContainer->setAttribute(domElement.name(), domElement.value());
        }
    }
}

QDomNode Configuration::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement configurationNode = domXMLDocument.createElement(m_nodeName);
    configurationNode.setAttribute(QLatin1String("Name"), m_fullName);
    m_attributeContainer->appendToXMLNode(configurationNode);
    m_tools->configurationBuildTools()->appendToXMLNode(configurationNode, domXMLDocument);
    m_tools->deploymentTools()->appendToXMLNode(configurationNode, domXMLDocument);
    m_tools->debuggerTools()->appendToXMLNode(configurationNode, domXMLDocument);

    return configurationNode;
}

IAttributeContainer *Configuration::attributeContainer() const
{
    return m_attributeContainer;
}

QString Configuration::fullName() const
{
    return m_fullName;
}

QString Configuration::displayname() const
{
    return m_configurationName;
}

QString Configuration::platform() const
{
    return m_platformName;
}

void Configuration::setFullName(const QString &fullName)
{
    m_fullName = fullName;
    emit nameChanged();
}

void Configuration::setDisplayName(const QString &name)
{
    m_configurationName = name;
}

void Configuration::setPlatform(const QString &platform)
{
    m_platformName = platform;
}

ITools *Configuration::tools() const
{
    return m_tools;
}

IConfiguration *Configuration::clone() const
{
    return new Configuration(*this);
}

VcNodeWidget *Configuration::createSettingsWidget()
{
    return new ConfigurationBaseWidget(this);
}

void Configuration::swap(Configuration &first, Configuration &second)
{
    std::swap(first.m_fullName, second.m_fullName);
    std::swap(first.m_platformName, second.m_platformName);
    std::swap(first.m_configurationName, second.m_configurationName);
    std::swap(first.m_nodeName, second.m_nodeName);
    std::swap(first.m_attributeContainer, second.m_attributeContainer);
    std::swap(first.m_tools, second.m_tools);
}

void Configuration::processToolNode(const QDomNode &toolNode)
{
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

                    // process next sibling
                    QDomNode nextSibling = toolNode.nextSibling();
                    if (!nextSibling.isNull())
                        processToolNode(nextSibling);
                }
                break;
            }
        }
    }
}

} // namespace Internal
} // namespace VcProjectManager
