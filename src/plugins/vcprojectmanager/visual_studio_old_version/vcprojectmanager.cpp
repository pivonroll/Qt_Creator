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
#include "../common/vcprojectmanagerconstants.h"
#include "../common/vcschemamanager.h"
#include "../common/projectconstants.h"
#include "../vcprojectbuildoptionspage.h"
#include "vcproject.h"
#include "vcprojectmanager.h"
#include "utils.h"
#include "vcprojectmodel/vcprojectdocument_constants.h"

#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class VcManager inheriates from ProjectExplorer::IProjectManager.
 * When Qt Creator wants to open Visual Studio project files,
 * it will create an instance of this class.
 */
VcProjectManager::VcProjectManager(VcProjectBuildOptionsPage *configPage)
{
    Q_UNUSED(configPage)
}

QString VcProjectManager::mimeType() const
{
    return QLatin1String(Constants::VCPROJ_MIMETYPE);
}

ProjectExplorer::Project *VcProjectManager::openProject(const QString &fileName, QString *errorString)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    // Check whether the project file exists.
    if (canonicalFilePath.isEmpty()) {
        if (errorString)
            *errorString = tr("Failed opening project '%1': Project file does not exist").
                arg(QDir::toNativeSeparators(fileName));
        return nullptr;
    }

    // check if project is a valid vc project
    // versions supported are 2003, 2005 and 2008
    DocumentVersion docVersion = VisualStudioUtils::getProjectVersion(canonicalFilePath);

    if (docVersion != DV_UNRECOGNIZED) {
        if (errorString)
            errorString->clear();
        return new VcProject(this, canonicalFilePath, docVersion);
    }

    if (errorString)
        *errorString = tr("Could not open project %1").arg(fileName);

    return nullptr;
}


} // namespace Internal
} // namespace VcProjectManager
