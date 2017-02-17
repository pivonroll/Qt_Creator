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
#include "vcprojectdocumentx.h"

#include <QFile>
#include <QTextStream>
#include <QDomNode>

#include "configurationx.h"
#include "filesx.h"
#include "utilsx.h"
#include "vcprojx_constants.h"

#include "private/importgroup.h"
#include "private/item.h"
#include "private/itemgroup.h"
#include "private/itemdefinitiongroup.h"
#include "private/itemmetadata.h"
#include "private/project.h"
#include "private/propertygroup.h"
#include "configurationsx.h"
#include "projectfactoriesx.h"

namespace VcProjectManager {
namespace Internal {

namespace VisualStudioProjectX {
class ItemGroup;
}

VcProjectDocumentX::VcProjectDocumentX(const QString &filePath)
    : m_filePath(filePath),
      m_project(new VisualStudioProjectX::Project),
      m_filters(new VisualStudioProjectX::Project),
      m_projectFactories(new VisualStudioProjectX::ProjectFactoriesX(this))
{
    if (filePath.isEmpty())
        return;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument domDocument(filePath);
    domDocument.setContent(&file);
    processNode(domDocument);
    processFilterDoc();

    m_configurations = new VisualStudioProjectX::ConfigurationsX(m_project);

    m_files = new VisualStudioProjectX::FilesX(m_project, m_filters, this);
}

VcProjectDocumentX::VcProjectDocumentX(const VcProjectDocumentX &other)
{
    m_project = new VisualStudioProjectX::Project(*other.m_project);
    m_filters = new VisualStudioProjectX::Project(*other.m_filters);
    m_configurations = new VisualStudioProjectX::ConfigurationsX(*other.m_configurations);
    m_files = new VisualStudioProjectX::FilesX(*other.m_files);
    m_projectFactories = new VisualStudioProjectX::ProjectFactoriesX(this);
    m_filePath = other.m_filePath;
    m_filtersProcInstructionNode = other.m_filtersProcInstructionNode;
    m_projectProcInstructionNode = other.m_projectProcInstructionNode;
}

VcProjectDocumentX::VcProjectDocumentX(VcProjectDocumentX &&other)
    : VcProjectDocumentX()
{
    swap(*this, other);
}

VcProjectDocumentX &VcProjectDocumentX::operator=(VcProjectDocumentX other)
{
    swap(*this, other);
    return *this;
}

VcProjectDocumentX::~VcProjectDocumentX()
{
    delete m_project;
    delete m_filters;
    delete m_files;
    delete m_projectFactories;
    delete m_configurations;
}

IConfigurations *VcProjectDocumentX::configurations() const
{
    return m_configurations;
}

IFiles *VcProjectDocumentX::files() const
{
    return m_files;
}

IGlobals *VcProjectDocumentX::globals() const
{
    return nullptr;
}

IPlatforms *VcProjectDocumentX::platforms() const
{
    return nullptr;
}

IReferences *VcProjectDocumentX::referencess() const
{
    return nullptr;
}

IAttributeContainer *VcProjectDocumentX::attributeContainer() const
{
    return nullptr;
}

IToolFiles *VcProjectDocumentX::toolFiles() const
{
    return nullptr;
}

IPublishingData *VcProjectDocumentX::publishingData() const
{
    return nullptr;
}

QString VcProjectDocumentX::filePath() const
{
    return m_filePath;
}

bool VcProjectDocumentX::saveToFile(const QString &filePath) const
{
    if (m_project && m_filters) {
        // save project
        QDomDocument domDoc;
        QDomProcessingInstruction procesingInstruction = domDoc.createProcessingInstruction(m_projectProcInstructionNode.target(),
                                                                                            m_projectProcInstructionNode.data());
        domDoc.appendChild(procesingInstruction);
        m_project->toXMLDomNode(domDoc);

        QFile outFile(filePath);
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream stream(&outFile);
        stream << domDoc.toString();
        outFile.close();

        // save filters
        domDoc = QDomDocument();
        procesingInstruction = domDoc.createProcessingInstruction(m_filtersProcInstructionNode.target(),
                                                                  m_filtersProcInstructionNode.data());
        domDoc.appendChild(procesingInstruction);
        m_filters->toXMLDomNode(domDoc);

        QString filterPath = filePath + QLatin1String(VisualStudioProjectX::FILTERS_EXTENSION);
        QFile filterOutFile(filterPath);
        if (!filterOutFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream filterStream(&outFile);
        filterStream << domDoc.toString();
        filterOutFile.close();

        return true;
    }

    return false;
}

DocumentVersion VcProjectDocumentX::documentVersion() const
{
    return DV_MSVC_2010;
}

void VcProjectDocumentX::processNode(const QDomNode &node)
{
    QDomNode domNode = node.firstChild();
    m_projectProcInstructionNode = domNode.toProcessingInstruction();

    QDomNode nextSibbling = domNode.nextSibling();
    m_project->processNode(nextSibbling);
}

VcNodeWidget *VcProjectDocumentX::createSettingsWidget()
{
    return nullptr;
}

QDomNode VcProjectDocumentX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    return m_project->toXMLDomNode(domXMLDocument);
}

IConfiguration *VcProjectDocumentX::createDefaultBuildConfiguration(const QString &fullConfigName) const
{
    QStringList clist = fullConfigName.split(QLatin1Char('|'));
    QString configDisplayName = clist[0];
    QString platformName = clist[1];

    QString condition = QLatin1String("\'%1%2%3\'==\'%4%5%6\'");
    condition = condition.arg(QLatin1String(VisualStudioProjectX::CONFIGURATION_VARIABLE)).
            arg(QLatin1String(VisualStudioProjectX::CONFIGURATION_PLATFORM_DELIMITER)).
            arg(QLatin1String(VisualStudioProjectX::PLATFORM_VARIABLE)).
            arg(configDisplayName).
            arg(QLatin1String(VisualStudioProjectX::CONFIGURATION_PLATFORM_DELIMITER)).
            arg(platformName);

    VisualStudioProjectX::ItemGroup *itemGroup = m_project->findItemGroupWithLabel(QLatin1String(VisualStudioProjectX::PROJECT_CONFIGURATIONS));

    if (!itemGroup) {
        itemGroup = new VisualStudioProjectX::ItemGroup;
        itemGroup->setLabel(QLatin1String(VisualStudioProjectX::PROJECT_CONFIGURATIONS));
        m_project->addItemGroup(itemGroup);
    }

    VisualStudioProjectX::ConfigurationX *newConfig = new VisualStudioProjectX::ConfigurationX(m_project);

    VisualStudioProjectX::Item *item = new VisualStudioProjectX::Item;
    itemGroup->addItem(item);
    item->setName(QLatin1String(VisualStudioProjectX::PROJECT_CONFIGURATION));

    QString include = QLatin1String("%1%2%3");
    include = include.arg(configDisplayName).
            arg(QLatin1String(VisualStudioProjectX::CONFIGURATION_PLATFORM_DELIMITER)).
            arg(platformName);
    item->setInclude(include);

    VisualStudioProjectX::ItemMetaData *newMetaData = new VisualStudioProjectX::ItemMetaData;
    newMetaData->setName(QLatin1String(VisualStudioProjectX::CONFIGURATION));
    newMetaData->setValue(configDisplayName);
    item->addItemMetaData(newMetaData);

    newMetaData = new VisualStudioProjectX::ItemMetaData;
    newMetaData->setName(QLatin1String(VisualStudioProjectX::PLATFORM));
    newMetaData->setValue(platformName);
    item->addItemMetaData(newMetaData);


    VisualStudioProjectX::PropertyGroup *newPropertyGroup = new VisualStudioProjectX::PropertyGroup;
    m_project->addPropertyGroup(newPropertyGroup);
    newPropertyGroup->setCondition(condition);

    newPropertyGroup = new VisualStudioProjectX::PropertyGroup(*newPropertyGroup);
    m_project->addPropertyGroup(newPropertyGroup);
    newPropertyGroup->setCondition(condition);
    newPropertyGroup->setLabel(QLatin1String(VisualStudioProjectX::CONFIGURATION));

    VisualStudioProjectX::ImportGroup *importGroup = new VisualStudioProjectX::ImportGroup;
    m_project->addImportGroup(importGroup);
    importGroup->setLabel(QLatin1String(VisualStudioProjectX::PROPERTY_SHEETS));
    importGroup->setCondition(condition);

    VisualStudioProjectX::ItemDefinitionGroup *itemDefinitionGroup = new VisualStudioProjectX::ItemDefinitionGroup;
    m_project->addItemDefinitionGroup(itemDefinitionGroup);
    itemDefinitionGroup->setCondition(condition);

    return newConfig;
}

IProjectFactories *VcProjectDocumentX::projectFactories() const
{
    return nullptr;
}

IVisualStudioProject *VcProjectDocumentX::clone() const
{
    return new VcProjectDocumentX(*this);
}

void VcProjectDocumentX::swap(VcProjectDocumentX &first, VcProjectDocumentX &second)
{
    std::swap(first.m_configurations, second.m_configurations);
    std::swap(first.m_filePath, second.m_filePath);
    std::swap(first.m_files, second.m_files);
    std::swap(first.m_filters, second.m_filters);
    std::swap(first.m_filtersProcInstructionNode, second.m_filtersProcInstructionNode);
    std::swap(first.m_project, second.m_project);
    std::swap(first.m_projectFactories, second.m_projectFactories);
    std::swap(first.m_projectProcInstructionNode, second.m_projectProcInstructionNode);
}

void VcProjectDocumentX::processFilterDoc()
{
    if (m_filePath.isEmpty())
        return;

    QFile file(m_filePath + QLatin1String(VisualStudioProjectX::FILTERS_EXTENSION));

    if (!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument domDocument(m_filePath + QLatin1String(VisualStudioProjectX::FILTERS_EXTENSION));
    domDocument.setContent(&file);

    QDomNode domNode = domDocument.firstChild();
    m_filtersProcInstructionNode = domNode.toProcessingInstruction();

    QDomNode nextSibbling = domNode.nextSibling();
    m_filters->processNode(nextSibbling);
}

} // namespace Internal
} // namespace VcProjectManager
