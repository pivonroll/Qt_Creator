#ifndef VISUALSTUDIOPROJECT_INTERNAL_GLOBALSECTION_H
#define VISUALSTUDIOPROJECT_INTERNAL_GLOBALSECTION_H

#include "solutionconfiguration.h"

#include <QList>

namespace VisualStudioProjectNS {
namespace Internal {

struct ProjectConfigurationSection {
    QString m_projectReferenceId;
    QString m_projectConfigurationName;
    QString m_targetSolutionConfiguration;
};

struct ProjectConfigurationBuild {
    QString m_projectReferenceId;
    QString m_projectConfigurationName;
    QString m_targetSolutionConfiguration;
    QString m_buildId;
};

struct GlobalSection
{
    QString m_name;
    QString m_type;
    QList<SolutionConfiguration> m_solutionConfigurations;
    QList<ProjectConfigurationSection> m_projConfigSections;
    QList<ProjectConfigurationBuild> m_projBuildSections;
};

} // namespace Internal
} // namespace VisualStudioProjectNS

#endif // VISUALSTUDIOPROJECT_INTERNAL_GLOBALSECTION_H
