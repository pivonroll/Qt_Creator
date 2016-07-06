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
#ifndef VCPROJECTMANAGER_INTERNAL_VC_PROJECT_DOCUMENTX_H
#define VCPROJECTMANAGER_INTERNAL_VC_PROJECT_DOCUMENTX_H

#include <QDomProcessingInstruction>

#include <visualstudiointerfaces/ivisualstudioproject.h>

namespace VcProjectManager {
namespace Internal {

namespace VisualStudioProjectX {
    class Project;
    class ConfigurationsX;
    class ProjectFactoriesX;
    class FileFactoryX;
    class FilesX;
}

class VcProjectDocumentX : public IVisualStudioProject
{
    friend class VisualStudioProjectX::FileFactoryX;
public:
    VcProjectDocumentX(const QString &filePath = QString());
    VcProjectDocumentX(const VcProjectDocumentX &other);
    VcProjectDocumentX(VcProjectDocumentX &&other);
    VcProjectDocumentX& operator=(VcProjectDocumentX other);
    ~VcProjectDocumentX();

    // IVisualStudioProject interface
    IConfigurations *configurations() const;
    IFiles *files() const;
    IGlobals *globals() const;
    IPlatforms *platforms() const;
    IReferences *referencess() const;
    IAttributeContainer *attributeContainer() const;
    IToolFiles *toolFiles() const;
    IPublishingData *publishingData() const;
    QString filePath() const;
    bool saveToFile(const QString &filePath) const;
    DocumentVersion documentVersion() const;

    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    IConfiguration *createDefaultBuildConfiguration(const QString &fullConfigName) const;
    IProjectFactories *projectFactories() const;
    IVisualStudioProject *clone() const;

private:
    static void swap(VcProjectDocumentX &first, VcProjectDocumentX &second);
    void processFilterDoc();

    QString m_filePath;
    VisualStudioProjectX::Project *m_project;
    VisualStudioProjectX::Project *m_filters;
    VisualStudioProjectX::ConfigurationsX *m_configurations;
    QDomProcessingInstruction m_projectProcInstructionNode;
    QDomProcessingInstruction m_filtersProcInstructionNode;
    VisualStudioProjectX::FilesX *m_files;
    VisualStudioProjectX::ProjectFactoriesX *m_projectFactories;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VC_PROJECT_DOCUMENTX_H
