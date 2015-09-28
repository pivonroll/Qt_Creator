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
#ifndef VCPROJECTMANAGER_INTERNAL_ITOOLATTRIBUTEDESCRIPTIONDATAITEM_H
#define VCPROJECTMANAGER_INTERNAL_ITOOLATTRIBUTEDESCRIPTIONDATAITEM_H

#include <QString>

namespace VcProjectManager {
namespace Internal {

/*!
 * \interface IAttributeDescriptionDataItem is used to store data from tool description XML files.
 */

class ToolAttributeOption;
class IToolAttribute;

class IAttributeDescriptionDataItem
{
public:
    virtual ~IAttributeDescriptionDataItem() {}

    /*!
     * \return a description text for an attribute.
     */
    virtual QString descriptionText() const = 0;

    /*!
     * \return a display text for an attribute.
     */
    virtual QString displayText() const = 0;

    /*!
     * \return a key for an attribute.
     */
    virtual QString key() const = 0;

    /*!
     * \brief An attribute can have more than one allowed value to be set.
     * Not all atributes have this.
     * \return a first option from which user can choose from.
     */
    virtual ToolAttributeOption *firstOption() const = 0;

    /*!
     * \brief Sets the first possible option value from which user can choose attribute's value.
     * Not all attributes can have it.
     */
    virtual void setFirstOption(ToolAttributeOption *opt) = 0;

    /*!
     * \return a default value for an attribute.
     */
    virtual QString defaultValue() const = 0;

    /*!
     * \brief Creates a new attribute and returns a pointer to it.
     * It is user responsibility to destroy the attribute object.
     * \return newly created attribute.
     */
    virtual IToolAttribute *createAttribute() const = 0;

    /*!
     * \brief Some attribute descriptions can have some other values assigned to them,
     * like the character used as a separator in a string etc.
     * \return a value assigned to a \a key.
     */
    virtual QString optionalValue(const QString &key) const = 0;

    /*!
     * \brief Some attribute descriptions can have some other values assigned to them,
     * like the character used as a separator in a string etc.
     * Sets an additional value to an attribute's description field.
     * The additional value can be referenced by \a key.
     */
    virtual void setOptionalValue(const QString &key, const QString &value) = 0;

    /*!
     * \brief Some attribute descriptions can have some other values assigned to them,
     * like the character used as a separator in a string etc.
     * Removes an optional value from an attribute description.
     * Optional value is referenced by \a key.
     */
    virtual void removeOptionalValue(const QString &key) = 0;

    /*!
     * \return a type of an attribute this class describes.
     * Usually it is Integer, String, StringList and Boolean.
     */
    virtual QString type() const = 0;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_ITOOLATTRIBUTEDESCRIPTIONDATAITEM_H
