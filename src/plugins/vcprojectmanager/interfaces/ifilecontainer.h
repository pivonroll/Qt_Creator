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
#ifndef VCPROJECTMANAGER_INTERNAL_IFILECONTAINER_H
#define VCPROJECTMANAGER_INTERNAL_IFILECONTAINER_H

#include "ivcprojectnodemodel.h"
#include <projectexplorer/projectnodes.h>

QT_FORWARD_DECLARE_CLASS(QStringList)

namespace VcProjectManager {
namespace Internal {

class IFile;
class IAttributeContainer;

class IFileContainer : public IVcProjectXMLNode
{
public:
    /*!
     * \return a type of a container. For example Folder or Filter.
     */
    virtual QString containerType() const = 0;

    /*!
     * \brief Adds a file to a container.
     */
    virtual void addFile(IFile *file) = 0;

    /*!
     * \return a file at \a index.
     */
    virtual IFile *file(int index) const = 0;

    /*!
     * \return a count of files in this container.
     */
    virtual int fileCount() const = 0;

    /*!
     * \brief Remove a file from a container.
     */
    virtual void removeFile(IFile *file) = 0;

    /*!
     * \brief Adds a file container \a fileContainer to this container.
     */
    virtual void addFileContainer(IFileContainer *fileContainer) = 0;

    /*!
     * \return a count of file containers stored in this container.
     */
    virtual int childCount() const = 0;

    /*!
     * \return a file container at \a index.
     */
    virtual IFileContainer *fileContainer(int index) const = 0;

    /*!
     * \brief Remove a file container from this container.
     */
    virtual void removeFileContainer(IFileContainer *fileContainer) = 0;

    /*!
     * \return an attribute container used to store attributes for this container.
     */
    virtual IAttributeContainer *attributeContainer() const = 0;

    /*!
     * \return a display name for a container.
     */
    virtual QString displayName() const = 0;

    /*!
     * \brief Sets display name for a container.
     */
    virtual void setDisplayName(const QString &displayName) = 0;

    /*!
     * \brief Used to fetch all files in the container tree starting
     * from this container and including this container.
     */
    virtual void allFiles(QStringList &sl) const = 0;

    /*!
     * \return \b true if a file exists in container tree starting from this container,
     * including this container.
     */
    virtual bool fileExists(const QString &relativeFilePath) const = 0;

    /*!
     * \return a clone of this object.
     */
    virtual IFileContainer *clone() const = 0;

    virtual QString relativePath() const = 0;
    virtual void setRelativePath(const QString &relativePath) = 0;

    virtual IFile *createNewFile(const QString &relativePath, ProjectExplorer::FileType fileType) = 0;
    virtual IFileContainer *createNewFileContainer(const QString &containerType, const QString &displayName) = 0;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_IFILECONTAINER_H
