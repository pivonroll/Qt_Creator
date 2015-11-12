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
#include "filex.h"


#include "configurationx.h"
#include "private/item.h"
#include "vcprojectdocumentx.h"
#include "configurationcontainerx.h"
#include "tools/tool_constantsx.h"
#include "vcprojx_constants.h"
#include "../../utils.h"
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
    : m_item(0),
      m_filterItem(0),
      m_project(0),
      m_filters(0),
      m_parentProjectDocument(0)
{}

FileX::FileX(const FileX &file)
{
    m_item = new Item(*file.m_item);
    m_filterItem = new Item(*file.m_filterItem);
    m_project = file.m_project;
    m_filters = file.m_filters;
    m_parentProjectDocument = file.m_parentProjectDocument;
}

FileX::~FileX()
{
    if (!Utils::findItemGroupContainingItem(m_item, m_project))
        delete m_item;

    if (!Utils::findItemGroupContainingItem(m_filterItem, m_filters))
        delete m_filterItem;
}

QString FileX::relativePath() const
{
    if (m_item)
        return m_item->include();
    return QString();
}

void FileX::setRelativePath(const QString &relativePath)
{
    if (m_item)
        m_item->setInclude(relativePath);
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
    return 0;
}

IAttributeContainer *FileX::attributeContainer() const
{
    return 0;
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
    return 0;
}

QDomNode FileX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);
    return QDomNode();
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
