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
#ifndef VCPROJECTMANAGER_INTERNAL_VC_SCHEMA_MANAGER_H
#define VCPROJECTMANAGER_INTERNAL_VC_SCHEMA_MANAGER_H

#include "vcprojectmanagerconstants.h"

#include <QHash>
#include <QMap>
#include <QObject>

namespace VcProjectManager {
namespace Internal {

class VcSchemaManager : public QObject
{
    friend class VcProjectManagerPlugin;
    Q_OBJECT

public:
    static VcSchemaManager *instance();
    ~VcSchemaManager();

    void addDocumentSchema(const QString &schemaPath, Constants::SchemaVersion version);
    QString documentSchema(Constants::SchemaVersion version);
    void setDocumentSchema(Constants::SchemaVersion version, const QString &schemaPath);
    void removeDocumentSchema(Constants::SchemaVersion version);

    QString toolSchema() const;
    void setToolSchema(const QString &schemaPath);

    QList<QString> toolXMLFilePaths() const;
    void addToolXML(const QString &toolKey, const QString &toolFilePath);
    void removeToolXML(const QString &toolKey);
    void removeToolSchemas();

    void removeAllSchemas();

    void saveSettings();

private:
    VcSchemaManager();
    void loadSettings();
    bool similarToolXMLExistsFor(const QString &filePath);

    static VcSchemaManager *m_instance;
    QMap<Constants::SchemaVersion, QString> m_documentSchemas;
    QString m_toolSchema;
    QHash<QString, QString> m_toolXMLPaths; //<tool_key, tool_xml_file_path>
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VC_SCHEMA_MANAGER_H
