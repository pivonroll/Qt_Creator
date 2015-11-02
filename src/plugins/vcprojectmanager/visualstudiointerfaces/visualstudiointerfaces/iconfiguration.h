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
#ifndef VCPROJECTMANAGER_INTERNAL_ICONFIGURATION_H
#define VCPROJECTMANAGER_INTERNAL_ICONFIGURATION_H

#include "ivcprojectnodemodel.h"

#include <QObject>

namespace VcProjectManager {
namespace Internal {

class ITools;
class IAttributeContainer;

class IConfiguration : public QObject, public IVcProjectXMLNode
{
    Q_OBJECT

public:
    enum ConfigurationVersion
    {
        ConfigurationVersion_pre2010,
        ConfigurationVersion_post2010
    };

    IConfiguration(QObject *parent = 0) : QObject(parent) {}
    IConfiguration(const IConfiguration &) : QObject(0) {}

    virtual ConfigurationVersion version() const = 0;

    /*!
     * \return a container used to store attributes for this configuration.
     */
    virtual IAttributeContainer *attributeContainer() const = 0;

    /*!
     * \return a full name of a configuration.
     * Example Debug|Win32.
     */
    virtual QString fullName() const = 0;

    /*!
     * \return a display name for a configuration.
     */
    virtual QString displayname() const = 0;

    /*!
     * \return a platform set for this configuration.
     */
    virtual QString platform() const = 0;

    /*!
     * \brief Sets a full name for this configuration.
     * Name must be in a format: <Build Name>|<Platform>.
     * Example: Debug|Win32
     */
    virtual void setFullName(const QString &fullName) = 0;

    /*!
     * \brief Sets a display name for a configuration.
     */
    virtual void setDisplayName(const QString &displayname) = 0;

    /*!
     * \brief Sets a platform used in a configuration.
     */
    virtual void setPlatform(const QString &platform) = 0;

    /*!
     * \return tools assigned to this configuration.
     * For more detail see \class ITools.
     */
    virtual ITools *tools() const = 0;

    /*!
     * Implement in order to support creating a clone of a IConfiguration instance.
     * \return A pointer to newly created IConfiguration instance.
     */
    virtual IConfiguration *clone() const = 0;

signals:
    /*!
     * \brief Emited when full name of a configuration is changed.
     */
    void nameChanged();
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_ICONFIGURATION_H
