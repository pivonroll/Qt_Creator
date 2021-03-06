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
#include "fileconfigurationsettingswidget.h"
#include "ui_fileconfigurationsettingswidget.h"

#include <vcprojectmodel/vcprojectdocument_constants.h>

#include <visualstudiotoolattributes/tooldescription.h>
#include <visualstudiotoolattributes/tooldescriptiondatamanager.h>

#include <visualstudiointerfaces/iattributecontainer.h>
#include <visualstudiointerfaces/iconfiguration.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiointerfaces/iconfigurationbuildtools.h>
#include <visualstudiointerfaces/iconfigurationcontainer.h>
#include <visualstudiointerfaces/iconfigurations.h>
#include <visualstudiointerfaces/itools.h>
#include <visualstudiointerfaces/ivisualstudioproject.h>

#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class FileConfigurationSettingsWidget
 * Represents a widget which will show all build configuration widgets for a file.
 *
 * \fn void saveData()
 * \reimp
 */

FileConfigurationSettingsWidget::FileConfigurationSettingsWidget(IConfiguration *fileBuildConfig, IVisualStudioProject *parentProjectDoc, QWidget *parent) :
    VcNodeWidget(parent),
    ui(new Ui::FileConfigurationSettingsWidget),
    m_fileBuildConfig(fileBuildConfig),
    m_toolSettingsWidget(nullptr),
    m_parentProjectDoc(parentProjectDoc)
{
    ui->setupUi(this);

    readTools();

    if (m_fileBuildConfig &&
            m_fileBuildConfig->tools() &&
            m_fileBuildConfig->tools()->configurationBuildTools()) {

        if (m_fileBuildConfig->tools()->configurationBuildTools()->tool(0)) {
            QVBoxLayout *layout = new QVBoxLayout;
            m_toolSettingsWidget = m_fileBuildConfig->tools()->configurationBuildTools()->tool(0)->createSettingsWidget();
            layout->addWidget(m_toolSettingsWidget);
            ui->m_toolWidget->setLayout(layout);
        }
    }

    QString excludeFromBuild = m_fileBuildConfig->attributeContainer()->attributeValue(QLatin1String(VcDocConstants::VS_PROJECT_CONFIG_EXCLUDED));

    if (excludeFromBuild.isEmpty() || excludeFromBuild == QLatin1String("false"))
        ui->m_excludedFromBuild->setCurrentIndex(0);
    else
        ui->m_excludedFromBuild->setCurrentIndex(1);


    QMapIterator<QString, IConfigurationBuildTool *> it(m_toolMap);

    while (it.hasNext()) {
        it.next();
        ui->m_tool->addItem(it.value()->toolDescription()->toolDisplayName(), it.key());
    }

    if (m_fileBuildConfig && m_fileBuildConfig->tools() && m_fileBuildConfig->tools()->configurationBuildTools() &&
            m_fileBuildConfig->tools()->configurationBuildTools()->tool(0) &&
            m_fileBuildConfig->tools()->configurationBuildTools()->tool(0)->toolDescription()) {
        int index = toolIndex(m_fileBuildConfig->tools()->configurationBuildTools()->tool(0)->toolDescription()->toolKey());
        if (index != -1)
            ui->m_tool->setCurrentIndex(index);
    }

    connect(ui->m_tool, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTool(int)));
}

FileConfigurationSettingsWidget::~FileConfigurationSettingsWidget()
{
    delete ui;
}

void FileConfigurationSettingsWidget::saveData()
{
    if (m_toolSettingsWidget)
        m_toolSettingsWidget->saveData();

    QTC_ASSERT(m_fileBuildConfig, return);
    QTC_ASSERT(ui->m_tool, return);

    ITools *tools = m_fileBuildConfig->tools();

    QTC_ASSERT(tools, return);

    IConfigurationBuildTools *buildTools = tools->configurationBuildTools();

    QTC_ASSERT(buildTools, return);

    IConfigurationBuildTool *tool = buildTools->tool(0);

    QTC_ASSERT(tool, return);

    const ToolDescription *toolDescription = tool->toolDescription();

    QTC_ASSERT(toolDescription, return);

    const QString currentTool = ui->m_tool->itemData(ui->m_tool->currentIndex()).toString();

    if (toolDescription->toolKey() != currentTool) {
        buildTools->removeTool(tool);
        delete tool;
        tool = m_toolMap[currentTool];
        if (tool)
            buildTools->addTool(tool);
    }
}

void FileConfigurationSettingsWidget::changeTool(int index)
{
    // remove previous tool
    if (m_toolSettingsWidget) {
        ui->m_toolWidget->layout()->removeWidget(m_toolSettingsWidget);
        // flush data in order to save them
        m_toolSettingsWidget->saveData();
        m_toolSettingsWidget->deleteLater();
        m_toolSettingsWidget = nullptr;
    }

    // add settings widget for the selected tool
    QString toolKey = ui->m_tool->itemData(index).toString();
    IConfigurationBuildTool *tool = m_toolMap[toolKey];

    if (tool) {
        m_toolSettingsWidget = tool->createSettingsWidget();
        ui->m_toolWidget->layout()->addWidget(m_toolSettingsWidget);
    }
}

int FileConfigurationSettingsWidget::toolIndex(const QString &toolKey)
{
    for (int i = 0; i < ui->m_tool->count(); ++i) {
        if (ui->m_tool->itemData(i).toString() == toolKey)
            return i;
    }

    return -1;
}

void FileConfigurationSettingsWidget::readTools()
{
    if (m_fileBuildConfig && m_fileBuildConfig->tools() &&
            m_fileBuildConfig->tools()->configurationBuildTools() &&
            m_fileBuildConfig->tools()->configurationBuildTools()->tool(0) &&
            m_fileBuildConfig->tools()->configurationBuildTools()->tool(0)->toolDescription()) {
        QString key = m_fileBuildConfig->tools()->configurationBuildTools()->tool(0)->toolDescription()->toolKey();
        m_toolMap[key] = m_fileBuildConfig->tools()->configurationBuildTools()->tool(0);
    }

    if (m_parentProjectDoc && m_parentProjectDoc->configurations() &&
            m_parentProjectDoc->configurations()->configurationContainer()) {
        IConfiguration *parentConfig = m_parentProjectDoc->configurations()->configurationContainer()->configuration(m_fileBuildConfig->fullName());

        if (parentConfig && parentConfig->tools() && parentConfig->tools()->configurationBuildTools()) {
            for (int i = 0; i < parentConfig->tools()->configurationBuildTools()->toolCount(); ++i) {
                IConfigurationBuildTool *tool = parentConfig->tools()->configurationBuildTools()->tool(i);

                if (tool && tool->toolDescription() &&
                        !m_toolMap.contains(tool->toolDescription()->toolKey()))
                    m_toolMap[tool->toolDescription()->toolKey()] = tool->clone();
            }
        }
    }
}

} // namespace Internal
} // namespace VcProjectManager
