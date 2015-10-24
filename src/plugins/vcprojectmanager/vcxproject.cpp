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
#include "vcxproject.h"

#include "vcdocprojectnodes.h"
#include "vcxprojectfile.h"
#include "vcxprojectmanager.h"


namespace VcProjectManager {
namespace Internal {

VcXProject::VcXProject(VcXProjectManager *projectManager, const QString &canonicalFilePath)
    : m_projectManager(projectManager)
{
    m_projectFile = new VcXProjectFile(canonicalFilePath);
    m_rootNode = m_projectFile->createProjectNode();

    // create project nodes
}

VcXProject::~VcXProject()
{
    VcDocProjectNode *temp = m_rootNode;
    m_rootNode = nullptr;
    delete temp;
    delete m_projectFile;
}

QString VcXProject::displayName() const
{
    if (m_rootNode)
        return m_rootNode->displayName();

    return QString();
}

Core::IDocument *VcXProject::document() const
{
    return m_projectFile;
}

ProjectExplorer::IProjectManager *VcXProject::projectManager() const
{
    return m_projectManager;
}

ProjectExplorer::ProjectNode *VcXProject::rootProjectNode() const
{
    return m_rootNode;
}

QStringList VcXProject::files(ProjectExplorer::Project::FilesMode fileMode) const
{
    Q_UNUSED(fileMode)
    return QStringList();
}

} // namespace Internal
} // namespace VcProjectManager

