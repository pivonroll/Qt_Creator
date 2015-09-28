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
#ifndef VCPROJECTMANAGER_INTERNAL_ITOOLSECTIONDESCRIPTION_H
#define VCPROJECTMANAGER_INTERNAL_ITOOLSECTIONDESCRIPTION_H

#include <QString>

namespace VcProjectManager {
namespace Internal {

class IToolSection;
class IAttributeDescriptionDataItem;

class IToolSectionDescription
{
public:
    virtual ~IToolSectionDescription() {}

    /*!
     * \brief Creates a tool section. User must handle the destruction of the
     * newly created object.
     * \return a newly created instance of the tool section.
     */
    virtual IToolSection* createToolSection() const = 0;

    /*!
     * \return a display name for this tool section desription.
     */
    virtual QString displayName() const = 0;

    /*!
     * \brief Sets a display name for this tool section description.
     */
    virtual void setDisplayName(const QString &displayName) = 0;

    /*!
     * \return an attribute description associated with key \a attributeKey.
     */
    virtual IAttributeDescriptionDataItem *attributeDescription(const QString &attributeKey) const = 0;

    /*!
     * \return an attribute description at an \a index.
     */
    virtual IAttributeDescriptionDataItem *attributeDescription(int index) const = 0;

    /*!
     * \return a count of attribute descriptions in this tool section description.
     */
    virtual int attributeDescriptionCount() const = 0;

    /*!
     * \brief Adds an attribute description to the list of attribute descriptions.
     */
    virtual void addAttributeDescription(IAttributeDescriptionDataItem* attributeDesc) = 0;

    /*!
     * \brief Removes an attribute description from the list of attribute descriptions.
     */
    virtual void removeAttributeDescription(IAttributeDescriptionDataItem* attributeDesc) = 0;

    /*!
     * \brief Removes an attribute description with a key \a attributeKey from the list of attribute descriptions.
     */
    virtual void removeAttributeDescription(const QString &attributeKey) = 0;
};

} // Internal
} // VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_ITOOLSECTIONDESCRIPTION_H
