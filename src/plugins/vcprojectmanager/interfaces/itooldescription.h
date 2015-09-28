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
#ifndef VCPROJECTMANAGER_ITOOLDESCRIPTION_H
#define VCPROJECTMANAGER_ITOOLDESCRIPTION_H

#include <QString>

namespace VcProjectManager {
namespace Internal {

class IConfigurationBuildTool;
class IToolSectionDescription;

class IToolDescription
{
public:
    virtual ~IToolDescription() {}

    /*!
     * \return a count of section descriptions.
     */
    virtual int sectionDescriptionCount() const = 0;

    /*!
     * \return a tool section description at an \a index.
     */
    virtual IToolSectionDescription *sectionDescription(int index) const = 0;

    /*!
     * \brief Adds a section description to the list of section descriptions.
     */
    virtual void addSectionDescription(IToolSectionDescription *sectionDescription) = 0;

    /*!
     * \brief Removes a section description from the list of section descriptions.
     */
    virtual void removeSectionDescription(IToolSectionDescription *sectionDescription) = 0;

    /*!
     * \return a tool key for this tool description.
     */
    virtual QString toolKey() const = 0;

    /*!
     * \brief Sets a tool key for this tool description.
     */
    virtual void setToolKey(const QString &toolKey) = 0;

    /*!
     * \return a tool display name.
     */
    virtual QString toolDisplayName() const = 0;

    /*!
     * \brief Sets a tool display name used in this description.
     */
    virtual void setToolDisplayName(const QString &toolDisplayName) = 0;

    /*!
     * \brief Creates a new tool object based on the data in tool description.
     * \return a newly created tool object.
     */
    virtual IConfigurationBuildTool *createTool() const = 0;
};

} // Internal
} // VcProjectManager

#endif // VCPROJECTMANAGER_ITOOLDESCRIPTION_H
