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
#ifndef VCPROJECTMANAGER_INTERNAL_CONFIGURATION_CONTAINER_H
#define VCPROJECTMANAGER_INTERNAL_CONFIGURATION_CONTAINER_H

#include <visualstudiointerfaces/iconfigurationcontainer.h>

#include <QList>
#include <QObject>

QT_BEGIN_NAMESPACE
class QDomDocument;
class QDomElement;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class IConfiguration;

/*!
 * \brief The ConfigurationContainer class
 */

class ConfigurationContainer : public IConfigurationContainer
{
    Q_OBJECT

public:
    explicit ConfigurationContainer(QObject *parent = nullptr);
    ConfigurationContainer(const ConfigurationContainer &other);
    ConfigurationContainer(ConfigurationContainer &&other);
    ConfigurationContainer &operator=(ConfigurationContainer other);
    ~ConfigurationContainer();

    /*!
     * \brief Adds a configuration to the list of build configurations.
     */
    void addConfiguration(IConfiguration *config);

    /*!
     * \return a configuration with full name \a fullName.
     */
    IConfiguration *configuration(const QString &fullName) const;

    /*!
     * \return a configuration at \a index.
     */
    IConfiguration *configuration(int index) const;

    /*!
     * \return a count of configuration stored in this container.
     */
    int configurationCount() const;

    /*!
     * \brief Removes a configuration with full name \a fullName.
     */
    void removeConfiguration(const QString &fullName);

    /*!
     * \brief Appends configurations to and XML node \a domElement.
     * Use \a domXmlDocument to create new nodes.
     */
    void appendToXMLNode(QDomElement &domElement, QDomDocument &domXMLDocument);

    IConfiguration *createNewConfiguration(const QString &configDisplayName, const QString &platformName);

    IConfigurationContainer *clone() const;
    void copyDataFrom(IConfigurationContainer *config);

private:
    static void swap(ConfigurationContainer &first, ConfigurationContainer &second);
    QList<IConfiguration *> m_configs;
};

} // Internal
} // VcProjectManager
#endif // VCPROJECTMANAGER_INTERNAL_CONFIGURATION_CONTAINER_H
