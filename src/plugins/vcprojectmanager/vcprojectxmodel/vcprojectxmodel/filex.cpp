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
#include "filex.h"


#include "private/item.h"
#include "private/itemmetadata.h"

#include "configurationx.h"
#include "vcprojectdocumentx.h"
#include "configurationcontainerx.h"
#include "tools/tool_constantsx.h"
#include "vcprojx_constants.h"
#include "../../utils/utils.h"
#include "tools/configurationtoolx.h"

#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/itools.h>

#include <visualstudiotoolattributes/tooldescription.h>

#include "utilsx.h"

#include <utils/fileutils.h>

#include <QFileInfo>
#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

FileX::FileX()
    : m_projectItem(nullptr),
      m_filterItem(nullptr),
      m_project(nullptr),
      m_filters(nullptr),
      m_parentProjectDocument(nullptr)
{}

FileX::FileX(const FileX &other)
{
    m_projectItem = new Item(*other.m_projectItem);
    m_filterItem = new Item(*other.m_filterItem);
    m_project = other.m_project;
    m_filters = other.m_filters;
    m_parentProjectDocument = other.m_parentProjectDocument;
}

FileX::FileX(FileX &&other)
    : FileX()
{
    swap(*this, other);
}

FileX &FileX::operator=(FileX other)
{
    swap(*this, other);
    return *this;
}

FileX::~FileX()
{
    if (!Utils::findItemGroupContainingItem(m_projectItem, m_project))
        delete m_projectItem;

    if (!Utils::findItemGroupContainingItem(m_filterItem, m_filters))
        delete m_filterItem;
}

QString FileX::displayName() const
{
    QString relativePath = m_projectItem->include();
    int index = relativePath.lastIndexOf(QLatin1Char('\\'));
    if (index == -1)
        return relativePath;

    return relativePath.mid(index + 1);
}

QString FileX::relativePath() const
{
    if (m_projectItem)
        return m_projectItem->include();
    return QString();
}

void FileX::setRelativePath(const QString &relativePath)
{
    if (m_projectItem)
        m_projectItem->setInclude(relativePath);
}

QString FileX::canonicalPath() const
{
    if (m_parentProjectDocument) {
        QFileInfo fileInfo(m_parentProjectDocument->filePath());
        fileInfo = QFileInfo(fileInfo.canonicalPath() + QLatin1Char('/') + relativePath());
        return fileInfo.canonicalFilePath();
    }

    return relativePath();
}

IConfigurationContainer *FileX::configurationContainer() const
{
    return nullptr;
}

IAttributeContainer *FileX::attributeContainer() const
{
    return nullptr;
}

IFile *FileX::clone() const
{
    return new FileX(*this);
}

ProjectExplorer::FileType FileX::fileType() const
{
    return VcProjectManager::Internal::VisualStudioUtils::getFileType(::Utils::FileName::fromString(canonicalPath()));
}

IConfiguration *FileX::createDefaultBuildConfiguration(const QString &configName, const QString &platformName) const
{
    QStringList strings = configName.split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));
    IConfiguration *config = configurationContainer()->createNewConfiguration(strings[0], strings[1]);

    if (config) {
        ConfigurationX *configurationX = static_cast<ConfigurationX *>(config);
        config->setDisplayName(configName);
        config->setPlatform(platformName);

        ToolDescriptionDataManager *tDDM = ToolDescriptionDataManager::instance();
        ToolDescription *toolDesc = tDDM->toolDescription(QLatin1String(ToolConstantsx::TOOL_CL_COMPILE));

        if (toolDesc)
            config->tools()->configurationBuildTools()->addTool(new ConfigurationToolX(toolDesc, m_project, configurationX));
    }
    return config;
}

void FileX::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

VcNodeWidget *FileX::createSettingsWidget()
{
    return nullptr;
}

QDomNode FileX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);
    return QDomNode();
}

FileX *FileX::createNewFile(const QString &relativePath, const QString &filterPath)
{
    QString itemName;
    if (relativePath.endsWith(QLatin1String(".h")))
        itemName = QLatin1String("ClInclude");
    else if (relativePath.endsWith(QLatin1String(".cpp")))
        itemName = QLatin1String("ClCompile");
    else if (relativePath.endsWith(QLatin1String(".txt")))
        itemName = QLatin1String("Text");

    FileX *newFileItem = new FileX();
    newFileItem->m_filterItem = new Item;
    newFileItem->m_filterItem->setInclude(relativePath);
    newFileItem->m_filterItem->setName(itemName);
    ItemMetaData *filterItemMetaData = new ItemMetaData;
    newFileItem->m_filterItem->addItemMetaData(filterItemMetaData);
    filterItemMetaData->setName(QLatin1String("Filter"));
    filterItemMetaData->setValue(filterPath);

    newFileItem->m_projectItem = new Item;
    newFileItem->m_projectItem->setInclude(relativePath);
    newFileItem->m_projectItem->setName(itemName);

    return newFileItem;
}

void FileX::swap(FileX &first, FileX &second)
{
    std::swap(first.m_filterItem, second.m_filterItem);
    std::swap(first.m_filters, second.m_filters);
    std::swap(first.m_projectItem, second.m_projectItem);
    std::swap(first.m_parentProjectDocument, second.m_parentProjectDocument);
    std::swap(first.m_project, second.m_project);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
