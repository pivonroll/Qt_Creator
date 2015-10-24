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
#include "vcxprojectfile.h"

#include "vcdocprojectnodes.h"
#include "vcprojectxmodel/vcxproj/vcprojectdocumentx.h"

#include <QFileInfo>

namespace VcProjectManager {
namespace Internal {

VcXProjectFile::VcXProjectFile(const QString &filePath)
{
    m_visualStudioProject = new VcProjectDocumentX(filePath);
    setFilePath(Utils::FileName::fromString(filePath));
}

VcXProjectFile::~VcXProjectFile()
{
    delete m_visualStudioProject;
}

bool VcXProjectFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    Q_UNUSED(errorString)
    Q_UNUSED(fileName)
    Q_UNUSED(autoSave)
    return false;
}

QString VcXProjectFile::defaultPath() const
{
    return m_visualStudioProject->filePath();
}

QString VcXProjectFile::suggestedFileName() const
{
    return QFileInfo(m_visualStudioProject->filePath()).fileName();
}

bool VcXProjectFile::isModified() const
{
    return false;
}

bool VcXProjectFile::isSaveAsAllowed() const
{
    return false;
}

bool VcXProjectFile::reload(QString *errorString, Core::IDocument::ReloadFlag flag, Core::IDocument::ChangeType type)
{
    Q_UNUSED(errorString)
    Q_UNUSED(flag)
    Q_UNUSED(type)
    return false;
}

VcDocProjectNode *VcXProjectFile::createProjectNode() const
{
    return new VcDocProjectNode(m_visualStudioProject);
}

IVisualStudioProject *VcXProjectFile::visualStudioProject() const
{
    return m_visualStudioProject;
}

} // namespace Internal
} // namespace VcProjectManager

