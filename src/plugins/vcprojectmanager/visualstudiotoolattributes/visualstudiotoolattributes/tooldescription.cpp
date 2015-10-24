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
#include "tooldescription.h"

#include <visualstudiointerfaces/iattributedescriptiondataitem.h>
#include <visualstudiointerfaces/itoolattribute.h>

#include "../../vcprojectmodel/tools/configurationtool.h"
#include "toolsectiondescription.h"

#include <utils/qtcassert.h>

namespace VcProjectManager {
namespace Internal {

ToolDescription::ToolDescription()
{
}

ToolDescription::~ToolDescription()
{
}

IToolSectionDescription *ToolDescription::sectionDescription(int index) const
{
    QTC_ASSERT(0 <= index && index < m_sectionDescriptions.size(), return 0);
    return m_sectionDescriptions[index];
}

int ToolDescription::sectionDescriptionCount() const
{
    return m_sectionDescriptions.size();
}

void ToolDescription::removeSectionDescription(IToolSectionDescription *sectionDescription)
{
    if (!sectionDescription)
        return;

    if (m_sectionDescriptions.contains(sectionDescription)) {
        m_sectionDescriptions.removeOne(sectionDescription);
        delete sectionDescription;
        return;
    }

    foreach (IToolSectionDescription* toolSectionDesc, m_sectionDescriptions) {
        if (toolSectionDesc->displayName() == sectionDescription->displayName()) {
            m_sectionDescriptions.removeOne(sectionDescription);
            delete sectionDescription;
            return;
        }
    }
}

void ToolDescription::addSectionDescription(IToolSectionDescription *sectionDescription)
{
    foreach (IToolSectionDescription* toolSectionDesc, m_sectionDescriptions) {
        if (toolSectionDesc->displayName() == sectionDescription->displayName())
            return;
    }

    m_sectionDescriptions.append(sectionDescription);
}

QString ToolDescription::toolKey() const
{
    return m_toolKey;
}

void ToolDescription::setToolKey(const QString &toolKey)
{
    m_toolKey = toolKey;
}

QString ToolDescription::toolDisplayName() const
{
    return m_displayName;
}

void ToolDescription::setToolDisplayName(const QString &toolName)
{
    m_displayName = toolName;
}

} // namespace Internal
} // namespace VcProjectManager
