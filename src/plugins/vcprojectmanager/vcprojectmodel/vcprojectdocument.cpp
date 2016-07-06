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

#include "generalattributecontainer.h"
#include "vcprojectdocument.h"
#include "tools/tool_constants.h"
#include "tools/configurationtool.h"

#include <visualstudiointerfaces/iattributecontainer.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/iconfigurationcontainer.h>
#include <visualstudiointerfaces/itools.h>
#include <widgets/visualstudiowidgets/configurationseditwidget.h>
#include <widgets/visualstudiowidgets/projectsettingswidget.h>

#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <coreplugin/mainwindow.h>

#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QPair>
#include <QStringList>
#include <QTextStream>
#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

VcProjectDocument::~VcProjectDocument()
{
}

bool VcProjectDocument::saveToFile(const QString &filePath) const
{
    QDomDocument domDoc;
    toXMLDomNode(domDoc);

    QFile outFile(filePath);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(Core::ICore::mainWindow(), QObject::tr("Error!"), QObject::tr("Failed to open project file for writing."));
        return false;
    }

    QTextStream stream( &outFile );
    stream << domDoc.toString();
    outFile.close();
    return true;
}

DocumentVersion VcProjectDocument::documentVersion() const
{
    return m_documentVersion;
}

QString VcProjectDocument::filePath() const
{
    return m_filePath;
}

IConfigurations* VcProjectDocument::configurations() const
{
    return m_configurations;
}

IPlatforms* VcProjectDocument::platforms() const
{
    return m_platforms;
}

IReferences *VcProjectDocument::referencess() const
{
    return m_references;
}

IToolFiles *VcProjectDocument::toolFiles() const
{
    return m_toolFiles;
}

IPublishingData *VcProjectDocument::publishingData() const
{
    return m_publishingData;
}

IAttributeContainer *VcProjectDocument::attributeContainer() const
{
    return m_attributeContainer;
}

IFiles* VcProjectDocument::files() const
{
    return m_files;
}

IGlobals* VcProjectDocument::globals() const
{
    return m_globals;
}

VcProjectDocument::VcProjectDocument(const QString &filePath, DocumentVersion docVersion)
    : m_filePath(filePath),
      m_documentVersion(docVersion),
      m_platforms(new Platforms),
      m_configurations(new Configurations(this)),
      m_files(new Files(this)),
      m_references(new References),
      m_globals(new Globals),
      m_toolFiles(new ToolFiles),
      m_publishingData(new PublishingData),
      m_attributeContainer(new GeneralAttributeContainer)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument document(filePath);
    if (!document.setContent(&file))
        return;

    processNode(document);
}

VcProjectDocument::VcProjectDocument(const VcProjectDocument &other)
{
    // <?xml part
    m_processingInstr = other.m_processingInstr;
    m_processingInstructionTarget = other.m_processingInstructionTarget;

    m_attributeContainer = new GeneralAttributeContainer(*other.m_attributeContainer);
    m_platforms = new Platforms(*(other.m_platforms));
    m_configurations = new Configurations(*(other.m_configurations));
    m_files = new Files(*other.m_files);
    m_references = new References(*(other.m_references));
    m_globals = new Globals(*(other.m_globals));

    m_toolFiles = new ToolFiles(*(other.m_toolFiles));
    m_publishingData = new PublishingData(*(other.m_publishingData));
}

VcProjectDocument::VcProjectDocument(VcProjectDocument &&other)
    : VcProjectDocument()
{
    swap(*this, other);
}

VcProjectDocument &VcProjectDocument::operator=(VcProjectDocument other)
{
    swap(*this, other);
    return *this;
}

void VcProjectDocument::processNode(const QDomNode &domDoc)
{
    QDomNode node = domDoc.firstChild(); // xml

    parseProcessingInstruction(node.toProcessingInstruction());

    node = node.nextSibling(); // Visual studio
    processDocumentAttributes(node.toElement());
    node = node.firstChild();
    processDocumentNode(node);
}

void VcProjectDocument::parseProcessingInstruction(const QDomProcessingInstruction &processingInstruction)
{
    QStringList data = processingInstruction.data().split(QLatin1Char(' '));
    foreach (const QString &dataElement, data) {
        QStringList sl = dataElement.split(QLatin1Char('='));

        if (sl.size() == 2) {
            QString value = sl[1];
            value = value.replace(QLatin1String("\'"), QString());
            m_processingInstr.append(QPair<QString, QString>(sl[0].trimmed(), value.trimmed()));
        }
    }

    m_processingInstructionTarget = processingInstruction.target();
}

void VcProjectDocument::addToolToConfiguration(IConfiguration *config, const QString &toolKey) const
{
    if (!config || !config->tools() || !config->tools()->configurationBuildTools() || toolKey.isEmpty())
        return;

    ToolDescriptionDataManager *tDDM = ToolDescriptionDataManager::instance();
    ToolDescription *toolDesc = tDDM->toolDescription(toolKey);

    if (toolDesc) {
        IConfigurationBuildTool *tool = new ConfigurationTool(toolDesc);
        config->tools()->configurationBuildTools()->addTool(tool);
    }
}

void VcProjectDocument::processDocumentAttributes(const QDomElement &vsNode)
{
    QDomNamedNodeMap namedNodeMap = vsNode.attributes();

    for (int i = 0; i < namedNodeMap.count(); ++i) {
        QDomNode domNode = namedNodeMap.item(i);

        if (domNode.nodeType() == QDomNode::AttributeNode) {
            QDomAttr domElement = domNode.toAttr();
            m_attributeContainer->setAttribute(domElement.name(), domElement.value());
        }
    }
}

QDomNode VcProjectDocument::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    QString processingData;

    QPair<QString, QString> pr;

    foreach (pr, m_processingInstr) {
        processingData.append(pr.first);
        processingData.append(QLatin1Char('='));
        processingData.append(QLatin1String("\"") + pr.second + QLatin1String("\""));
        processingData.append(QLatin1Char(' '));
    }

    QDomProcessingInstruction processingInstruction = domXMLDocument.createProcessingInstruction(QLatin1String("xml"), processingData);
    domXMLDocument.appendChild(processingInstruction);
    domXMLDocument.appendChild(toVcDocumentElement(domXMLDocument));

    return domXMLDocument;
}

QDomElement VcProjectDocument::toVcDocumentElement(QDomDocument &domXMLDocument) const
{
    QDomElement vcDocNode = domXMLDocument.createElement(QLatin1String("VisualStudioProject"));

    m_attributeContainer->appendToXMLNode(vcDocNode);

    if (m_platforms->platformCount())
        vcDocNode.appendChild(m_platforms->toXMLDomNode(domXMLDocument));

    if (m_configurations->configurationContainer()->configurationCount())
        vcDocNode.appendChild(m_configurations->toXMLDomNode(domXMLDocument));

    if (m_files->fileCount() || m_files->fileContainerCount())
        vcDocNode.appendChild(m_files->toXMLDomNode(domXMLDocument));

    if (m_references->referenceCount())
        vcDocNode.appendChild(m_references->toXMLDomNode(domXMLDocument));

    if (m_globals->globalCount())
        vcDocNode.appendChild(m_globals->toXMLDomNode(domXMLDocument));

    if (m_toolFiles->toolFileCount())
        vcDocNode.appendChild(m_toolFiles->toXMLDomNode(domXMLDocument));

    if (m_publishingData->publishingItemCount() && m_publishingData->attributeContainer()->getAttributeCount())
        vcDocNode.appendChild(m_publishingData->toXMLDomNode(domXMLDocument));

    return vcDocNode;
}

VcNodeWidget *VcProjectDocument::createSettingsWidget()
{
    return new VcProjectDocumentWidget(this);
}

IConfiguration *VcProjectDocument::createDefaultBuildConfiguration(const QString &fullConfigName) const
{
    IConfiguration *newConfig = new Configuration(QLatin1String("Configuration"));
    newConfig->setFullName(fullConfigName);

    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCCLCompilerTool));
    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCLinkerTool));
    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCManifestTool));
    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCXDCMakeTool));
    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCBscMakeTool));
    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCPreBuildEventTool));
    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCPreLinkEventTool));
    addToolToConfiguration(newConfig, QLatin1String(ToolConstants::strVCPostBuildEventTool));

    newConfig->attributeContainer()->setAttribute(QLatin1String("OutputDirectory"), QLatin1String("$(SolutionDir)$(ConfigurationName)"));
    newConfig->attributeContainer()->setAttribute(QLatin1String("IntermediateDirectory"), QLatin1String("$(ConfigurationName)"));
    newConfig->attributeContainer()->setAttribute(QLatin1String("ConfigurationType"), QLatin1String("1"));

    return newConfig;
}

IProjectFactories *VcProjectDocument::projectFactories() const
{
    return nullptr;
}

IVisualStudioProject *VcProjectDocument::clone() const
{
    return new VcProjectDocument(*this);
}

VcProjectDocument::VcProjectDocument()
    : m_platforms(nullptr),
      m_configurations(nullptr),
      m_files(nullptr),
      m_references(nullptr),
      m_globals(nullptr),
      m_toolFiles(nullptr),
      m_publishingData(nullptr),
      m_attributeContainer(nullptr)
{

}

void VcProjectDocument::swap(VcProjectDocument &first, VcProjectDocument &second)
{
    std::swap(first.m_attributeContainer, second.m_attributeContainer);
    std::swap(first.m_configurations, second.m_configurations);
    std::swap(first.m_documentVersion, second.m_documentVersion);
    std::swap(first.m_filePath, second.m_filePath);
    std::swap(first.m_files, second.m_files);
    std::swap(first.m_globals, second.m_globals);
    std::swap(first.m_platforms, second.m_platforms);
    std::swap(first.m_processingInstr, second.m_processingInstr);
    std::swap(first.m_processingInstructionTarget, second.m_processingInstructionTarget);
    std::swap(first.m_publishingData, second.m_publishingData);
    std::swap(first.m_references, second.m_references);
    std::swap(first.m_toolFiles, second.m_toolFiles);
}

void VcProjectDocument::processDocumentNode(const QDomNode &node)
{
    if (node.isNull())
        return;

    if (node.nodeName() == QLatin1String("Platforms"))
        m_platforms->processNode(node);

    else if (node.nodeName() == QLatin1String("Configurations"))
        m_configurations->processNode(node);

    else if (node.nodeName() == QLatin1String("References"))
        m_references->processNode(node);

    else if (node.nodeName() == QLatin1String("Files"))
        m_files->processNode(node);

    else if (node.nodeName() == QLatin1String("Globals"))
        m_globals->processNode(node);

    else if (node.nodeName() == QLatin1String("ToolFiles"))
        m_toolFiles->processNode(node);

    else if (node.nodeName() == QLatin1String("PublishingData"))
        m_publishingData->processNode(node);

    processDocumentNode(node.nextSibling());
}


VcProjectDocumentWidget::VcProjectDocumentWidget(VcProjectDocument *vcDoc)
    : m_vcDoc(vcDoc)
{
    ProjectSettingsWidget *projectSettingsWidget = new ProjectSettingsWidget(this);

    // add Configurations
    m_configurationsWidget = static_cast<ConfigurationsEditWidget *>(m_vcDoc->configurations()->createSettingsWidget());
    projectSettingsWidget->addWidget(tr("Configurations"), m_configurationsWidget);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(projectSettingsWidget);
    setLayout(layout);

    connect(projectSettingsWidget, &ProjectSettingsWidget::okButtonClicked, this, &VcProjectDocumentWidget::onOkButtonClicked);
    connect(projectSettingsWidget, &ProjectSettingsWidget::cancelButtonClicked, this, &VcProjectDocumentWidget::onCancelButtonClicked);
}

VcProjectDocumentWidget::~VcProjectDocumentWidget()
{
}

void VcProjectDocumentWidget::saveData()
{
    m_configurationsWidget->saveData();
}

void VcProjectDocumentWidget::onOkButtonClicked()
{
    saveData();
    hide();
    emit accepted();
    deleteLater();
}

void VcProjectDocumentWidget::onCancelButtonClicked()
{
    hide();
    emit cancelled();
    deleteLater();
}

} // namespace Internal
} // namespace VcProjectManager
