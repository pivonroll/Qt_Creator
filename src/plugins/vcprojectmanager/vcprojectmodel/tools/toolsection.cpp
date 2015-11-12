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
#include "toolsection.h"

#include <visualstudiotoolattributes/generaltoolattributecontainer.h>

#include <visualstudiotoolattributes/toolsectiondescription.h>
#include <visualstudiotoolattributes/attributedescriptiondataitem.h>

#include <visualstudiowidgets/toolwidgets/toolsectionsettingswidget.h>

#include <visualstudiointerfaces/itoolattribute.h>

#include "toolattributes/booltoolattribute.h"
#include "toolattributes/integertoolattribute.h"
#include "toolattributes/stringlisttoolattribute.h"
#include "toolattributes/stringtoolattribute.h"

namespace VcProjectManager {
namespace Internal {

ToolSection::ToolSection(const ToolSectionDescription *toolSectionDesc)
    : m_toolDesc(toolSectionDesc)
    , m_attributeContainer(new GeneralToolAttributeContainer)
{
    for (int i = 0; i < toolSectionDesc->attributeDescriptionCount(); ++i) {

        if (!toolSectionDesc->attributeDescription(i))
            continue;

            IToolAttribute *toolAttribute = nullptr;

            if (toolSectionDesc->attributeDescription(i)->type() == QLatin1String("Boolean"))
                toolAttribute = new BoolToolAttribute(toolSectionDesc->attributeDescription(i));
            else if (toolSectionDesc->attributeDescription(i)->type() == QLatin1String("Integer"))
                toolAttribute = new IntegerToolAttribute(toolSectionDesc->attributeDescription(i));
            else if (toolSectionDesc->attributeDescription(i)->type() == QLatin1String("String"))
                toolAttribute = new StringToolAttribute(toolSectionDesc->attributeDescription(i));
            else if (toolSectionDesc->attributeDescription(i)->type() == QLatin1String("StringList"))
                toolAttribute = new StringListToolAttribute(toolSectionDesc->attributeDescription(i));

            if (toolAttribute)
                m_attributeContainer->addToolAttribute(toolAttribute);
    }
}

ToolSection::ToolSection(const ToolSection &toolSec)
{
    m_attributeContainer = new GeneralToolAttributeContainer;
    *m_attributeContainer = *toolSec.m_attributeContainer;

    m_toolDesc = toolSec.m_toolDesc;

    for (int i = 0; i < toolSec.attributeContainer()->toolAttributeCount(); ++i)
        m_attributeContainer->addToolAttribute(toolSec.attributeContainer()->toolAttribute(i)->clone());
}

ToolSection::~ToolSection()
{
    delete m_attributeContainer;
}

IToolAttributeContainer *ToolSection::attributeContainer() const
{
    return m_attributeContainer;
}

const ToolSectionDescription *ToolSection::sectionDescription() const
{
    return m_toolDesc;
}

VcNodeWidget *ToolSection::createSettingsWidget()
{
    return new ToolSectionSettingsWidget(this);
}

IToolSection *ToolSection::clone() const
{
    return new ToolSection(*this);
}

} // namespace Internal
} // namespace VcProjectManager
