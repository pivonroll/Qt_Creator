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
#ifndef VCPROJECTMANAGER_INTERNAL_TOOLSECTION_H
#define VCPROJECTMANAGER_INTERNAL_TOOLSECTION_H

#include <visualstudiointerfaces/itoolsection.h>

#include <QList>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class ToolSectionDescription;
class GeneralToolAttributeContainer;

class ToolSection : public IToolSection
{
public:
    ToolSection(const ToolSectionDescription *toolSectionDesc = nullptr);
    ToolSection(const ToolSection &toolSec);
    ToolSection(ToolSection &&toolSec);
    ToolSection& operator=(ToolSection toolSec);
    ~ToolSection();

    IToolAttributeContainer *attributeContainer() const;
    const ToolSectionDescription *sectionDescription() const;
    VcNodeWidget *createSettingsWidget();
    IToolSection *clone() const;

private:
    void swap(ToolSection &first, ToolSection &second);
    const ToolSectionDescription *m_toolDesc;
    GeneralToolAttributeContainer *m_attributeContainer;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_TOOLSECTION_H
