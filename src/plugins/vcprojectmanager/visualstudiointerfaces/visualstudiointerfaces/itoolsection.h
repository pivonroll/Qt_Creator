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
#ifndef VCPROJECTMANAGER_INTERNAL__I_TOOL_SECTION_H
#define VCPROJECTMANAGER_INTERNAL__I_TOOL_SECTION_H

namespace VcProjectManager {
namespace Internal {

class ToolSectionDescription;
class VcNodeWidget;
class IToolAttributeContainer;

class IToolSection
{
public:
    virtual ~IToolSection() {}

    /*!
     * \return a attribute container used to store attributes for this tool section.
     * See \class IAttributeContainer for more detail.
     */
    virtual IToolAttributeContainer *attributeContainer() const = 0;

    /*!
     * \return a tool section description associated with this tool section.
     * See \class IToolSectionDescription for more information.
     */
    virtual const ToolSectionDescription *sectionDescription() const = 0;

    /*!
     * \brief Creates a settings widget for this tool section.
     * User must handle a destruction of the widget himself/herself.
     * \return a newly created instance of the widget.
     */
    virtual VcNodeWidget *createSettingsWidget() = 0;

    /*!
     * \return a clone of this object.
     */
    virtual IToolSection *clone() const = 0;
};

} // Internal
} // VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL__I_TOOL_SECTION_H
