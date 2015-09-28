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
#ifndef VCPROJECTMANAGER_INTERNAL_IATTRIBUTEHANDLER_H
#define VCPROJECTMANAGER_INTERNAL_IATTRIBUTEHANDLER_H

#include <QString>

QT_BEGIN_NAMESPACE
class QDomElement;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class IAttributeContainer
{
public:
    virtual ~IAttributeContainer() {}

    /*!
     * \return a value stored in an attribute referenced by attribute's name.
     */
    virtual QString attributeValue(const QString &attributeName) const = 0;

    /*!
     * \return a name of an attribute stored at \a index.
     */
    virtual QString getAttributeName(int index) const = 0;

    /*!
     * \brief Clears the value stored in an attribute \a attributeName)
     */
    virtual void clearAttribute(const QString &attributeName) = 0;

    /*!
     * \brief Removes an attribute from a container. Attribute is referenced by attribute's name.
     */
    virtual void removeAttribute(const QString &attributeName) = 0;

    /*!
     * \brief Set's a value for an attribute.
     */
    virtual void setAttribute(const QString &attributeName, const QString &attributeValue) = 0;

    /*!
     * \return the number of attributes stored in a container.
     */
    virtual int getAttributeCount() const = 0;

    /*!
     * \brief Implement this in order to append attributes stored in a container to an XML node.
     */
    virtual void appendToXMLNode(QDomElement &elementNode) const = 0;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_IATTRIBUTEHANDLER_H
