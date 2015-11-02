#ifndef VISUALSTUDIOPROJECT_INTERNAL_VISUALSTUDIOSOLUTIONPARSER_H
#define VISUALSTUDIOPROJECT_INTERNAL_VISUALSTUDIOSOLUTIONPARSER_H

#include "projectreference.h"
#include "globals.h"

#include <QString>

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
};

} // namespace Internal
} // namespace VisualStudioProjectNS

#endif // VISUALSTUDIOPROJECT_INTERNAL_VISUALSTUDIOSOLUTIONPARSER_H
