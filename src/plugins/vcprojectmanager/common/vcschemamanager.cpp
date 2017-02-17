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
#include "vcprojectmanagerconstants.h"
#include "vcschemamanager.h"

#include <vcprojectmodel/tools/tool_constants.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <coreplugin/icore.h>

#include <QSettings>
#include <QStringList>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class VcSchemaManager is used to store data
 * about various XML schema files which are used to
 * validate Visual Studio projects and XML representations
 * of Visual Studio configuration's tool items.
 * This is a \b singleton.
 */

/*!
 * \return an instance of a \class VcSchemaManager.
 */
VcSchemaManager *VcSchemaManager::instance()
{
    static VcSchemaManager instance;
    return &instance;
}

VcSchemaManager::~VcSchemaManager()
{
}

/*!
 * \brief adds an XML schema file \a schemaPath used to validate
 * given \a version of a Visual Studio project.
 */
void VcSchemaManager::addDocumentSchema(const QString &schemaPath, Constants::SchemaVersion version)
{
    m_documentSchemas.insert(version, schemaPath);
}

/*!
 * \return a XML schema file path used to validate
 * given \a version of a Visual Studio projects.
 */
QString VcSchemaManager::documentSchema(Constants::SchemaVersion version)
{
    return m_documentSchemas.value(version);
}

/*!
 * \brief sets a path to an XML schema refered to as \a schemaPath
 * used to validate given \a version of a Visual Studio projects.
 * If XML schema for given \a version already exists it wil be
 * replaced by \a schemaPath.
 */
void VcSchemaManager::setDocumentSchema(Constants::SchemaVersion version, const QString &schemaPath)
{
    m_documentSchemas.insert(version, schemaPath);
}

/*!
 * \brief Removes path to an XML schema file used to
 * validate a given \a version of Visual Studio project.
 *
 */
void VcSchemaManager::removeDocumentSchema(Constants::SchemaVersion version)
{
    m_documentSchemas.remove(version);
}

/*!
 * \return path to an XML schema file used to validate tool XML files.
 * Tool XML files describe the tool properties of a Visual Studio build configuration.
 */
QString VcSchemaManager::toolSchema() const
{
    return m_toolSchema;
}

/*!
 * \brief Sets the path to an XML schema file used to validate XML files
 * that describe the tool properties of a Visual Studio build configuration.
 */
void VcSchemaManager::setToolSchema(const QString &schemaPath)
{
    m_toolSchema = schemaPath;
}

/*!
 * \return a list of paths to XML files that describe tool properties
 * of a Visual Studio build configuration.
 */
QList<QString> VcSchemaManager::toolXMLFilePaths() const
{
    return m_toolXMLPaths.values();
}

/*!
 * \brief Adds a path of a XML file, refered to as \a toolFilePath, that contains
 * description of a tool properties for a Visual Studio build configuration.
 * Tool for which we add the XML file is refered to with \a toolKey.
 */
void VcSchemaManager::addToolXML(const QString &toolKey, const QString &toolFilePath)
{
    if (m_toolXMLPaths.contains(toolKey))
        return;

    m_toolXMLPaths[toolKey] = toolFilePath;
}

/*!
 * \brief Removes a path to an XML file used to describe tool properties for a tool
 * refered to by a \a toolKey.
 */
void VcSchemaManager::removeToolXML(const QString &toolKey)
{
    m_toolXMLPaths.remove(toolKey);
}

/*!
 * \brief Removes all XML descriptions for all tools.
 */
void VcSchemaManager::removeToolSchemas()
{
    m_toolXMLPaths.clear();
}

/*!
 * \brief Removes all XML schema files, for tool XMl validation
 * and for Visual Studio project validation.
 */
void VcSchemaManager::removeAllSchemas()
{
    m_documentSchemas.clear();
    m_toolSchema.clear();
}

/*!
 * \brief Saves the paths to XML schemas and XML files used to describe tool properties.
 */
void VcSchemaManager::saveSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(VcProjectManager::Constants::VC_PROJECT_SCHEMA_PATH));

    settings->setValue(QLatin1String(Constants::VC_PROJECT_SCHEMA_2003_QUIALIFIER),
                       m_documentSchemas.value(Constants::SV_2003));
    settings->setValue(QLatin1String(Constants::VC_PROJECT_SCHEMA_2005_QUIALIFIER),
                       m_documentSchemas.value(Constants::SV_2005));
    settings->setValue(QLatin1String(Constants::VC_PROJECT_SCHEMA_2008_QUIALIFIER),
                       m_documentSchemas.value(Constants::SV_2008));

    settings->endGroup();

    settings->setValue(QLatin1String(Constants::VC_PROJECT_TOOL_SCHEMA), m_toolSchema);

    settings->beginGroup(QLatin1String(Constants::VC_PROJECT_TOOL_XML));

    QHashIterator<QString, QString> it(m_toolXMLPaths);

    while (it.hasNext()) {
        it.next();
        settings->setValue(it.key(), it.value());
    }

    settings->endGroup();
}

/*!
 * \brief Constructor.
 */
VcSchemaManager::VcSchemaManager()
{
    loadSettings();
}

/*!
 * \brief Loads stored paths for XML schema files and XML files
 * that describe tool properties.
 */
void VcSchemaManager::loadSettings()
{
    m_documentSchemas.clear();

    QSettings *settings = Core::ICore::settings();

    settings->beginGroup(QLatin1String(VcProjectManager::Constants::VC_PROJECT_SCHEMA_PATH));
    m_documentSchemas.insert(Constants::SV_2003, settings->value(QLatin1String(Constants::VC_PROJECT_SCHEMA_2003_QUIALIFIER)).toString());
    m_documentSchemas.insert(Constants::SV_2005, settings->value(QLatin1String(Constants::VC_PROJECT_SCHEMA_2005_QUIALIFIER)).toString());
    m_documentSchemas.insert(Constants::SV_2008, settings->value(QLatin1String(Constants::VC_PROJECT_SCHEMA_2008_QUIALIFIER)).toString());
    settings->endGroup();

    m_toolSchema = settings->value(QLatin1String(Constants::VC_PROJECT_TOOL_SCHEMA)).toString();

    settings->beginGroup(QLatin1String(Constants::VC_PROJECT_TOOL_XML));
    QStringList childKeys = settings->childKeys();

    foreach (const QString &key, childKeys)
        m_toolXMLPaths[key] = settings->value(key).toString();

    settings->endGroup();
}

/*!
 * \return \b true if there's already a XML file that describes properties for a tool
 * just as the given XML file refered to with \a filePath. Otherwise it returnes \b false.
 */
bool VcSchemaManager::similarToolXMLExistsFor(const QString &filePath)
{
    ToolInfo fileToolInfo = ToolDescriptionDataManager::readToolInfo(filePath);

    foreach (const QString &toolXMLPath, m_toolXMLPaths) {
        ToolInfo info = ToolDescriptionDataManager::readToolInfo(toolXMLPath);
        if (info == fileToolInfo)
            return true;
    }

    return false;
}

} // namespace Internal
} // namespace VcProjectManager
