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
#ifndef VCPROJECTMANAGER_INTERNAL_IPUBLISHINGDATA_H
#define VCPROJECTMANAGER_INTERNAL_IPUBLISHINGDATA_H

#include "ivcprojectnodemodel.h"

namespace VcProjectManager {
namespace Internal {

class IPublishingItem;
class IAttributeContainer;

class IPublishingData : public IVcProjectXMLNode
{
public :
    /*!
     * \brief Adds a publishing item to the list of publishing data.
     */
    virtual void addPublishingItem(IPublishingItem *item) = 0;

    /*!
     * \brief Removes a publishing item from the list of publishing data.
     */
    virtual void removePublishingItem(IPublishingItem *item) = 0;

    /*!
     * \return a count of publishing items stored.
     */
    virtual int publishingItemCount() const = 0;

    /*!
     * \return publishing item at \a index.
     */
    virtual IPublishingItem *publishingItem(int index) const = 0;

    /*!
     * \return an attribute container used to store attributes
     * for this publishing data. See \class IAttributeContainer for more detail.
     */
    virtual IAttributeContainer *attributeContainer() const = 0;
};

} // Internal
} // VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_IPUBLISHINGDATA_H
