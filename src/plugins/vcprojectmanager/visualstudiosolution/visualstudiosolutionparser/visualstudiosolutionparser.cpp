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
#include "visualstudiosolutionparser.h"

#include <QTextStream>
#include <QFile>
#include <QStringList>

namespace VisualStudioProjectNS {
namespace Internal {

VisualStudioSolutionParser::VisualStudioSolutionParser(const QString &filePath)
    : m_filePath(filePath)
{
    parse();
}

void VisualStudioSolutionParser::parse()
{
    m_globals.m_sections.clear();
    m_projectReferences.clear();

    QFile file(m_filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        if (line.startsWith(QLatin1String("Microsoft Visual Studio Solution File, Format Version"))) {
            m_formatVersion = line;
            m_formatVersion.remove(0, QLatin1String("Microsoft Visual Studio Solution File, Format Version").size());
        }
        else if (line.startsWith(QLatin1String("# Visual Studio"))) {
            m_versionTag = line;
            m_versionTag.remove(0, QLatin1String("# Visual Studio ").size());
        }
        else if (line == QLatin1String("Global"))
            parseGlobal(in);
        else if (line.startsWith(QLatin1String("Project")))
            parseProjectReferences(in, line);
    }
}

void VisualStudioSolutionParser::parseGlobal(QTextStream &in)
{
    QString line;

    while(!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line == QLatin1String("EndGlobal"))
            break;

        else if (line.startsWith(QLatin1String("GlobalSection"))) {
            GlobalSection globalSection;
            QStringList list = line.split(QLatin1Char('='));

            QString temp = list[0].trimmed();
            globalSection.m_name = temp.mid(temp.indexOf(QLatin1Char('(')),
                                            temp.indexOf(QLatin1Char(')')));
            globalSection.m_name.remove(0, 1);
            globalSection.m_name.chop(1);
            globalSection.m_type = list[1].trimmed();

            if (globalSection.m_name == QLatin1String("ProjectConfigurationPlatforms"))
                parseProjectConfigurationPlatforms(in, globalSection);
            else if (globalSection.m_name == QLatin1String("SolutionConfigurationPlatforms")
                            || globalSection.m_name == QLatin1String("SolutionProperties"))
                parseSolutionConfigurationPlatforms(in, globalSection);
            else if (globalSection.m_name == QLatin1String("NestedProjects"))
                parseNestedProjects(in, globalSection);

            m_globals.m_sections << globalSection;
        }
    }
}

void VisualStudioSolutionParser::parseProjectReferences(QTextStream &in, const QString &lineArg)
{
    QString line = lineArg;
    QStringList list = line.split(QLatin1Char('='));

    QString tempId = list[0].mid(list[0].indexOf(QLatin1String("(\"")),
            list[0].indexOf(QLatin1String("\")")));
    tempId.remove(0, 3);
    tempId.chop(4);

    QStringList list2 = list[1].split(QLatin1Char(','));

    QString temp1 = list2[0].trimmed();
    temp1.remove(0, 1);
    temp1.chop(1);

    QString temp2 = list2[1].trimmed();
    temp2.remove(0, 1);
    temp2.chop(1);
    temp2.replace(QLatin1Char('\\'), QLatin1Char('/'));

    QString temp3 = list2[2].trimmed();
    temp3.remove(0, 2);
    temp3.chop(2);

    if (temp2.endsWith(QLatin1String(".vcxproj"))) {
        ProjectReference projectReference;
        projectReference.m_id = tempId;
        projectReference.m_projectName = temp1;
        projectReference.m_relativeProjectPath = temp2;
        projectReference.m_referenceId = temp3;
        m_projectReferences << projectReference;
    }
    else {
        FolderReference folderReference;
        folderReference.m_id = tempId;
        folderReference.m_projectName = temp1;
        folderReference.m_displayName = temp2;
        folderReference.m_referenceId = temp3;
        m_folderReferences << folderReference;
    }

    while(!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line == QLatin1String("EndProject"))
            break;
    }
}

void VisualStudioSolutionParser::parseProjectConfigurationPlatforms(QTextStream &in, GlobalSection &globalSection)
{
    QString line;

    while(!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line == QLatin1String("EndGlobalSection"))
            break;

        QStringList list = line.split(QLatin1Char('='));
        QString temp = list[0].trimmed();
        QStringList list2 = temp.split(QLatin1Char('.'));

        if (list2[2] == QLatin1String("ActiveCfg")) {
            ProjectConfigurationSection projConfig;
            projConfig.m_projectReferenceId = list2[0];
            projConfig.m_projectReferenceId.remove(0, 1);
            projConfig.m_projectReferenceId.chop(1);

            projConfig.m_projectConfigurationName = list2[1];
            projConfig.m_targetSolutionConfiguration = list[1].trimmed();
            globalSection.m_projConfigSections << projConfig;
        } else if (list2[2] == QLatin1String("Build")) {
            ProjectConfigurationBuild projBuild;
            projBuild.m_projectReferenceId = list2[0];
            projBuild.m_projectReferenceId.remove(0, 1);
            projBuild.m_projectReferenceId.chop(1);

            projBuild.m_projectConfigurationName = list2[1];
            projBuild.m_targetSolutionConfiguration = list[1].trimmed();
            globalSection.m_projBuildSections << projBuild;
        }
    }
}

void VisualStudioSolutionParser::parseSolutionConfigurationPlatforms(QTextStream &in, GlobalSection &globalSection)
{
    QString line;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line == QLatin1String("EndGlobalSection"))
            break;

        SolutionConfiguration solutionConfig;
        QStringList list = line.split(QLatin1Char('='));

        solutionConfig.m_key = list[0].trimmed();
        solutionConfig.m_value = list[1].trimmed();

        globalSection.m_solutionConfigurations << solutionConfig;
    }
}

void VisualStudioSolutionParser::parseNestedProjects(QTextStream &in, GlobalSection &globalSection)
{
    QString line;

    while(!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line == QLatin1String("EndGlobalSection"))
            break;

        NestedProject nestedProject;
        QStringList list = line.split(QLatin1Char('='));

        nestedProject.m_idKey = list[0].trimmed();
        nestedProject.m_idKey.remove(0, 1);
        nestedProject.m_idKey.chop(1);
        nestedProject.m_targetId = list[1].trimmed();
        nestedProject.m_targetId.remove(0, 1);
        nestedProject.m_targetId.chop(1);

        globalSection.m_nestedProjects << nestedProject;
    }
}

} // namespace Internal
} // namespace VisualStudioProjectNS

