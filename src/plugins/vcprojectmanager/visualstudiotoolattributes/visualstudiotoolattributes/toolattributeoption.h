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
#ifndef VCPROJECTMANAGER_INTERNAL_TOOLOPTIONS_H
#define VCPROJECTMANAGER_INTERNAL_TOOLOPTIONS_H

#include <QString>

namespace VcProjectManager {
namespace Internal {

/*!
 * \brief The ToolAttributeOption class represents
 * one option that can be assigned to a tool attribute.
 * Options are linked, so see \a nextOption() for option
 * that is next in the list of viable options that can be
 * assigned to a tool attribute.
 */
class ToolAttributeOption
{
public:
    ToolAttributeOption();
    ToolAttributeOption(const QString &description, const QString &value);
    ~ToolAttributeOption();

    /*!
     * \return a description for this tool attribute option.
     */
    QString description() const;

    /*!
     * \brief Sets a description for this tool attribute option.
     */
    void setDescription(const QString &description);

    /*!
     * \return a value stored in this option.
     * This value can be used and assigned to a tool attribute.
     */
    QString value() const;

    /*!
     * \brief Sets a value in this option.
     * This value can be used and assigned to a tool attribute.
     */
    void setValue(const QString &value);

    /*!
     * \return a 0 (NULL) if there is no next ToolAttributeOption.
     * Otherwise it returns a pointer to a next ToolAttributeOption in the list.
     */
    ToolAttributeOption *nextOption() const;

    /*!
     * \brief Appends an option as a sequel to this option.
     * If there was another option as a sequel to this one it will be
     * moved to be a sequel of \a option.
     */
    void appendOption(ToolAttributeOption *option);

private:
    QString m_description;
    QString m_value;
    ToolAttributeOption *m_nextOption;
    bool m_isNull;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_TOOLOPTIONS_H
