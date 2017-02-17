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
#ifndef VCPROJECTMANAGER_INTERNAL_FILE_H
#define VCPROJECTMANAGER_INTERNAL_FILE_H

#include <visualstudiointerfaces/ifile.h>

QT_BEGIN_NAMESPACE
class QDomElement;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class IVisualStudioProject;
class GeneralAttributeContainer;
class IToolSection;
class ConfigurationContainer;

class File : public IFile
{
    friend class FileFactory;

public:
    explicit File(IVisualStudioProject *parentProjectDoc = nullptr);
    File(const File &other);
    File(File &&other);
    File &operator=(File other);
    ~File();

    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    IConfigurationContainer *configurationContainer() const;
    IAttributeContainer *attributeContainer() const;

    QString displayName() const;
    QString relativePath() const;
    void setRelativePath(const QString &relativePath);
    IFile *clone() const;
    ProjectExplorer::FileType fileType() const;
    QString canonicalPath() const;
    IConfiguration *createDefaultBuildConfiguration(const QString &configName, const QString &platformName) const;

private:
    static void swap(File &first, File &second);
    void processFileConfiguration(const QDomNode &fileConfigNode);
    void processFile(const QDomNode &fileNode);
    void processNodeAttributes(const QDomElement &element);
    void copyAllNonDefaultToolAtributes(IConfiguration *fileConfig, IConfiguration *projConfig);
    void copyAllNonDefaultToolAtributes(IToolSection *fileSec, IToolSection *projSec);
    void leaveOnlyCppTool(IConfiguration *config);
    void copyProjectConfigs();

    QString m_relativePath; // required
    QList<File *> m_files;
    IVisualStudioProject *m_parentProjectDoc;
    ConfigurationContainer *m_configurationContainer;
    GeneralAttributeContainer *m_attributeContainer;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_FILE_H
