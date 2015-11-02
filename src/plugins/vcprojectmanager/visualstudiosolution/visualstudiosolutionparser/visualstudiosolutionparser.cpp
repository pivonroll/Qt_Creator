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

        else if (line == QLatin1String("Global")) {
            Globals globals;

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

                    if (globalSection.m_name == QLatin1String("ProjectConfigurationPlatforms")) {
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
                    } else if (globalSection.m_name == QLatin1String("SolutionConfigurationPlatforms")
                               || globalSection.m_name == QLatin1String("SolutionProperties")) {
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

                    globals.m_sections << globalSection;
                }
            }

            m_globals = globals;
        }

        else if (line.startsWith(QLatin1String("Project"))) {
            ProjectReference projectReference;
            QStringList list = line.split(QLatin1Char('='));
            projectReference.m_id = list[0].mid(list[0].indexOf(QLatin1String("(\"")),
                    list[0].indexOf(QLatin1String("\")")));

            projectReference.m_id.remove(0, 3);
            projectReference.m_id.chop(4);

            QStringList list2 = list[1].split(QLatin1Char(','));

            QString temp = list2[0].trimmed();
            temp.remove(0, 1);
            temp.chop(1);
            projectReference.m_projectName = temp;

            temp = list2[1].trimmed();
            temp.remove(0, 1);
            temp.chop(1);
            projectReference.m_relativeProjectPath = temp;

            temp = list2[2].trimmed();
            temp.remove(0, 2);
            temp.chop(2);
            projectReference.m_referenceId = temp;

            m_projectReferences << projectReference;
        }
    }
}

} // namespace Internal
} // namespace VisualStudioProjectNS

