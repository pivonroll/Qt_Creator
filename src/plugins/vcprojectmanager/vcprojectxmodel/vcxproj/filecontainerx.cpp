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
#include "filecontainerx.h"

#include "../../vcprojectmanagerconstants.h"
#include "filex.h"

#include "private/item.h"
#include "private/itemgroup.h"
#include "private/itemmetadata.h"
#include "private/project.h"
#include "vcprojx_constants.h"

#include <utils/qtcassert.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

FileContainerX::FileContainerX(const FileContainerX &fileCont)
{
    m_project = fileCont.m_project;
    m_filters = fileCont.m_filters;
}

FileContainerX &FileContainerX::operator=(const FileContainerX &fileCont)
{
    if (this != &fileCont) {
        m_files.clear();
        m_fileContainers.clear();

        m_project = fileCont.m_project;
        m_filters = fileCont.m_filters;

        foreach (IFile *file, fileCont.m_files) {
            if (file)
                m_files.append(file->clone());
        }

        foreach (IFileContainer *fileContainer, fileCont.m_fileContainers) {
            if (fileContainer)
                m_fileContainers.append(fileContainer->clone());
        }
    }

    return *this;
}

FileContainerX::~FileContainerX()
{
    qDeleteAll(m_files);
    qDeleteAll(m_fileContainers);
}

QString FileContainerX::containerType() const
{
    return QLatin1String(Constants::VC_PROJECT_FILE_CONTAINER_FILTER);
}

void FileContainerX::addFile(IFile *file)
{
    FileX *fileX = dynamic_cast<FileX *>(file);

    if (fileX) {
        if (fileX->m_filterItem) {
            ItemGroup *itemGroup = findProperItemGroup(fileX->m_filterItem->name(), m_filters);

            if (!itemGroup)
                itemGroup = new ItemGroup;
            itemGroup->addItem(fileX->m_filterItem);

            itemGroup = findProperItemGroup(fileX->m_filterItem->name(), m_project);

            if (!itemGroup)
                itemGroup = new ItemGroup;
            itemGroup->addItem(fileX->m_filterItem);
        }
        m_files.append(file);
    }
}

IFile *FileContainerX::file(int index) const
{
    QTC_ASSERT(0 <= index && index < m_files.size(), return 0);
    return m_files[index];
}

int FileContainerX::fileCount() const
{
    return m_files.size();
}

void FileContainerX::removeFile(IFile *file)
{
    FileX *fileX = dynamic_cast<FileX *>(file);

    if (fileX) {
        if (fileX->m_filterItem) {
            ItemGroup *itemGroup = findItemGroup(fileX->m_filterItem, m_filters);

            if (itemGroup)
                itemGroup->removeItem(fileX->m_filterItem);

            if (!itemGroup->itemCount())
                m_filters->removeItemGroup(itemGroup);

            itemGroup = findItemGroup(fileX->m_filterItem, m_project);

            if (itemGroup)
                itemGroup->removeItem(fileX->m_filterItem);

            if (!itemGroup->itemCount())
                m_project->removeItemGroup(itemGroup);
        }
        m_files.removeOne(fileX);
    }
}

void FileContainerX::addFileContainer(IFileContainer *fileContainer)
{
    FileContainerX *fileContX = dynamic_cast<FileContainerX *>(fileContainer);

    if (fileContX) {
        QTC_ASSERT(fileContX->m_filterItem, return);

        fileContX->m_filters = m_filters;
        fileContX->m_project = m_project;
        fileContX->m_parentContainer = this;

        FileContainerX *parentCont = m_parentContainer;
        QString relativePath;
        while (parentCont) {
            relativePath.prepend(parentCont->displayName() + QLatin1Char('\\'));
            parentCont = parentCont->m_parentContainer;
        }

        fileContX->m_filterItem->setName(QLatin1String(FILTER_ITEM));

        // if for some reason some other file container created fileContX file container we need to set include path properly
        QStringList stringList = fileContX->m_filterItem->include().split(QLatin1Char('\\'));
        fileContX->m_filterItem->setInclude(relativePath + QLatin1Char('\\') + stringList.last());

        ItemGroup *filterGroup = findProperItemGroup(QLatin1String(FILTER_ITEM), m_filters);

        if (!filterGroup) {
            filterGroup = new ItemGroup;
            m_filters->addItemGroup(filterGroup);
        }

        filterGroup->addItem(fileContX->m_filterItem);

        m_fileContainers.append(fileContainer);
    }
}

int FileContainerX::childCount() const
{
    return m_fileContainers.size();
}

IFileContainer *FileContainerX::fileContainer(int index) const
{
    QTC_ASSERT(0 <= index && index < m_fileContainers.size(), return 0);
    return m_fileContainers[index];
}

void FileContainerX::removeFileContainer(IFileContainer *fileContainer)
{
    FileContainerX *fileContainerX = dynamic_cast<FileContainerX *>(fileContainer);

    if (fileContainerX) {
        // remove all files and filters that belong to this filter
        QString include;
        FileContainerX *parentCont = fileContainerX->m_parentContainer;

        while (parentCont) {
            include.prepend(parentCont->displayName() + QLatin1Char('\\'));
            parentCont = parentCont->m_parentContainer;
        }

        QStringList relativePaths;
        removeAllFilesInAFilter(include, relativePaths);
        removeAllFiltersInAFilter(include);
        removeFilesFromProject(relativePaths);
    }
}

IAttributeContainer *FileContainerX::attributeContainer() const
{
    return 0;
}

QString FileContainerX::displayName() const
{
    if (m_filterItem) {
        QStringList stringList = m_filterItem->include().split(QLatin1Char('\\'));
        return stringList.last();
    }
    return QString();
}

void FileContainerX::setDisplayName(const QString &displayName)
{
    if (m_filterItem) {
        QStringList stringList = m_filterItem->include().split(QLatin1Char('\\'));
        stringList.last() = displayName;
        m_filterItem->setInclude(stringList.join(QLatin1Char('\\')));
    }
}

void FileContainerX::allFiles(QStringList &sl) const
{
    foreach (IFileContainer *filter, m_fileContainers)
        filter->allFiles(sl);

    foreach (IFile *file, m_files)
        sl.append(file->canonicalPath());
}

bool FileContainerX::fileExists(const QString &relativeFilePath) const
{
    for (int i = 0; i < m_files.size(); ++i) {
        IFile *file = m_files[i];

        if (file && file->relativePath() == relativeFilePath)
            return true;
    }
    return false;
}

IFileContainer *FileContainerX::clone() const
{
    return new FileContainerX(*this);
}

QString FileContainerX::relativePath() const
{
    if (m_filterItem)
        return m_filterItem->include();
    return QString();
}

void FileContainerX::setRelativePath(const QString &relativePath)
{
    if (m_filterItem)
        m_filterItem->setInclude(relativePath);
}

IFile *FileContainerX::createNewFile(const QString &relativePath, ProjectExplorer::FileType fileType)
{
    FileX *file = new FileX();
    file->m_project = m_project;
    file->m_filters = m_filters;
    file->m_item = new Item;
    file->m_filterItem = new Item;

    switch (fileType) {
    case ProjectExplorer::UnknownFileType:
        file->m_item->setName(QLatin1String(FILE_ITEM_NONE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_NONE));
        break;
    case ProjectExplorer::HeaderType:
        file->m_item->setName(QLatin1String(FILE_ITEM_CL_INCLUDE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_CL_INCLUDE));
        break;
    case ProjectExplorer::SourceType:
        file->m_item->setName(QLatin1String(FILE_ITEM_CL_COMPILE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_CL_COMPILE));
        break;
    case ProjectExplorer::FormType:
        break;
    case ProjectExplorer::ResourceType:
        file->m_item->setName(QLatin1String(FILE_ITEM_RESOURCE_COMPILE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_RESOURCE_COMPILE));
        break;
    case ProjectExplorer::QMLType:
        break;
    case ProjectExplorer::ProjectFileType:
        break;
    case ProjectExplorer::FileTypeSize:
        break;
    default:
        break;
    }

    file->m_item->setInclude(relativePath);
    file->m_filterItem->setInclude(relativePath);

    if (m_filterItem) {
        ItemMetaData *itemMetaData = new ItemMetaData;
        itemMetaData->setName(QLatin1String(FILTER_ITEM));
        itemMetaData->setValue(m_filterItem->include());
        file->m_filterItem->addItemMetaData(itemMetaData);
    }

    return file;
}

IFileContainer *FileContainerX::createNewFileContainer(const QString &containerType, const QString &displayName)
{
    FileContainerX *fileContainer = new FileContainerX;
    fileContainer->m_project = m_project;
    fileContainer->m_filters = m_filters;
    fileContainer->m_parentContainer = this;
    fileContainer->m_filterItem = new Item;
    fileContainer->m_filterItem->setName(containerType);

    FileContainerX *parentCont = m_parentContainer;
    QString relativePath;
    while (parentCont) {
        relativePath.prepend(parentCont->displayName() + QLatin1Char('\\'));
        parentCont = parentCont->m_parentContainer;
    }

    fileContainer->m_filterItem->setInclude(relativePath + QLatin1Char('\\') + displayName);
    ItemMetaData *itemMetaData = new ItemMetaData;
    itemMetaData->setName(QLatin1String(UNIQUE_IDENTIFIER));
    // TODO(Radovan): add unique identifier id value
    fileContainer->m_filterItem->addItemMetaData(itemMetaData);

    return fileContainer;
}

void FileContainerX::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

VcNodeWidget *FileContainerX::createSettingsWidget()
{
    return 0;
}

QDomNode FileContainerX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);
    return QDomNode();
}

FileContainerX::FileContainerX()
    : m_filterItem(0),
      m_project(0),
      m_filters(0),
      m_parentContainer(0)
{
}

ItemGroup *FileContainerX::findProperItemGroup(const QString &itemName, Project *docProject) const
{
    if (docProject) {
        for (int i = 0; i < docProject->itemGroupCount(); ++i) {
            ItemGroup *itemGroup = docProject->itemGroup(i);

            if (itemGroup && itemGroupContainsItemName(itemName, itemGroup))
                return itemGroup;
        }
    }

    return 0;
}

ItemGroup *FileContainerX::findItemGroup(Item *item, Project *docProject) const
{
    if (item && docProject) {
        for (int i = 0; i < docProject->itemGroupCount(); ++i) {
            ItemGroup *itemGroup = docProject->itemGroup(i);

            if (itemGroup && groupContainsItem(item, itemGroup))
                return itemGroup;
        }
    }

    return 0;
}

bool FileContainerX::groupContainsItem(Item *item, ItemGroup *itemGroup) const
{
    if (item && itemGroup) {
        for (int i = 0; i < itemGroup->itemCount(); ++i) {
            Item *it = itemGroup->item(i);

            if (it && it->name() == item->name() && it->include() == item->include())
                return true;
        }
    }

    return false;
}

bool FileContainerX::itemGroupContainsItemName(const QString &itemName, ItemGroup *itemGroup) const
{
    if (itemGroup) {
        for (int i = 0; i < itemGroup->itemCount(); ++i) {
            Item *item = itemGroup->item(i);

            if (item && item->name() == itemName)
                return true;
        }
    }

    return false;
}

void FileContainerX::removeAllFilesInAFilter(const QString &fullFilterName, QStringList &relativePaths)
{
    for (int i = 0; i < m_filters->itemGroupCount(); ++i) {
        ItemGroup *itemGroup = m_filters->itemGroup(i);

        if (itemGroup) {
            removeAllFilesInAFilter(itemGroup, fullFilterName, relativePaths);
        }
    }
}

void FileContainerX::removeAllFilesInAFilter(ItemGroup *itemGroup, const QString &fullFilterName, QStringList &relativePaths)
{
    if (itemGroup) {
        for (int i = 0; i < itemGroup->itemCount();) {
            Item *item = itemGroup->item(i);

            if (item && isFileInAFilter(item, fullFilterName)) {
                relativePaths.append(item->include());
                itemGroup->removeItem(item);
            }
            else
                ++i;
        }
    }
}

bool FileContainerX::isFileInAFilter(Item *item, const QString &fullFilterName)
{
    if (item) {
        for (int i = 0; i < item->itemMetaDataCount(); ++i) {
            ItemMetaData *itemMetaData = item->itemMetaData(i);

            if (itemMetaData && itemMetaData->name() == QLatin1String(FILTER_ITEM) && itemMetaData->value().startsWith(fullFilterName))
                return true;
        }
    }

    return false;
}

void FileContainerX::removeAllFiltersInAFilter(const QString &fullFilterName)
{
    ItemGroup *itemGroup = findProperItemGroup(QLatin1String(FILTER_ITEM), m_filters);

    if (itemGroup) {
        for (int i = 0; i < itemGroup->itemCount();) {
            Item *item = itemGroup->item(i);

            if (item && item->include().startsWith(fullFilterName))
                itemGroup->removeItem(item);

            else
                ++i;
        }
    }
}

void FileContainerX::removeFilesFromProject(const QStringList &relativePaths)
{
    for (int i = 0; i < m_project->itemGroupCount(); ++i) {
        ItemGroup *itemGroup = m_project->itemGroup(i);

        if (itemGroup) {
            removeFileFromItemGroup(itemGroup, relativePaths);
        }
    }
}

void FileContainerX::removeFileFromItemGroup(ItemGroup *itemGroup, const QStringList &relativePaths)
{
    if (itemGroup) {
        for (int i = 0; i < itemGroup->itemCount();) {
            Item *item = itemGroup->item(i);

            if (item && relativePaths.contains(item->include()))
                itemGroup->removeItem(item);
            else
                ++i;
        }
    }
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
