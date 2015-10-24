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
#include "toolsectionsettingswidget.h"
#include "toolsettingswidget.h"

#include <visualstudiointerfaces/isectioncontainer.h>
#include <visualstudiointerfaces/itoolsection.h>
#include <visualstudiointerfaces/iconfigurationbuildtool.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <QTableWidget>
#include <QVBoxLayout>

namespace VcProjectManager {
namespace Internal {

/*!
 * \class ToolSettingsWidget
 * Represents a settings widget for a tool.
 * It consists of a tab widget with all tool section widgets added to it.
 */
ToolSettingsWidget::ToolSettingsWidget(IConfigurationBuildTool *tool, QWidget *parent)
    : VcNodeWidget(parent),
      m_tool(tool)
{
    QTabWidget *mainTabWidget = new QTabWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(mainTabWidget);
    setLayout(layout);

    if (!m_tool)
        return;

    for (int i = 0; i < m_tool->sectionContainer()->sectionCount(); ++i) {
        IToolSection *toolSection = m_tool->sectionContainer()->section(i);

        if (toolSection) {
            VcNodeWidget *toolSectionWidget = toolSection->createSettingsWidget();
            mainTabWidget->addTab(toolSectionWidget, toolSection->sectionDescription()->displayName());
            m_sections.append(toolSectionWidget);
        }
    }
}

/*!
 * \reimp
 */
void ToolSettingsWidget::saveData()
{
    foreach (VcNodeWidget *toolSectionWidget, m_sections)
        toolSectionWidget->saveData();
}

} // namespace Internal
} // namespace VcProjectManager
