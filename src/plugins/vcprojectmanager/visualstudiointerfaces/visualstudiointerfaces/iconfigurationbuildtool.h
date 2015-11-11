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
#ifndef VCPROJECTMANAGER_INTERNAL_ICONFIGURATIONBUILDTOOL_H
#define VCPROJECTMANAGER_INTERNAL_ICONFIGURATIONBUILDTOOL_H

#include "ivcprojectnodemodel.h"

namespace VcProjectManager {
namespace Internal {

class ISectionContainer;
class ToolDescription;

class IConfigurationBuildTool : public IVcProjectXMLNode
{
public:

    /*!
     * \return a tool section container.
     * For more details see \class ISectionContainer.
     *
     */
    virtual ISectionContainer *sectionContainer() const = 0;

    /*!
     * \return a description of a tool.
     * For more details see \class IToolDescription.
     */
    virtual const ToolDescription *toolDescription() const = 0;

    /*!
     * \return a clone of this instance.
     */
    virtual IConfigurationBuildTool *clone() const = 0;

    /*!
     * \return \b true of all attributes in this tool are set to default,
     * Otherwise it returns false.
     */
    virtual bool allAttributesAreDefault() const = 0;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_ICONFIGURATIONBUILDTOOL_H
