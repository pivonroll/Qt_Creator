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
#ifndef VCPROJECTMANAGER_INTERNAL_VC_PROJECT_KIT_INFORMATION_H
#define VCPROJECTMANAGER_INTERNAL_VC_PROJECT_KIT_INFORMATION_H

#include <projectexplorer/kitinformation.h>

namespace VcProjectManager {
namespace Internal {

class MsBuildInformation;

class VcProjectKitInformation : public ProjectExplorer::KitInformation
{
    Q_OBJECT

public:
    VcProjectKitInformation();
    ~VcProjectKitInformation();

    Core::Id dataId() const;
    unsigned int priority() const; // the higher the closer to the top.
    QVariant defaultValue(ProjectExplorer::Kit *) const;

    // called to find issues with the kit
    QList<ProjectExplorer::Task> validate(const ProjectExplorer::Kit *k) const;
    // called to fix issues with this kitinformation. Does not modify the rest of the kit.
    void fix(ProjectExplorer::Kit *k);
    // called on initial setup of a kit.
    void setup(ProjectExplorer::Kit *k);

    ItemList toUserOutput(const ProjectExplorer::Kit *) const;
    ProjectExplorer::KitConfigWidget *createConfigWidget(ProjectExplorer::Kit *k) const;

    // KitInformation interface
    QVariant defaultValue(const ProjectExplorer::Kit *k) const;

    static MsBuildInformation *msBuildInfo(const ProjectExplorer::Kit *k);

    static void setMsBuild(ProjectExplorer::Kit *k, MsBuildInformation *msBuildId);

private slots:
    void onMSBuildAdded(Core::Id msBuildId);
    void onMSBuildRemoved(Core::Id msBuildId);
    void onMSBuildReplaced(Core::Id oldMsBuildId, Core::Id newMsBuildId);
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VC_PROJECT_KIT_INFORMATION_H
