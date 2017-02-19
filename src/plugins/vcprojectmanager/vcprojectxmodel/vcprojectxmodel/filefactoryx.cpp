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
#include "filefactoryx.h"

#include "filex.h"
#include "filecontainerx.h"

#include "private/item.h"
#include "private/itemmetadata.h"
#include "vcprojectdocumentx.h"
#include "vcprojx_constants.h"

#include <QUuid>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

FileFactoryX::FileFactoryX(VcProjectDocumentX *parentProjectDocument)
    : m_parentProjectDocument(parentProjectDocument)
{
}

IFile *FileFactoryX::createFile(const QString &relativePath, ProjectExplorer::FileType fileType) const
{
    FileX *file = new FileX;
    file->setRelativePath(relativePath);
    file->m_parentProjectDocument = m_parentProjectDocument;
    file->m_project = m_parentProjectDocument->m_project;
    file->m_filters = m_parentProjectDocument->m_filters;
    file->m_projectItem = new Item;
    file->m_filterItem = new Item;

    switch (fileType) {
    case ProjectExplorer::FileType::Unknown:
        file->m_projectItem->setName(QLatin1String(FILE_ITEM_NONE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_NONE));
        break;
    case ProjectExplorer::FileType::Header:
        file->m_projectItem->setName(QLatin1String(FILE_ITEM_CL_INCLUDE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_CL_INCLUDE));
        break;
    case ProjectExplorer::FileType::Source:
        file->m_projectItem->setName(QLatin1String(FILE_ITEM_CL_COMPILE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_CL_COMPILE));
        break;
    case ProjectExplorer::FileType::Form:
        break;
    case ProjectExplorer::FileType::Resource:
        file->m_projectItem->setName(QLatin1String(FILE_ITEM_RESOURCE_COMPILE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_RESOURCE_COMPILE));
        break;
    case ProjectExplorer::FileType::QML:
        break;
    case ProjectExplorer::FileType::Project:
        break;
    case ProjectExplorer::FileType::FileTypeSize:
        break;
    default:
        break;
    }

    file->m_projectItem->setInclude(relativePath);
    file->m_filterItem->setInclude(relativePath);

    return file;
}

IFileContainer *FileFactoryX::createFileContainer(const QString &relativePath, const QString &containerType) const
{
    FileContainerX *newFileContainer = new FileContainerX;
    newFileContainer->m_filterItem = new Item;
    newFileContainer->m_filterItem->setName(containerType);
    newFileContainer->m_filterItem->setInclude(relativePath);

    ItemMetaData *newItemMetaData = new ItemMetaData;
    newItemMetaData->setName(QLatin1String("UniqueIdentifier"));
    newItemMetaData->setValue(QLatin1Char('{') + QUuid::createUuid().toString() + QLatin1Char('}'));
    newFileContainer->m_filterItem->addItemMetaData(newItemMetaData);

    return newFileContainer;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

