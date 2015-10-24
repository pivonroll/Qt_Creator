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
#include "filesx.h"

#include <QDomNode>

#include <visualstudiointerfaces/ifile.h>
#include "private/project.h"
#include "private/itemgroup.h"
#include "private/item.h"
#include "private/itemmetadata.h"
#include "filecontainerx.h"
#include "filex.h"
#include "utilsx.h"
#include "vcprojx_constants.h"

#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

FilesX::~FilesX()
{
}

void FilesX::addFile(IFile *file)
{
    m_private->addFile(file);
}

void FilesX::removeFile(IFile *file)
{
    m_private->removeFile(file);
}

int FilesX::fileCount() const
{
    return m_private->fileCount();
}

IFile *FilesX::file(int index) const
{
    return m_private->file(index);
}

IFile *FilesX::file(const QString &relativePath) const
{
    for (int i = 0; i < m_private->fileCount(); ++i) {
        IFile *file = m_private->file(i);

        if (file && file->relativePath() == relativePath)
            return file;
    }
    return 0;
}

bool FilesX::fileExists(const QString &relativeFilePath) const
{
    return m_private->fileExists(relativeFilePath);
}

void FilesX::addFileContainer(IFileContainer *fileContainer)
{
    m_private->addFileContainer(fileContainer);
}

int FilesX::fileContainerCount() const
{
    return m_private->childCount();
}

IFileContainer *FilesX::fileContainer(int index) const
{
    return m_private->fileContainer(index);
}

void FilesX::removeFileContainer(IFileContainer *fileContainer)
{
    m_private->removeFileContainer(fileContainer);
}

void FilesX::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

VcNodeWidget *FilesX::createSettingsWidget()
{
    return 0;
}

QDomNode FilesX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);
    return QDomNode();
}

FilesX::FilesX(Project *project, Project *filters, VcProjectDocumentX *parentProject)
    : m_project(project),
      m_filters(filters),
      m_private(new FileContainerX),
      m_parentProject(parentProject)
{
    m_private->m_filters = m_filters;
    m_private->m_project = m_project;
    m_private->m_filterItem = 0;
    m_private->m_parentContainer = 0;

    readFiles();
}

void FilesX::readFileContainers()
{
    ItemGroup *itemGroup = Utils::findItemGroupWithName(QLatin1String(FILTER_ITEM), m_filters);

    if (!itemGroup)
        return;

    for (int i = 0; i < itemGroup->itemCount(); ++i) {
        Item *item = itemGroup->item(i);

        if (item && item->name() == QLatin1String(FILTER_ITEM)) {
            QStringList pathList = item->include().split(QLatin1Char('\\'));

            createFileContainer(itemGroup, pathList);
        }
    }
}

void FilesX::createFileContainer(ItemGroup *itemGroup, const QStringList &pathList)
{
    if (pathList.isEmpty())
        return;

    QStringList relativePath;
    FileContainerX *container = m_private;

    foreach (const QString &path, pathList) {
        relativePath << path;
        FileContainerX *fileContainer = findFileContainer(container, path);

        if (!fileContainer) {
            fileContainer = new FileContainerX;
            Item *item = Utils::findItemInGroup(relativePath.join(QLatin1Char('\\')), itemGroup);

            if (item)
                fileContainer->m_filterItem = item;

            else {
                item = new Item(relativePath.join(QLatin1Char('\\')));
                item->setName(QLatin1String(FILTER_ITEM));
                itemGroup->addItem(item);
            }

            fileContainer->setDisplayName(path);
            fileContainer->setRelativePath(relativePath.join(QLatin1Char('\\')));
            container->addFileContainer(fileContainer);
        }

        container = fileContainer;
    }
}

FileContainerX *FilesX::findFileContainer(IFileContainer *container, const QString &containerName) const
{
    if (!container)
        return 0;

    for (int i = 0; i < container->childCount(); ++i) {
        FileContainerX *childContainer = dynamic_cast<FileContainerX *>(container->fileContainer(i));

        if (childContainer && childContainer->displayName() == containerName)
            return childContainer;
    }

    return 0;
}

FileContainerX *FilesX::findFileContainerRecursive(IFileContainer *container, const QString &containerName) const
{
    if (!container)
        return 0;

    FileContainerX *childContainer = findFileContainer(container, containerName);

    if (childContainer)
        return childContainer;

    for (int i = 0; i < container->childCount(); ++i) {
        IFileContainer *cont = container->fileContainer(i);

        FileContainerX *foundCont = findFileContainerRecursive(cont, containerName);

        if (foundCont)
            return foundCont;
    }

    return 0;
}

FileContainerX *FilesX::findFileContainer(const QString &containerName) const
{
    FileContainerX *fileContX = m_private;
    FileContainerX *cont = findFileContainer(fileContX, containerName);

    if (cont)
        return cont;

    for (int i = 0; i < m_private->childCount(); ++i) {
        cont = findFileContainerRecursive(m_private->fileContainer(i), containerName);

        if (cont)
            return cont;
    }

    return 0;
}

void FilesX::readFiles()
{
    readFileContainers();
    readFileGroup(QLatin1String(CL_INCLUDE));
    readFileGroup(QLatin1String(CL_COMPILE));
}

void FilesX::readFileGroup(const QString &groupName)
{
    ItemGroup *filterItemGroup = Utils::findItemGroupWithName(groupName, m_filters);
    ItemGroup *projectItemGroup = Utils::findItemGroupWithName(groupName, m_project);

    if (!filterItemGroup || !projectItemGroup)
        return;

    for (int i = 0; i < filterItemGroup->itemCount(); ++i) {
        Item *filterItem = filterItemGroup->item(i);
        ItemMetaData *filterItemMeta = Utils::findItemMetaWithName(QLatin1String(FILTER_ITEM), filterItem);
        Item *projectItem = Utils::findItemInGroup(filterItem->include(), projectItemGroup);

        if (filterItemMeta && projectItem) {
            FileContainerX *cont = findFileContainer(filterItemMeta->value());

            FileX *fileX = new FileX;
            fileX->m_filterItem = filterItem;
            fileX->m_item = projectItem;
            fileX->m_parentProjectDocument = m_parentProject;
            fileX->m_filters = m_filters;

            cont->m_files.append(fileX);
        }
    }
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
