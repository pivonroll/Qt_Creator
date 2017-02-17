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
#ifndef VCPROJECTMANAGER_INTERNAL_FILES_H
#define VCPROJECTMANAGER_INTERNAL_FILES_H

#include "file.h"
#include "filecontainer.h"
#include <visualstudiointerfaces/ifiles.h>
#include <visualstudiointerfaces/ivcprojectnodemodel.h>

#include <QList>

namespace VcProjectManager {
namespace Internal {

class IVisualStudioProject;

class Files : public IFiles
{
public:
    explicit Files(IVisualStudioProject *parentProject = nullptr);
    Files(const Files &other);
    Files(Files &&other);
    Files &operator =(Files other);
    ~Files();

    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    void addFile(IFile *file);
    int fileCount() const;
    IFile *file(int index) const;
    IFile *file(const QString &relativePath) const;
    void removeFile(IFile *file);
    bool fileExists(const QString &relativeFilePath) const;

    void addFileContainer(IFileContainer *fileContainer);
    int fileContainerCount() const;
    IFileContainer *fileContainer(int index) const;
    void removeFileContainer(IFileContainer *fileContainer);
    IFile *findFile(const QString &canonicalFilePath) const;
    IFileContainer *findFileContainer(const QString &relativePath) const;

protected:
    static void swap(Files &first, Files &second);
    void processFile(const QDomNode &fileNode);
    void processFilter(const QDomNode &filterNode);
    void processFolder(const QDomNode &folderNode);

    QList<IFileContainer *> m_fileContainers;
    QList<IFile *> m_files;
    IVisualStudioProject *m_parentProject;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_FILES_H
