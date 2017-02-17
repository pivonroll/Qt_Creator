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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILE_X_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILE_X_H

#include <visualstudiointerfaces/ifile.h>

namespace VcProjectManager {
namespace Internal {

class VcProjectDocumentX;

namespace VisualStudioProjectX {

class Item;
class Project;

class FileX : public IFile
{
    friend class FileContainerX;
    friend class FilesX;
    friend class FileFactoryX;

public:
    FileX();
    FileX(const FileX &file);
    FileX(FileX &&file);
    FileX& operator=(FileX file);
    ~FileX();

    // IFile interface
    QString displayName() const;
    QString relativePath() const;
    void setRelativePath(const QString &relativePath);
    QString canonicalPath() const;
    IConfigurationContainer *configurationContainer() const;
    IAttributeContainer *attributeContainer() const;
    IFile *clone() const;
    ProjectExplorer::FileType fileType() const;
    IConfiguration *createDefaultBuildConfiguration(const QString &configName, const QString &platformName) const;

    // IVcProjectXMLNode interface
    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    static FileX *createNewFile(const QString &relativePath, const QString &filterPath);

private:
    static void swap(FileX &first, FileX &second);

    Item *m_projectItem;
    Item *m_filterItem;
    Project *m_project;
    Project *m_filters;

    VcProjectDocumentX *m_parentProjectDocument;
    QString m_relativeFilePath;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_FILE_X_H
