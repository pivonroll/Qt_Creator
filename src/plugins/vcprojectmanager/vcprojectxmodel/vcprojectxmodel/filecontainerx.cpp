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
#include "filecontainerx.h"

#include "../../common/vcprojectmanagerconstants.h"
#include "filex.h"

#include "private/item.h"
#include "private/itemgroup.h"
#include "private/itemmetadata.h"
#include "private/project.h"
#include "vcprojx_constants.h"
#include "utilsx.h"

#include <utils/qtcassert.h>

#include <QDomNode>
#include <QUuid>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

FileContainerX::FileContainerX(const FileContainerX &other)
{
    m_filterItem = new Item(*other.m_filterItem);
    m_project = other.m_project;
    m_filters = other.m_filters;
    m_parentContainer = other.m_parentContainer;
}

FileContainerX::FileContainerX(FileContainerX &&other)
    : FileContainerX()
{
    swap(*this, other);
}

FileContainerX &FileContainerX::operator=(FileContainerX other)
{
    swap(*this, other);
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

    if (!fileX)
        return;

    // insert into filters
    ItemGroup *itemGroup = Utils::findItemGroupWhichContainsItemWithName(fileX->m_filterItem->name(),
                                                                         m_filters);
    itemGroup->addItem(fileX->m_filterItem);
    fileX->m_filters = m_filters;

    // insert into projects
    itemGroup = Utils::findItemGroupWhichContainsItemWithName(fileX->m_filterItem->name(),
                                                              m_project);
    itemGroup->addItem(fileX->m_projectItem);
    fileX->m_project = m_project;

    m_files.append(file);
}

IFile *FileContainerX::file(int index) const
{
    QTC_ASSERT(0 <= index && index < m_files.size(), return nullptr);
    return m_files[index];
}

IFile *FileContainerX::file(const QString &relativePath) const
{
    foreach (IFile *file, m_files) {
        if (file->relativePath() == relativePath)
            return file;
    }

    return nullptr;
}

int FileContainerX::fileCount() const
{
    return m_files.size();
}

void FileContainerX::removeFile(IFile *file)
{
    FileX *fileX = dynamic_cast<FileX *>(file);

    if (!fileX)
        return;

    if (fileX->m_filterItem) {
        ItemGroup *itemGroup = VisualStudioProjectX::Utils::findItemGroupContainingItem(fileX->m_filterItem, m_filters);

        if (itemGroup)
            itemGroup->removeItem(fileX->m_filterItem);

        if (!itemGroup->itemCount())
            m_filters->removeItemGroup(itemGroup);

        itemGroup = VisualStudioProjectX::Utils::findItemGroupContainingItem(fileX->m_filterItem, m_project);

        if (itemGroup)
            itemGroup->removeItem(fileX->m_filterItem);

        if (!itemGroup->itemCount())
            m_project->removeItemGroup(itemGroup);
    }
    m_files.removeOne(fileX);
}

void FileContainerX::addFileContainer(IFileContainer *fileContainer)
{
    FileContainerX *fileContX = dynamic_cast<FileContainerX *>(fileContainer);

    if (!fileContX)
        return;

    ItemGroup *itemGroup = Utils::findItemGroupWhichContainsItemWithName(fileContX->m_filterItem->name(),
                                                                         m_filters);
    itemGroup->addItem(fileContX->m_filterItem);

    fileContX->m_filters = m_filters;
    fileContX->m_project = m_project;

    m_fileContainers.append(fileContainer);
}

int FileContainerX::childCount() const
{
    return m_fileContainers.size();
}

IFileContainer *FileContainerX::fileContainer(int index) const
{
    QTC_ASSERT(0 <= index && index < m_fileContainers.size(), return nullptr);
    return m_fileContainers[index];
}

void FileContainerX::removeFileContainer(IFileContainer *fileContainer)
{
    FileContainerX *fileContainerX = dynamic_cast<FileContainerX *>(fileContainer);

    if (!fileContainerX)
        return;

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

IAttributeContainer *FileContainerX::attributeContainer() const
{
    return nullptr;
}

QString FileContainerX::displayName() const
{
    if (!m_filterItem)
        return QString();

    QStringList stringList = m_filterItem->include().split(QLatin1Char('\\'));
    return stringList.last();
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

IFile *FileContainerX::findFile(const QString &canonicalFilePath) const
{
    foreach (IFile *file, m_files) {
        if (file->canonicalPath() ==  canonicalFilePath)
            return file;
    }

    foreach (IFileContainer *fileContainer, m_fileContainers) {
        IFile *file = fileContainer->findFile(canonicalFilePath);
        if (file)
            return file;
    }

    return nullptr;
}

IFileContainer *FileContainerX::findFileContainer(const QString &relativePath) const
{
    if (relativePath.isEmpty())
        return nullptr;

    foreach (IFileContainer *fileCont, m_fileContainers) {
        if (fileCont->relativePath() == relativePath)
            return fileCont;

        IFileContainer *cont = fileCont->findFileContainer(relativePath);

        if (cont)
            return cont;
    }

    return nullptr;
}

void FileContainerX::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

VcNodeWidget *FileContainerX::createSettingsWidget()
{
    return nullptr;
}

QDomNode FileContainerX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);
    return QDomNode();
}

FileContainerX::FileContainerX()
    : m_filterItem(nullptr),
      m_project(nullptr),
      m_filters(nullptr),
      m_parentContainer(nullptr)
{
}

void FileContainerX::swap(FileContainerX &first, FileContainerX &second)
{
    std::swap(first.m_fileContainers, second.m_fileContainers);
    std::swap(first.m_files, second.m_files);
    std::swap(first.m_filterItem, second.m_filterItem);
    std::swap(first.m_filters, second.m_filters);
    std::swap(first.m_parentContainer, second.m_parentContainer);
    std::swap(first.m_project, second.m_project);
}

void FileContainerX::removeAllFilesInAFilter(const QString &fullFilterName, QStringList &relativePaths)
{
    for (int i = 0; i < m_filters->itemGroupCount(); ++i) {
        ItemGroup *itemGroup = m_filters->itemGroup(i);

        if (itemGroup)
            removeAllFilesInAFilter(itemGroup, fullFilterName, relativePaths);
    }
}

void FileContainerX::removeAllFilesInAFilter(ItemGroup *itemGroup, const QString &fullFilterName, QStringList &relativePaths)
{
    if (!itemGroup)
        return;

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

bool FileContainerX::isFileInAFilter(Item *item, const QString &fullFilterName)
{
    if (!item)
        return false;

    for (int i = 0; i < item->itemMetaDataCount(); ++i) {
        ItemMetaData *itemMetaData = item->itemMetaData(i);

        if (itemMetaData && itemMetaData->name() == QLatin1String(FILTER_ITEM) && itemMetaData->value().startsWith(fullFilterName))
            return true;
    }

    return false;
}

void FileContainerX::removeAllFiltersInAFilter(const QString &fullFilterName)
{
    ItemGroup *itemGroup = VisualStudioProjectX::Utils::findItemGroupWhichContainsItemWithName(QLatin1String(FILTER_ITEM), m_filters);

    if (!itemGroup)
        return;

    for (int i = 0; i < itemGroup->itemCount();) {
        Item *item = itemGroup->item(i);

        if (item && item->include().startsWith(fullFilterName))
            itemGroup->removeItem(item);

        else
            ++i;
    }
}

void FileContainerX::removeFilesFromProject(const QStringList &relativePaths)
{
    for (int i = 0; i < m_project->itemGroupCount(); ++i) {
        ItemGroup *itemGroup = m_project->itemGroup(i);

        if (itemGroup)
            removeFileFromItemGroup(itemGroup, relativePaths);
    }
}

void FileContainerX::removeFileFromItemGroup(ItemGroup *itemGroup, const QStringList &relativePaths)
{
    if (!itemGroup)
        return;

    for (int i = 0; i < itemGroup->itemCount();) {
        Item *item = itemGroup->item(i);

        if (item && relativePaths.contains(item->include()))
            itemGroup->removeItem(item);
        else
            ++i;
    }
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
