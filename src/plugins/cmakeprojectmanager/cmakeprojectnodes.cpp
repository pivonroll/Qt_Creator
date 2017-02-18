/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "cmakeprojectnodes.h"

#include "cmakeprojectconstants.h"

#include <coreplugin/fileiconprovider.h>

#include <utils/algorithm.h>

#include <QApplication>

using namespace CMakeProjectManager;
using namespace CMakeProjectManager::Internal;

CMakeInputsNode::CMakeInputsNode(const Utils::FileName &cmakeLists) :
    ProjectExplorer::ProjectNode(CMakeInputsNode::inputsPathFromCMakeListsPath(cmakeLists))
{
    setPriority(Node::DefaultPriority - 10); // Bottom most!
    setDisplayName(QCoreApplication::translate("CMakeFilesProjectNode", "CMake Modules"));
    setIcon(QIcon(":/projectexplorer/images/session.png")); // TODO: Use a better icon!
}

Utils::FileName CMakeInputsNode::inputsPathFromCMakeListsPath(const Utils::FileName &cmakeLists)
{
    Utils::FileName result = cmakeLists;
    result.appendPath("cmakeInputs"); // cmakeLists is a file, so this can not exist on disk
    return result;
}

bool CMakeInputsNode::showInSimpleTree() const
{
    return false;
}

QList<ProjectExplorer::ProjectAction> CMakeInputsNode::supportedActions(ProjectExplorer::Node *node) const
{
    Q_UNUSED(node);
    return QList<ProjectExplorer::ProjectAction>();
}

CMakeListsNode::CMakeListsNode(const Utils::FileName &cmakeListPath) :
    ProjectExplorer::ProjectNode(cmakeListPath)
{
    static QIcon folderIcon;
    if (folderIcon.isNull()) {
        const QIcon overlayIcon(Constants::FILEOVERLAY_CMAKE);
        QPixmap dirPixmap = qApp->style()->standardIcon(QStyle::SP_DirIcon).pixmap(QSize(16, 16));

        folderIcon.addPixmap(Core::FileIconProvider::overlayIcon(dirPixmap, overlayIcon));
    }
    setIcon(folderIcon);
}

bool CMakeListsNode::showInSimpleTree() const
{
    return false;
}

QList<ProjectExplorer::ProjectAction> CMakeListsNode::supportedActions(ProjectExplorer::Node *node) const
{
    Q_UNUSED(node);
    return QList<ProjectExplorer::ProjectAction>();
}

CMakeProjectNode::CMakeProjectNode(const Utils::FileName &directory) :
    ProjectExplorer::ProjectNode(directory)
{
    setPriority(Node::DefaultProjectPriority + 1000);
    setIcon(QIcon(":/projectexplorer/images/projectexplorer.png")); // TODO: Use proper icon!
}

bool CMakeProjectNode::showInSimpleTree() const
{
    return true;
}

QString CMakeProjectNode::tooltip() const
{
    return QString();
}

QList<ProjectExplorer::ProjectAction> CMakeProjectNode::supportedActions(ProjectExplorer::Node *node) const
{
    Q_UNUSED(node);
    return QList<ProjectExplorer::ProjectAction>();
}

CMakeTargetNode::CMakeTargetNode(const Utils::FileName &directory) :
    ProjectExplorer::ProjectNode(directory)
{
    setPriority(Node::DefaultProjectPriority + 900);
    setIcon(QIcon(":/projectexplorer/images/build.png")); // TODO: Use proper icon!
}

bool CMakeTargetNode::showInSimpleTree() const
{
    return true;
}

QString CMakeTargetNode::tooltip() const
{
    return m_tooltip;
}

QList<ProjectExplorer::ProjectAction> CMakeTargetNode::supportedActions(ProjectExplorer::Node *node) const
{
    Q_UNUSED(node);
    return QList<ProjectExplorer::ProjectAction>();
}

void CMakeTargetNode::setTargetInformation(const QList<Utils::FileName> &artifacts,
                                           const QString &type)
{
    m_tooltip = QCoreApplication::translate("CMakeTargetNode", "Target type: ") + type + "<br>";
    if (artifacts.count() == 0) {
        m_tooltip += QCoreApplication::translate("CMakeTargetNode", "No build artifacts");
    } else {
        const QStringList tmp = Utils::transform(artifacts, &Utils::FileName::toUserOutput);
        m_tooltip += QCoreApplication::translate("CMakeTargetNode", "Build artifacts:<br>")
                + tmp.join("<br>");
    }
}
