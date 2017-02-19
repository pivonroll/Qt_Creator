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
#include "menuhandler.h"
#include "vcdocprojectnodes.h"
#include "common/vcprojectmanagerconstants.h"

#include <widgets/visualstudiowidgets/vcenternamedialog.h>

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projecttree.h>

#include <QAction>

namespace VcProjectManager {
namespace Internal {

MenuHandler::MenuHandler()
{
    initialize();
    initialize2005();
}

MenuHandler *MenuHandler::instance()
{
    static MenuHandler instance;
    return &instance;
}

MenuHandler::~MenuHandler()
{
}

/*!
 * \brief Initializes context menu actions for project nodes, folder nodes and file nodes for
 * Visual Studio projects 2003 and 2008.
 */
void MenuHandler::initialize()
{
    const Core::Context projectContext(Constants::VC_PROJECT_ID);

    Core::ActionContainer *projectContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_PROJECTCONTEXT);
    Core::ActionContainer *subProjectContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_SUBPROJECTCONTEXT);
    Core::ActionContainer *folderContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_FOLDERCONTEXT);
    Core::ActionContainer *fileContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_FILECONTEXT);

    // versions 2003 and 2005
    m_addFilter = new QAction(tr("Add Filter..."), this);
    Core::Command *cmd = Core::ActionManager::registerAction(m_addFilter, Constants::VC_PROJECT_ADD_FILTER_ACTION,
                                                             projectContext);
    cmd->setAttribute(Core::Command::CA_Hide);
    projectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    subProjectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    folderContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FOLDER_FILES);

    m_removeFilter = new QAction(tr("Remove Filter"), this);
    cmd = Core::ActionManager::registerAction(m_removeFilter, Constants::VC_PROJECT_REMOVE_FILTER_ACTION,
                                              projectContext);
    cmd->setAttribute(Core::Command::CA_Hide);
    folderContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FOLDER_FILES);

    m_projectProperties = new QAction(tr("Show Properties..."), this);
    cmd = Core::ActionManager::registerAction(m_projectProperties, Constants::VC_PROJECT_SHOW_PROPERTIES_ACTION,
                                              projectContext);
    cmd->setAttribute(Core::Command::CA_Hide);
    projectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    subProjectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);

    m_fileProperties = new QAction(tr("Show Properties..."), this);
    cmd = Core::ActionManager::registerAction(m_fileProperties, Constants::VC_FILE_SHOW_PROPERTIES_ACTION,
                                              projectContext);
    cmd->setAttribute(Core::Command::CA_Hide);
    fileContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FILE_OTHER);

    connect(m_projectProperties, &QAction::triggered, this, &MenuHandler::onShowProjectSettings);
    connect(m_addFilter, &QAction::triggered, this, &MenuHandler::onAddFilter);
    connect(m_removeFilter, &QAction::triggered, this, &MenuHandler::onRemoveFilter);
    connect(m_fileProperties, &QAction::triggered, this, &MenuHandler::onShowFileSettings);
}

/*!
 * \brief Initializes context menu actions for project nodes, folder nodes and file nodes for
 * Visual Studio projects 2005.
 */
void MenuHandler::initialize2005()
{
    const Core::Context project2005Context(Constants::VC_PROJECT_2005_ID);

    Core::ActionContainer *projectContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_PROJECTCONTEXT);
    Core::ActionContainer *subProjectContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_SUBPROJECTCONTEXT);
    Core::ActionContainer *folderContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_FOLDERCONTEXT);
    Core::ActionContainer *fileContextMenu =
            Core::ActionManager::createMenu(ProjectExplorer::Constants::M_FILECONTEXT);

    // version 2005
    m_addFolder2005 = new QAction(tr("Add Folder..."), this);
    Core::Command *cmd = Core::ActionManager::registerAction(m_addFolder2005, Constants::VC_PROJECT_ADD_FOLDER_ACTION,
                                                             project2005Context);
    cmd->setAttribute(Core::Command::CA_Hide);
    projectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    subProjectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    folderContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FOLDER_FILES);

    m_addFilter2005 = new QAction(tr("Add Filter..."), this);
    cmd = Core::ActionManager::registerAction(m_addFilter2005, Constants::VC_PROJECT_ADD_FILTER_ACTION,
                                              project2005Context);
    cmd->setAttribute(Core::Command::CA_Hide);
    projectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    subProjectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    folderContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FOLDER_FILES);

    m_removeFolder2005 = new QAction(tr("Remove Folder"), this);
    cmd = Core::ActionManager::registerAction(m_removeFolder2005, Constants::VC_PROJECT_REMOVE_FOLDER_ACTION,
                                              project2005Context);
    cmd->setAttribute(Core::Command::CA_Hide);
    folderContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FOLDER_FILES);

    m_removeFilter2005 = new QAction(tr("Remove Filter"), this);
    cmd = Core::ActionManager::registerAction(m_removeFilter2005, Constants::VC_PROJECT_REMOVE_FILTER_ACTION,
                                              project2005Context);
    cmd->setAttribute(Core::Command::CA_Hide);
    folderContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FOLDER_FILES);

    m_projectProperties2005 = new QAction(tr("Show Properties..."), this);
    cmd = Core::ActionManager::registerAction(m_projectProperties2005, Constants::VC_PROJECT_SHOW_PROPERTIES_ACTION,
                                              project2005Context);
    cmd->setAttribute(Core::Command::CA_Hide);
    projectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);
    subProjectContextMenu->addAction(cmd, ProjectExplorer::Constants::G_PROJECT_FILES);

    m_fileProperties2005 = new QAction(tr("Show Properties..."), this);
    cmd = Core::ActionManager::registerAction(m_fileProperties2005, Constants::VC_FILE_SHOW_PROPERTIES_ACTION,
                                              project2005Context);
    cmd->setAttribute(Core::Command::CA_Hide);
    fileContextMenu->addAction(cmd, ProjectExplorer::Constants::G_FILE_OTHER);

    connect(m_projectProperties2005, &QAction::triggered, this, &MenuHandler::onShowProjectSettings);
    connect(m_addFilter2005, &QAction::triggered, this, &MenuHandler::onAddFilter);
    connect(m_removeFilter2005, &QAction::triggered, this, &MenuHandler::onRemoveFilter);
    connect(m_addFolder2005, &QAction::triggered, this, &MenuHandler::onAddFolder);
    connect(m_removeFolder2005, &QAction::triggered, this, &MenuHandler::onRemoveFolder);
    connect(m_fileProperties2005, &QAction::triggered, this, &MenuHandler::onShowFileSettings);
}

/*!
 * \brief Shows a settings dialog for Visual Studio project node.
 */
void MenuHandler::onShowProjectSettings()
{
    ProjectExplorer::Node *node = ProjectExplorer::ProjectTree::currentNode();

    if (node && node->nodeType() == ProjectExplorer::NodeType::Project) {
        VcDocProjectNode *projectNode = static_cast<VcDocProjectNode *>(node);
        projectNode->showSettingsDialog();
    }
}

/*!
 * \brief Adds a folder to a project or a folder node.
 */
void MenuHandler::onAddFolder()
{
    VcEnterNameDialog dlg(QLatin1String("Folder:"));

    if (dlg.exec() == QDialog::Accepted) {
        QString folderName = dlg.containerName();
        ProjectExplorer::Node *node = ProjectExplorer::ProjectTree::currentNode();

        QTC_ASSERT(node, return);

        if (node->nodeType() == ProjectExplorer::NodeType::Folder) {
            VcFileContainerNode *folderNode = static_cast<VcFileContainerNode *>(node);
            folderNode->addFileContainerNode(folderName, VcFileContainerNode::VcContainerType_Folder);
        }

        if (node->nodeType() == ProjectExplorer::NodeType::Project) {
            VcDocProjectNode *projectNode = static_cast<VcDocProjectNode *>(node);
            projectNode->addFileContainerNode(folderName, VcFileContainerNode::VcContainerType_Folder);
        }
    }
}

/*!
 * \brief Adds a filter to a project or a filter node.
 */
void MenuHandler::onAddFilter()
{
    VcEnterNameDialog dlg(QLatin1String("Filter:"));

    if (dlg.exec() == QDialog::Accepted) {
        QString filterName = dlg.containerName();
        ProjectExplorer::Node *node = ProjectExplorer::ProjectTree::currentNode();

        QTC_ASSERT(node, return);

        if (node->nodeType() == ProjectExplorer::NodeType::Folder) {
            VcFileContainerNode *folderNode = static_cast<VcFileContainerNode *>(node);
            folderNode->addFileContainerNode(filterName);
        }

        if (node->nodeType() == ProjectExplorer::NodeType::Project) {
            VcDocProjectNode *projectNode = static_cast<VcDocProjectNode *>(node);
            projectNode->addFileContainerNode(filterName, VcFileContainerNode::VcContainerType_Filter);
        }
    }
}

/*!
 * \brief Removes a filter from a project or a filter node.
 */
void MenuHandler::onRemoveFilter()
{
    ProjectExplorer::Node *node = ProjectExplorer::ProjectTree::currentNode();

    if (node && node->nodeType() == ProjectExplorer::NodeType::Folder) {
        VcFileContainerNode *filterNode = static_cast<VcFileContainerNode *>(node);
        ProjectExplorer::FolderNode *parentNode = filterNode->parentFolderNode();

        if (parentNode->nodeType() == ProjectExplorer::NodeType::Folder) {
            VcFileContainerNode *projectNode = static_cast<VcFileContainerNode *>(parentNode);
            projectNode->removeFileContainerNode(filterNode);
        }

        else if (parentNode->nodeType() == ProjectExplorer::NodeType::Project) {
            VcDocProjectNode *projectNode = static_cast<VcDocProjectNode *>(parentNode);
            projectNode->removeFileContainerNode(filterNode);
        }
    }
}

/*!
 * \brief Removes a folder from a project or a folder node.
 */
void MenuHandler::onRemoveFolder()
{
    ProjectExplorer::Node *node = ProjectExplorer::ProjectTree::currentNode();

    if (node && node->nodeType() == ProjectExplorer::NodeType::Folder) {
        VcFileContainerNode *folderNode = static_cast<VcFileContainerNode *>(node);
        ProjectExplorer::FolderNode *parentNode = folderNode->parentFolderNode();

        if (parentNode->nodeType() == ProjectExplorer::NodeType::Folder) {
            VcFileContainerNode *parentFolderNode = static_cast<VcFileContainerNode *>(parentNode);
            parentFolderNode->removeFileContainerNode(folderNode);
        }

        else if (parentNode->nodeType() == ProjectExplorer::NodeType::Project) {
            VcDocProjectNode *projectNode = static_cast<VcDocProjectNode *>(parentNode);
            projectNode->removeFileContainerNode(folderNode);
        }
    }
}

/*!
 * \brief Shows a settings dialog for files in Visual Studio project.
 */
void MenuHandler::onShowFileSettings()
{
    ProjectExplorer::Node *node = ProjectExplorer::ProjectTree::currentNode();

    if (node && node->nodeType() == ProjectExplorer::NodeType::File) {
        VcFileNode *fileNode = static_cast<VcFileNode *>(node);
        fileNode->showSettingsWidget();
    }
}

} // namespace Internal
} // namespace VcProjectManager
