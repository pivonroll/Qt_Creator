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
#ifndef VCPROJECTMANAGER_INTERNAL_FILE_CONTAINER_H
#define VCPROJECTMANAGER_INTERNAL_FILE_CONTAINER_H

#include <visualstudiointerfaces/ifilecontainer.h>

#include <QList>
#include <QString>

namespace VcProjectManager {
namespace Internal {

class IVisualStudioProject;
class GeneralAttributeContainer;

class FileContainer : public IFileContainer
{
public:
    FileContainer(const QString &containerType, IVisualStudioProject *parentProjectDoc, const QString &relativePath = QString());
    FileContainer(const FileContainer &other);
    FileContainer(FileContainer &&other);
    FileContainer &operator=(FileContainer other);
    ~FileContainer();

    QString containerType() const;

    void addFile(IFile *file);
    IFile *file(int index) const;
    IFile *file(const QString &relativePath) const;
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

    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    QString relativePath() const;
    void setRelativePath(const QString &relativePath);
    IFile *findFile(const QString &canonicalFilePath) const;
    IFileContainer *findFileContainer(const QString &relativePath) const;

private:
    FileContainer();
    static void swap(FileContainer &first, FileContainer &second);
    void processFile(const QDomNode &fileNode);
    void processFilter(const QDomNode &filterNode);
    void processFolder(const QDomNode &folderNode);
    void processNodeAttributes(const QDomElement &element);

    QList<IFile *> m_files;
    QList<IFileContainer *> m_fileContainers;

    QString m_name; // required
    IVisualStudioProject *m_parentProjectDoc;
    GeneralAttributeContainer *m_attributeContainer;
    QString m_containerType;
    QString m_relativePath;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_FILE_CONTAINER_H
