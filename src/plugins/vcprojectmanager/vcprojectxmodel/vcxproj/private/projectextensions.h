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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECTEXTENSIONS_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECTEXTENSIONS_H

#include <QString>
#include <QDomNode>

#include "ivisualstudionodex.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

class ProjectExtensions : public IVisualStudioNodeX
{
public:
    ProjectExtensions();
    ProjectExtensions(const ProjectExtensions &other);
    ProjectExtensions(ProjectExtensions &&other);
    ProjectExtensions& operator=(ProjectExtensions other);
    ~ProjectExtensions();

    // IVisualStudioProjectNode interface

    void processNode(const QDomNode &node);
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    QDomNode node() const;
    void setNode(const QDomNode &node);

private:
    static void swap(ProjectExtensions &first, ProjectExtensions &second);
    QDomNode m_node;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_PROJECTEXTENSIONS_H
