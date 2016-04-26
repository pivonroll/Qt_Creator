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
#include "configurationcontainer.h"
#include <visualstudiointerfaces/iconfiguration.h>

#include <QDomNode>

namespace VcProjectManager {
namespace Internal {

ConfigurationContainer::ConfigurationContainer(QObject *parent)
    : IConfigurationContainer(parent)
{
}

ConfigurationContainer::ConfigurationContainer(const ConfigurationContainer &other)
    : IConfigurationContainer(other)
{
    foreach (IConfiguration *config, other.m_configs)
        m_configs << config->clone();
}

ConfigurationContainer::ConfigurationContainer(ConfigurationContainer &&other)
    : ConfigurationContainer()
{
    swap(*this, other);
}

ConfigurationContainer &ConfigurationContainer::operator=(ConfigurationContainer other)
{
    swap(*this, other);
    return *this;
}

ConfigurationContainer::~ConfigurationContainer()
{
    qDeleteAll(m_configs);
}

void ConfigurationContainer::addConfiguration(IConfiguration *config)
{
    if (m_configs.contains(config))
        return;

    // if there is already a configuration with the same name
    foreach (const IConfiguration *conf, m_configs) {
        if (config->fullName() == conf->fullName())
            return;
    }

    m_configs.append(config);
    emit configurationAdded(config);
}

IConfiguration *ConfigurationContainer::configuration(const QString &fullName) const
{
    foreach (IConfiguration *config, m_configs) {
        if (config->fullName() == fullName)
            return config;
    }
    return 0;
}

IConfiguration *ConfigurationContainer::configuration(int index) const
{
    if (0 <= index && index < m_configs.size())
        return m_configs[index];
    return 0;
}

int ConfigurationContainer::configurationCount() const
{
    return m_configs.size();
}

void ConfigurationContainer::removeConfiguration(const QString &fullName)
{
    // if there is already a configuration with the same name
    foreach (IConfiguration *conf, m_configs) {
        if (conf->fullName() == fullName) {
            m_configs.removeOne(conf);
            emit configurationRemoved(fullName);
            delete conf;
            return;
        }
    }
}

void ConfigurationContainer::appendToXMLNode(QDomElement &domElement, QDomDocument &domXMLDocument)
{
    foreach (const IConfiguration *config, m_configs)
        domElement.appendChild(config->toXMLDomNode(domXMLDocument));
}

IConfiguration *ConfigurationContainer::createNewConfiguration(const QString &configDisplayName, const QString &platformName)
{
    Q_UNUSED(configDisplayName)
    Q_UNUSED(platformName)
    return 0;
}

IConfigurationContainer *ConfigurationContainer::clone() const
{
    return new ConfigurationContainer(*this);
}

void ConfigurationContainer::copyDataFrom(IConfigurationContainer *config)
{
    ConfigurationContainer *container = dynamic_cast<ConfigurationContainer *>(config);
    if (container)
        *this = *container;
}

void ConfigurationContainer::swap(ConfigurationContainer &first, ConfigurationContainer &second)
{
    std::swap(first.m_configs, second.m_configs);
}

} // Internal
} // VcProjectManager
