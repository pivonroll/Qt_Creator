#include "utils.h"

#include "vcschemamanager.h"

#include <QFile>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

namespace VcProjectManager {
namespace Internal {
namespace Utils {

/*!
 * \return \b true if Visual Studio project at \a filePath is a Visual Studio 2003 project.
 * Otherwise it returns \b false.
 */
bool checkIfVersion2003(const QString &filePath)
{
    VcSchemaManager *schemaMgr = VcSchemaManager::instance();
    QString vc2003Schema = schemaMgr->documentSchema(Constants::SV_2003);

    if (vc2003Schema.isEmpty()) {
        return false;
    }

    QFile schemaFile(vc2003Schema);
    schemaFile.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
            return true;
    }

    return false;
}

/*!
 * \return \b true if Visual Studio project at \a filePath is a Visual Studio 2005 project.
 * Otherwise it returns \b false.
 */
bool checkIfVersion2005(const QString &filePath)
{
    VcSchemaManager *schemaMgr = VcSchemaManager::instance();
    QString vc2005Schema = schemaMgr->documentSchema(Constants::SV_2005);
    if (vc2005Schema.isEmpty())
        return false;

    QFile schemaFile(vc2005Schema);
    schemaFile.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
            return true;
    }

    return false;
}

/*!
 * \return \b true if Visual Studio project at \a filePath is a Visual Studio 2008 project.
 * Otherwise it returns \b false.
 */
bool checkIfVersion2008(const QString &filePath)
{
    VcSchemaManager *schemaMgr = VcSchemaManager::instance();
    QString vc2008Schema = schemaMgr->documentSchema(Constants::SV_2008);
    if (vc2008Schema.isEmpty())
        return false;

    QFile schemaFile(vc2008Schema);
    schemaFile.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
            return true;
    }

    return false;
}

/*!
 * \return version of Visual Studio Project. Project is passed as the \a projectPath.
 */
VcDocConstants::DocumentVersion getProjectVersion(const QString &projectPath)
{
    if (checkIfVersion2003(projectPath))
        return VcDocConstants::DV_MSVC_2003;
    else if (checkIfVersion2005(projectPath))
        return VcDocConstants::DV_MSVC_2005;
    else if (checkIfVersion2008(projectPath))
        return VcDocConstants::DV_MSVC_2008;

    return VcDocConstants::DV_UNRECOGNIZED;
}

} // namespace Utils
} // namespace Internal
} // namespace VcProjectManager
