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
#ifndef VCPROJECTMANAGER_INTERNAL_IDEPLOYMENTTOOLS_H
#define VCPROJECTMANAGER_INTERNAL_IDEPLOYMENTTOOLS_H

#include <QtGlobal>

QT_BEGIN_NAMESPACE
class QDomElement;
class QDomDocument;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class IDeploymentTool;

class IDeploymentTools
{
public:
    virtual ~IDeploymentTools() {}

    /*!
     * \brief Adds tool to a container.
     */
    virtual void addTool(IDeploymentTool *tool) = 0;

    /*!
     * \brief Removes tool from a container.
     */
    virtual void removeTool(IDeploymentTool *tool) = 0;

    /*!
     * \return a tool at an \a index.
     */
    virtual IDeploymentTool *tool(int index) const = 0;

    /*!
     * \return count of tools stored in a container.
     */
    virtual int toolCount() const = 0;

    /*!
     * \brief Implement this in order to append tools stored in this conatainer to an
     * XML node \a domElement. Use \a domDocument to create new XML nodes.
     */
    virtual void appendToXMLNode(QDomElement &domElement, QDomDocument &domDocument) const = 0;
};

} // Internal
} // VcProjectManager

#endif // VCPROJECTMANAGER_IDEPLOYMENTTOOLS_H
