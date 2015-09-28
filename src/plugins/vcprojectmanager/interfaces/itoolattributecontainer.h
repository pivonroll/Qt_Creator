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
#ifndef VCPROJECTMANAGER_ITOOLATTRIBUTECONTAINER_H
#define VCPROJECTMANAGER_ITOOLATTRIBUTECONTAINER_H

#include <QString>

namespace VcProjectManager {
namespace Internal {

class IToolAttribute;

class IToolAttributeContainer
{
public:
    virtual ~IToolAttributeContainer() {}

    /*!
     * \return a tool attribute at an \a index.
     */
    virtual IToolAttribute *toolAttribute(int index) const = 0;

    /*!
     * \return a tool attribute with attribute key \a attributeKey.
     */
    virtual IToolAttribute *toolAttribute(const QString &attributeKey) const = 0;

    /*!
     * \return a count of tool attributes.
     */
    virtual int toolAttributeCount() const = 0;

    /*!
     * \brief Adds a tool attribute to the list of tool attributes.
     */
    virtual void addToolAttribute(IToolAttribute* toolAttribute) = 0;

    /*!
     * \brief Removes a tool attribute from the list of tool attributes.
     */
    virtual void removeToolAttribute(IToolAttribute* toolAttribute) = 0;
};

} // Internal
} // VcProjectManager
#endif // ITOOLATTRIBUTECONTAINER_H
