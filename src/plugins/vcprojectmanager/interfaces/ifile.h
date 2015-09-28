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
#ifndef VCPROJECTMANAGER_INTERNAL_IFILE_H
#define VCPROJECTMANAGER_INTERNAL_IFILE_H

#include "ivcprojectnodemodel.h"

#include <projectexplorer/projectnodes.h>

namespace VcProjectManager {
namespace Internal {

class IConfiguration;
class IConfigurationContainer;
class IAttributeContainer;

class IFile : public IVcProjectXMLNode
{
public:
    /*!
     * \return a relative path to a file.
     * Path is relative to .vcroj or .vcprojx file in which this file if referenced.
     */
    virtual QString relativePath() const = 0;

    /*!
     * \brief Sets relative path for a file.
     * Path should be relative to a .vcproj of .vcprojx file where this
     * file belongs to.
     */
    virtual void setRelativePath(const QString &path) = 0;

    /*!
     * \return canonical path for this file.
     */
    virtual QString canonicalPath() const  = 0;

    /*!
     * \return a configuration container used to store build configurations
     * for this file.
     * See \class ConfigurationContainer for more detail.
     */
    virtual IConfigurationContainer *configurationContainer() const = 0;

    /*!
     * \return a attribute container used to store attributes for this file.
     * See \class IAttributeContainer for more detail.
     */
    virtual IAttributeContainer *attributeContainer() const = 0;

    /*!
     * \return a clone of this object.
     */
    virtual IFile *clone() const = 0;

    /*!
     * \return a type of the file.
     */
    virtual ProjectExplorer::FileType fileType() const = 0;

    virtual IConfiguration *createDefaultBuildConfiguration(const QString &configName, const QString &platformName) const = 0;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_IFILE_H
