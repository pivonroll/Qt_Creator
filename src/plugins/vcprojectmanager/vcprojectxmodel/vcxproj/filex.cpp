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

#include "../../vcprojectmodel/tools/toolattributes/tooldescriptiondatamanager.h"
#include "../../interfaces/iconfigurationbuildtool.h"
#include "../../interfaces/iconfigurationbuildtools.h"
#include "../../interfaces/itools.h"
#include "../../interfaces/itooldescription.h"

#include <QFileInfo>
#include <QDomNode>

#include <utils/mimetypes/mimedatabase.h>
#include <projectexplorer/projectexplorerconstants.h>

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
{}

QString FileX::relativePath() const
{
    if (m_item)
        return m_item->include();
    return QString();
}

void FileX::setRelativePath(const QString &path)
{
    if (m_item)
        m_item->setInclude(path);
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
    ::Utils::MimeDatabase mdb;
    QString mimeType = mdb.mimeTypeForFile(canonicalPath()).name();

    if (mimeType == QLatin1String(ProjectExplorer::Constants::CPP_SOURCE_MIMETYPE)
            || mimeType == QLatin1String(ProjectExplorer::Constants::C_SOURCE_MIMETYPE))
        return ProjectExplorer::SourceType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::CPP_HEADER_MIMETYPE)
            || mimeType == QLatin1String(ProjectExplorer::Constants::C_HEADER_MIMETYPE))
        return ProjectExplorer::HeaderType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::RESOURCE_MIMETYPE))
        return ProjectExplorer::ResourceType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::FORM_MIMETYPE))
        return ProjectExplorer::FormType;
    if (mimeType == QLatin1String(ProjectExplorer::Constants::QML_MIMETYPE))
        return ProjectExplorer::QMLType;

    return ProjectExplorer::UnknownFileType;
}

IConfiguration *FileX::createDefaultBuildConfiguration(const QString &configName, const QString &platformName) const
{
    QStringList strings = configName.split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));
    IConfiguration *config = configurationContainer()->createNewConfiguration(strings[0], strings[1]);

    if (config) {
        config->setDisplayName(configName);
        config->setPlatform(platformName);

        ToolDescriptionDataManager *tDDM = ToolDescriptionDataManager::instance();
        IToolDescription *toolDesc = tDDM->toolDescription(QLatin1String(ToolConstantsx::TOOL_CL_COMPILE));

        if (toolDesc) {
            IConfigurationBuildTool *tool = toolDesc->createTool();
            config->tools()->configurationBuildTools()->addTool(tool);
        }
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
