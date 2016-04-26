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
#include "toolsectioncontainerx.h"

#include <visualstudiotoolattributes/attributedescriptiondataitem.h>
#include <visualstudiotoolattributes/toolsectiondescription.h>

#include <visualstudiointerfaces/itoolattribute.h>
#include <visualstudiointerfaces/itoolattributecontainer.h>
#include <visualstudiointerfaces/itoolsection.h>

#include <QDomElement>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ToolSectionContainerX::ToolSectionContainerX(VisualStudioProjectX::Project *project, ConfigurationX *configurationX)
    : m_project(project)
    , m_configurationX(configurationX)
{
}

ToolSectionContainerX::ToolSectionContainerX(const ToolSectionContainerX &toolSec)
{
    qDeleteAll(m_toolSections);
    m_toolSections.clear();

    foreach (IToolSection *sec, toolSec.m_toolSections) {
        if (sec)
            m_toolSections.append(sec->clone());
    }
}

ToolSectionContainerX &ToolSectionContainerX::operator =(const ToolSectionContainerX &toolSec)
{
    if (this != &toolSec) {
        qDeleteAll(m_toolSections);
        m_toolSections.clear();

        for (int i = 0; i < toolSec.sectionCount(); ++i) {
            IToolSection *sec = toolSec.section(i);
            if (sec)
                m_toolSections.append(sec->clone());
        }
    }

    return *this;
}

ToolSectionContainerX::~ToolSectionContainerX()
{
    qDeleteAll(m_toolSections);
}

IToolSection *ToolSectionContainerX::section(int index) const
{
    if (0 <= index && index < m_toolSections.size())
        return m_toolSections[index];
    return 0;
}

IToolSection *ToolSectionContainerX::section(const QString &sectionName) const
{
    foreach (IToolSection *sec, m_toolSections) {
        if (sec && sec->sectionDescription()->displayName() == sectionName) {
            return sec;
        }
    }
    return 0;
}

int ToolSectionContainerX::sectionCount() const
{
    return m_toolSections.size();
}

void ToolSectionContainerX::appendSection(IToolSection *section)
{
    if (!section)
        return;

    foreach (IToolSection *sec, m_toolSections) {
        if (sec && sec->sectionDescription()->displayName() == section->sectionDescription()->displayName())
            return;
    }
    m_toolSections.append(section);
}

void ToolSectionContainerX::removeSection(const QString &sectionName)
{
    foreach (IToolSection *sec, m_toolSections) {
        if (sec && sec->sectionDescription()->displayName() == sectionName) {
            m_toolSections.removeOne(sec);
            delete sec;
            return;
        }
    }
}

void ToolSectionContainerX::appendToXMLNode(QDomElement &elementNode, QDomDocument &domXMLDocument)
{
    Q_UNUSED(elementNode)
    Q_UNUSED(domXMLDocument)
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
