#ifndef VISUALSTUDIOPROJECT_INTERNAL_VISUALSTUDIOSOLUTIONPARSER_H
#define VISUALSTUDIOPROJECT_INTERNAL_VISUALSTUDIOSOLUTIONPARSER_H

#include "projectreference.h"
#include "globals.h"

#include <QString>

QT_BEGIN_NAMESPACE
class QTextStream;
QT_END_NAMESPACE

namespace VisualStudioProjectNS {
namespace Internal {

class VisualStudioSolutionParser
{
public:
    VisualStudioSolutionParser(const QString &filePath);

    void parse();

    QString m_filePath;

    QString m_formatVersion;
    QString m_versionTag;

    Globals m_globals;
    QList<ProjectReference> m_projectReferences;
    QList<FolderReference> m_folderReferences;

private:
    void parseGlobal(QTextStream &in);
    void parseProjectReferences(QTextStream &in, const QString &lineArg);
    void parseProjectConfigurationPlatforms(QTextStream &in, GlobalSection &globalSection);
    void parseSolutionConfigurationPlatforms(QTextStream &in, GlobalSection &globalSection);
    void parseNestedProjects(QTextStream &in, GlobalSection &globalSection);
};

} // namespace Internal
} // namespace VisualStudioProjectNS

#endif // VISUALSTUDIOPROJECT_INTERNAL_VISUALSTUDIOSOLUTIONPARSER_H
