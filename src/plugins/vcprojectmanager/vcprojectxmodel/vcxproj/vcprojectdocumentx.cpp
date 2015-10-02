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

VcProjectDocumentX::~VcProjectDocumentX()
{
    delete m_project;
    delete m_filters;
    delete m_files;
    delete m_projectFactories;
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
    return 0;
}

IPlatforms *VcProjectDocumentX::platforms() const
{
    return 0;
}

IReferences *VcProjectDocumentX::referencess() const
{
    return 0;
}

IAttributeContainer *VcProjectDocumentX::attributeContainer() const
{
    return 0;
}

IToolFiles *VcProjectDocumentX::toolFiles() const
{
    return 0;
}

IPublishingData *VcProjectDocumentX::publishingData() const
{
    return 0;
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

VcDocConstants::DocumentVersion VcProjectDocumentX::documentVersion() const
{
    return VcDocConstants::DV_MSVC_2010;
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
    return 0;
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

    VisualStudioProjectX::ConfigurationX *newConfig = new VisualStudioProjectX::ConfigurationX;
    newConfig->m_project = m_project;
    newConfig->m_item = new VisualStudioProjectX::Item;
    newConfig->m_item->setName(QLatin1String(VisualStudioProjectX::PROJECT_CONFIGURATION));

    QString include = QLatin1String("%1%2%3");
    include = include.arg(configDisplayName).
            arg(QLatin1String(VisualStudioProjectX::CONFIGURATION_PLATFORM_DELIMITER)).
            arg(platformName);
    newConfig->m_item->setInclude(include);

    VisualStudioProjectX::ItemMetaData *newMetaData = new VisualStudioProjectX::ItemMetaData;
    newMetaData->setName(QLatin1String(VisualStudioProjectX::CONFIGURATION));
    newMetaData->setValue(configDisplayName);
    newConfig->m_item->addItemMetaData(newMetaData);

    newMetaData = new VisualStudioProjectX::ItemMetaData;
    newMetaData->setName(QLatin1String(VisualStudioProjectX::PLATFORM));
    newMetaData->setValue(platformName);
    newConfig->m_item->addItemMetaData(newMetaData);


    VisualStudioProjectX::PropertyGroup *newPropertyGroup = new VisualStudioProjectX::PropertyGroup;
    newPropertyGroup->setCondition(condition);
    newConfig->m_propertyGroups.append(newPropertyGroup);

    newPropertyGroup = new VisualStudioProjectX::PropertyGroup(*newPropertyGroup);
    newPropertyGroup->setLabel(QLatin1String(VisualStudioProjectX::CONFIGURATION));
    newConfig->m_propertyGroups.append(newPropertyGroup);

    VisualStudioProjectX::ImportGroup *importGroup = new VisualStudioProjectX::ImportGroup;
    importGroup->setLabel(QLatin1String(VisualStudioProjectX::PROPERTY_SHEETS));
    importGroup->setCondition(condition);
    newConfig->m_importGroups.append(importGroup);

    newConfig->m_itemDefinitionGroup = new VisualStudioProjectX::ItemDefinitionGroup;
    newConfig->m_itemDefinitionGroup->setCondition(condition);

    return newConfig;
}

IProjectFactories *VcProjectDocumentX::projectFactories() const
{
    return 0;
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
