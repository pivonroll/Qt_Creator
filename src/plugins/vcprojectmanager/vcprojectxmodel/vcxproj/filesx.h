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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILES_X_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILES_X_H

#include <visualstudiointerfaces/ifiles.h>

#include <projectexplorer/projectnodes.h>

namespace VcProjectManager {
namespace Internal {

class VcProjectDocumentX;

namespace VisualStudioProjectX {

class Project;
class ItemGroup;
class Item;
class FileContainerX;

class FilesX : public IFiles
{
public:
    FilesX(Project *project, Project *filters, VcProjectDocumentX *parentProject);
    FilesX(const FilesX &other);
    FilesX(FilesX &&other);
    FilesX& operator=(FilesX other);
    ~FilesX();

    // IFiles interface
    void addFile(IFile *file);
    void removeFile(IFile *file);
    int fileCount() const;
    IFile *file(int index) const;
    IFile *file(const QString &relativePath) const;
    bool fileExists(const QString &relativeFilePath) const;
    void addFileContainer(IFileContainer *fileContainer);
    int fileContainerCount() const;
    IFileContainer *fileContainer(int index) const;
    void removeFileContainer(IFileContainer *fileContainer);

    IFile *findFile(const QString &canonicalFilePath) const;
    IFileContainer *findFileContainer(const QString &relativePath) const;

    // IVcProjectXMLNode interface
    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

private:
    FilesX();
    static void swap(FilesX &first, FilesX &second);
    void readFileContainers();
    void readFileContainers(ItemGroup *itemGroup, Item *item);

    void readFiles();
    void readFileGroup(const QString &groupName);

    Project *m_project;
    Project *m_filters;
    FileContainerX *m_private;
    VcProjectDocumentX *m_parentProject;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILES_X_H
