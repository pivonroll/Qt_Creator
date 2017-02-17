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
#include "file.h"

#include "configurationcontainer.h"
#include "filebuildconfiguration.h"
#include "generalattributecontainer.h"
#include "vcprojectdocument.h"
#include "tools/tool_constants.h"
#include "../utils/utils.h"

#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>
#include <visualstudiotoolattributes/attributedescriptiondataitem.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/isectioncontainer.h>
#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/itoolsection.h>

#include <visualstudiotoolattributes/tooldescription.h>

#include <widgets/visualstudiowidgets/fileconfigurationseditwidget.h>

#include <coreplugin/icore.h>
#include <utils/fileutils.h>

#include <QDomNode>
#include <QFileInfo>
#include <QDir>

namespace VcProjectManager {
namespace Internal {

File::File(IVisualStudioProject *parentProjectDoc)
    : m_parentProjectDoc(parentProjectDoc)
{
    m_configurationContainer = new ConfigurationContainer;
    m_attributeContainer = new GeneralAttributeContainer;
}

File::File(const File &other)
{
    m_attributeContainer = new GeneralAttributeContainer;
    m_configurationContainer = new ConfigurationContainer;

    m_parentProjectDoc = other.m_parentProjectDoc;
    m_relativePath = other.m_relativePath;
    *m_configurationContainer = *(other.m_configurationContainer);
    *m_attributeContainer = *(other.m_attributeContainer);

    foreach (const File *f, other.m_files)
        m_files.append(new File(*f));
}

File::File(File &&other)
    : File()
{
    swap(*this, other);
}

File &File::operator=(File other)
{
    swap(*this, other);
    return *this;
}

File::~File()
{
    qDeleteAll(m_files);
    delete m_configurationContainer;
    delete m_attributeContainer;
}

void File::processNode(const QDomNode &node)
{
    if (node.isNull())
        return;

    if (node.nodeType() == QDomNode::ElementNode)
        processNodeAttributes(node.toElement());

    copyProjectConfigs();

    if (node.hasChildNodes()) {
        QDomNode firstChild = node.firstChild();
        if (!firstChild.isNull()) {
            if (firstChild.nodeName() == QLatin1String("FileConfiguration"))
                processFileConfiguration(firstChild);
            else if (firstChild.nodeName() == QLatin1String("File"))
                processFile(firstChild);
        }
    }
}

VcNodeWidget *File::createSettingsWidget()
{
    return new FileConfigurationsEditWidget(this, m_parentProjectDoc);
}

QDomNode File::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QDomElement fileNode = domXMLDocument.createElement(QLatin1String("File"));

    fileNode.setAttribute(QLatin1String("RelativePath"), m_relativePath);

    foreach (const File *file, m_files)
        fileNode.appendChild(file->toXMLDomNode(domXMLDocument));

    m_configurationContainer->appendToXMLNode(fileNode, domXMLDocument);
    m_attributeContainer->appendToXMLNode(fileNode);
    return fileNode;
}

IConfigurationContainer *File::configurationContainer() const
{
    return m_configurationContainer;
}

IAttributeContainer *File::attributeContainer() const
{
    return m_attributeContainer;
}

QString File::displayName() const
{
    int index = m_relativePath.lastIndexOf(QLatin1Char('\\'));
    if (index == -1)
        return m_relativePath;

    return m_relativePath.mid(index + 1);
}

QString File::relativePath() const
{
    return m_relativePath;
}

void File::setRelativePath(const QString &relativePath)
{
    m_relativePath = relativePath;
}

IFile *File::clone() const
{
    return new File(*this);
}

ProjectExplorer::FileType File::fileType() const
{
    return VcProjectManager::Internal::VisualStudioUtils::getFileType(::Utils::FileName::fromString(canonicalPath()));
}

QString File::canonicalPath() const
{
    if (m_parentProjectDoc) {
        QFileInfo fileInfo(m_parentProjectDoc->filePath());
        fileInfo = QFileInfo(QDir::toNativeSeparators(fileInfo.canonicalPath() + QLatin1Char('/') + m_relativePath));
        return fileInfo.canonicalFilePath();
    }

    return QString() + m_relativePath;
}

IConfiguration *File::createDefaultBuildConfiguration(const QString &configName, const QString &platformName) const
{
    Q_UNUSED(configName)
    Q_UNUSED(platformName)
    return nullptr;
}

void File::swap(File &first, File &second)
{
    std::swap(first.m_relativePath, second.m_relativePath);
    std::swap(first.m_files, second.m_files);
    std::swap(first.m_parentProjectDoc, second.m_parentProjectDoc);
    std::swap(first.m_configurationContainer, second.m_configurationContainer);
    std::swap(first.m_attributeContainer, second.m_attributeContainer);
}

void File::processFileConfiguration(const QDomNode &fileConfigNode)
{
    IConfiguration *fileConfig = new FileBuildConfiguration(m_parentProjectDoc);
    fileConfig->processNode(fileConfigNode);

    IConfiguration *projConfig = m_configurationContainer->configuration(fileConfig->fullName());

    if (projConfig) {
        copyAllNonDefaultToolAtributes(fileConfig, projConfig);

        m_configurationContainer->removeConfiguration(projConfig->fullName());
        m_configurationContainer->addConfiguration(fileConfig);

        // process next sibling
        QDomNode nextSibling = fileConfigNode.nextSibling();
        if (!nextSibling.isNull()) {
            if (nextSibling.nodeName() == QLatin1String("FileConfiguration"))
                processFileConfiguration(nextSibling);
            else
                processFile(nextSibling);
        }
    } else
        delete fileConfig;
}

void File::processFile(const QDomNode &fileNode)
{
    File *file = new File(m_parentProjectDoc);
    file->processNode(fileNode);
    m_files.append(file);

    // process next sibling
    QDomNode nextSibling = fileNode.nextSibling();
    if (!nextSibling.isNull()) {
        if (nextSibling.nodeName() == QLatin1String("FileConfiguration"))
            processFileConfiguration(nextSibling);
        else
            processFile(nextSibling);
    }
}

void File::processNodeAttributes(const QDomElement &element)
{
    QDomNamedNodeMap namedNodeMap = element.attributes();

    for (int i = 0; i < namedNodeMap.size(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();

            if (domElement.name() == QLatin1String("RelativePath"))
                m_relativePath = domElement.value();

            else
                m_attributeContainer->setAttribute(domElement.name(), domElement.value());
        }
    }
}

void File::copyAllNonDefaultToolAtributes(IConfiguration *fileConfig, IConfiguration *projConfig)
{
    if (!fileConfig || !projConfig ||
            !fileConfig->tools() || !fileConfig->tools()->configurationBuildTools() ||
            !projConfig->tools() || !projConfig->tools()->configurationBuildTools())
        return ;
    IConfigurationBuildTool *tool = fileConfig->tools()->configurationBuildTools()->tool(0);

    if (!tool || !tool->toolDescription())
        return;
    IConfigurationBuildTool *projToolCopy = projConfig->tools()->configurationBuildTools()->
            tool(tool->toolDescription()->toolKey());

    if (!projToolCopy)
        return;

    ISectionContainer *secCont = tool->sectionContainer();
    ISectionContainer *projSecCont = projToolCopy->sectionContainer();

    if (!secCont || !projSecCont)
        return;

    for (int i = 0; i < secCont->sectionCount(); ++i) {
        IToolSection *toolSec = secCont->section(i);

        if (toolSec && toolSec->sectionDescription()) {
            IToolSection *projSec = projSecCont->section(toolSec->sectionDescription()->displayName());
            copyAllNonDefaultToolAtributes(toolSec, projSec);
        }
    }
}

void File::copyAllNonDefaultToolAtributes(IToolSection *fileSec, IToolSection *projSec)
{
    if (!fileSec || !projSec ||
            !fileSec->sectionDescription() || !projSec->sectionDescription())
        return;

    IToolAttributeContainer *attrCont = fileSec->attributeContainer();
    IToolAttributeContainer *projAttrCont = projSec->attributeContainer();

    if (!attrCont || !projAttrCont)
        return;

    for (int i = 0; i < projAttrCont->toolAttributeCount(); ++i) {
        IToolAttribute *projToolAttr = projAttrCont->toolAttribute(i);

        if (!projToolAttr || !projToolAttr->descriptionDataItem())
            continue;

        IToolAttribute *toolAttr = attrCont->toolAttribute(projToolAttr->descriptionDataItem()->key());
        if (toolAttr && !toolAttr->isUsed() && projToolAttr && projToolAttr->isUsed())
            toolAttr->setValue(projToolAttr->value());
    }
}

void File::leaveOnlyCppTool(IConfiguration *config)
{
    if (!config || !config->tools() || !config->tools()->configurationBuildTools())
        return;

    int i = 0;
    while (config->tools()->configurationBuildTools()->toolCount() > 1) {
        IConfigurationBuildTool *tool = config->tools()->configurationBuildTools()->tool(i);

        if (tool->toolDescription()->toolKey() != QLatin1String(ToolConstants::strVCCLCompilerTool)) {
            config->tools()->configurationBuildTools()->removeTool(tool);
            delete tool;
        }

        else
            ++i;
    }
}

void File::copyProjectConfigs()
{
    if (!m_parentProjectDoc || !m_parentProjectDoc->configurations() ||
            !m_parentProjectDoc->configurations()->configurationContainer())
        return;

    for (int i = 0; i < m_parentProjectDoc->configurations()->configurationContainer()->configurationCount(); ++i) {
        IConfiguration *config = m_parentProjectDoc->configurations()->configurationContainer()->configuration(i);

        if (!config)
            continue;

        FileBuildConfiguration *newConfig = FileBuildConfiguration::createFromProjectConfig(static_cast<Configuration*>(config), m_parentProjectDoc);
        leaveOnlyCppTool(newConfig);
        m_configurationContainer->addConfiguration(newConfig);
    }
}

} // namespace Internal
} // namespace VcProjectManager
