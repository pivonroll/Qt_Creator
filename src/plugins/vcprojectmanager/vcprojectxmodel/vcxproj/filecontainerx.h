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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILECONTAINERX_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILECONTAINERX_H

#include "../../interfaces/ifilecontainer.h"

#include <QList>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class Item;
class ItemGroup;
class Project;

class FileContainerX : public IFileContainer
{
    friend class FilesX;
public:
    FileContainerX(const FileContainerX &fileCont);
    FileContainerX &operator=(const FileContainerX &fileCont);
    ~FileContainerX();

    // IFileContainer interface
    QString containerType() const;
    void addFile(IFile *file);
    IFile *file(int index) const;
    int fileCount() const;
    void removeFile(IFile *file);
    void addFileContainer(IFileContainer *fileContainer);
    int childCount() const;
    IFileContainer *fileContainer(int index) const;
    void removeFileContainer(IFileContainer *fileContainer);
    IAttributeContainer *attributeContainer() const;
    QString displayName() const;
    void setDisplayName(const QString &displayName);
    void allFiles(QStringList &sl) const;
    bool fileExists(const QString &relativeFilePath) const;
    IFileContainer *clone() const;

    QString relativePath() const;
    void setRelativePath(const QString &relativePath);

    IFile *createNewFile(const QString &relativePath, ProjectExplorer::FileType fileType);
    IFileContainer *createNewFileContainer(const QString &containerType, const QString &displayName);

    // IVcProjectXMLNode interface
    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

private:
    FileContainerX();

    ItemGroup *findProperItemGroup(const QString &itemName, Project *docProject) const;
    ItemGroup *findItemGroup(Item *item, Project *docProject) const;
    bool groupContainsItem(Item *item, ItemGroup *itemGroup) const;
    bool itemGroupContainsItemName(const QString &itemName, ItemGroup *itemGroup)const;

    void removeAllFilesInAFilter(const QString &fullFilterName, QStringList &relativePaths);
    void removeAllFilesInAFilter(ItemGroup *itemGroup, const QString &fullFilterName, QStringList &relativePaths);
    bool isFileInAFilter(Item *item, const QString &fullFilterName);

    void removeAllFiltersInAFilter(const QString &fullFilterName);
    void removeFilesFromProject(const QStringList &relativePaths);
    void removeFileFromItemGroup(ItemGroup *itemGroup, const QStringList &relativePaths);

    Item *m_filterItem;
    Project *m_project;
    Project *m_filters;
    FileContainerX *m_parentContainer;
    QList<IFile *> m_files;
    QList<IFileContainer *> m_fileContainers;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILECONTAINERX_H
