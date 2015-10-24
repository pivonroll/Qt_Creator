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
#ifndef VCPROJECTMANAGER_INTERNAL_ICONFIGURATIONCONTAINER_H
#define VCPROJECTMANAGER_INTERNAL_ICONFIGURATIONCONTAINER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class IConfiguration;

class IConfigurationContainer : public QObject
{
    Q_OBJECT

public:
    explicit IConfigurationContainer(QObject *parent = 0);
    IConfigurationContainer(const IConfigurationContainer &);
    virtual IConfigurationContainer &operator=(const IConfigurationContainer &configurationContainer);
    virtual ~IConfigurationContainer();

    virtual void addConfiguration(IConfiguration *config) = 0;
    virtual void removeConfiguration(const QString &fullName) = 0;

    virtual IConfiguration *configuration(const QString &fullName) const = 0;
    virtual IConfiguration *configuration(int index) const = 0;
    virtual int configurationCount() const = 0;

    virtual IConfiguration *createNewConfiguration(const QString &configDisplayName, const QString &platformName) = 0;
    virtual IConfigurationContainer *clone() const = 0;

signals:
    /*!
     * \brief Emitted when new configuration is added.
     */
    void configurationAdded(IConfiguration *config);

    /*!
     * \brief Emitted when configuration is removed.
     */
    void configurationRemoved(QString fullConfigName);
};

} // Internal
} // VcProjectManager

#endif // ICONFIGURATIONCONTAINER_H
