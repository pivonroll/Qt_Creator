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
#ifndef VCPROJECTMANAGER_INTERNAL_TOOLFILES_H
#define VCPROJECTMANAGER_INTERNAL_TOOLFILES_H

#include <visualstudiointerfaces/itoolfiles.h>

#include <QList>

namespace VcProjectManager {
namespace Internal {

class ToolFiles : public IToolFiles
{
public:
    ToolFiles();
    ToolFiles(const ToolFiles &other);
    ToolFiles(ToolFiles &&other);
    ToolFiles &operator=(ToolFiles other);
    ~ToolFiles();

    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    void addToolFile(IToolFile *toolFile);
    int toolFileCount() const;
    IToolFile *toolFile(int index) const;
    void removeToolFile(IToolFile *toolFile);

private:
    void swap(ToolFiles &first, ToolFiles &second);
    void processToolFiles(const QDomNode &toolFile);

    QList<IToolFile *> m_toolFiles;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_TOOLFILES_H
