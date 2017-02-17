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
#ifndef VCPROJECTMANAGER_INTERNAL_I_VISUAL_STUDIO_PROJECT_H
#define VCPROJECTMANAGER_INTERNAL_I_VISUAL_STUDIO_PROJECT_H

#include "ivcprojectnodemodel.h"
#include "../common/projectconstants.h"

#include <QString>

namespace VcProjectManager {
namespace Internal {

class IAttributeContainer;
class IConfiguration;
class IConfigurations;
class IFiles;
class IGlobals;
class IPlatforms;
class IProjectFactories;
class IPublishingData;
class IReferences;
class ISettingsWidget;
class IToolFiles;

class IVisualStudioProject : public IVcProjectXMLNode
{
public:
    virtual ~IVisualStudioProject() {}
    /*!
     * \return a attribute container used to store attributes for this project.
     * See \class IAttributeContainer for more detail.
     */
    virtual IAttributeContainer *attributeContainer() const = 0;

    /*!
     * \return a configurations object which stores build configurations used in this project.
     */
    virtual IConfigurations *configurations() const = 0;

    /*!
     * \return a files object used to store files and file containers in this project.
     */
    virtual IFiles *files() const = 0;

    /*!
     * \return a globals object used to store globals defined in this project.
     */
    virtual IGlobals *globals() const = 0;

    /*!
     * \return a platforms object used to stre available platforms used in this project.
     */
    virtual IPlatforms *platforms() const = 0;

    /*!
     * \return a references object used to store references used in this project.
     */
    virtual IReferences *referencess() const = 0;

    /*!
     * \return a tool files object used to store tool files in this project.
     */
    virtual IToolFiles *toolFiles() const = 0;

    /*!
     * \return a publishing data object used to store publishing items in this project.
     */
    virtual IPublishingData *publishingData() const = 0;

    /*!
     * \return a path to this project file.
     * Path is a cannonical path ot the .vcproj or .vcprojx file this object represences.
     */
    virtual QString filePath() const = 0;

    /*!
     * \brief Call this when you want ot save data to a file.
     * \param filePath is a canonical path to a file to which we want to save data to.
     * \return \b true if operation succeded, otherwise it returns \b false.
     */
    virtual bool saveToFile(const QString &filePath) const = 0;

    /*!
     * \return a version of Visual Studio project which is represented with this object.
     */
    virtual DocumentVersion documentVersion() const = 0;

    /*!
     * \brief Creates a default build configuration for this project and sets appropriate tools and attributes for it.
     * \param fullConfigName is a full name of the new configuration. Pattern for the name is <Build name>|<Platform>
     * \return a newly created configuration object;
     * Note: It is up to a user to destroy the object.
     */
    virtual IConfiguration *createDefaultBuildConfiguration(const QString &fullConfigName) const = 0;

    virtual IProjectFactories *projectFactories() const = 0;

    virtual IVisualStudioProject *clone() const = 0;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_I_VISUAL_STUDIO_PROJECT_H
