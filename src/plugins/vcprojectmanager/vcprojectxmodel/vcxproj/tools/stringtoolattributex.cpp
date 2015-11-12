/**************************************************************************
**
** Copyright (c) 2015 Bojan Petrovic
** Copyright (c) 2015 Radovan Zivkovic
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
#include "stringtoolattributex.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include "../configurationx.h"
#include "../private/project.h"
#include "../private/propertygroup.h"
#include "../private/property.h"

#include "../utilsx.h"
#include "../vcprojx_constants.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

StringToolAttributeX::StringToolAttributeX(const AttributeDescriptionDataItem *attributeDescription, Project *project, ConfigurationX *configuration)
    : m_attributeDescription(attributeDescription)
    , m_isUsed(false)
    , m_project(project)
    , m_configuration(configuration)
{
}

StringToolAttributeX::StringToolAttributeX(const StringToolAttributeX &other)
{
    m_attributeDescription = other.m_attributeDescription;
    m_isUsed = other.m_isUsed;
    m_project = other.m_project;
    m_configuration = other.m_configuration;
}

const AttributeDescriptionDataItem *StringToolAttributeX::descriptionDataItem() const
{
    return m_attributeDescription;
}

IToolAttributeSettingsWidget *StringToolAttributeX::createSettingsItem()
{
    return nullptr;
}

QString StringToolAttributeX::value() const
{
    PropertyGroup *propertyGroup = Utils::findPropertyGroup(m_project, m_configuration->fullName(), QLatin1String(CONFIGURATION));

    if (!propertyGroup)
        return m_attributeDescription->defaultValue();

    Property *property = Utils::findProperty(propertyGroup, m_attributeDescription->key());

    if (!property)
        return m_attributeDescription->defaultValue();

    return property->value();
}

void StringToolAttributeX::setValue(const QString &value)
{
    PropertyGroup *propertyGroup = Utils::findPropertyGroup(m_project, m_configuration->fullName(), QLatin1String(CONFIGURATION));

    if (!propertyGroup) {
        propertyGroup = new PropertyGroup;
        QString condition = QLatin1String(CONFIGURATION_VARIABLE)
                + QLatin1String(CONFIGURATION_PLATFORM_DELIMITER)
                + QLatin1String(PLATFORM_VARIABLE)
                + QLatin1String("==")
                + m_configuration->fullName();
        propertyGroup->setCondition(condition);
        propertyGroup->setLabel(QLatin1String(CONFIGURATION));
        m_project->addPropertyGroup(propertyGroup);
    }

    Property *property = Utils::findProperty(propertyGroup, m_attributeDescription->key());

    if (!property) {
        property = new Property;
        property->setName(m_attributeDescription->key());
        propertyGroup->addProperty(property);
    }

    property->setValue(value);
}

bool StringToolAttributeX::isUsed() const
{
    return m_isUsed;
}

IToolAttribute *StringToolAttributeX::clone() const
{
    return new StringToolAttributeX(*this);
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

