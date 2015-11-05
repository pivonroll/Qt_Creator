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

struct NestedProject {
    // m_idKey is a child of m_targetId
    QString m_idKey;    // project or folder id
    QString m_targetId; // may be project ID or folder ID
};

struct GlobalSection
{
    QString m_name;
    QString m_type;
    QList<SolutionConfiguration> m_solutionConfigurations;
    QList<ProjectConfigurationSection> m_projConfigSections;
    QList<ProjectConfigurationBuild> m_projBuildSections;
    QList<NestedProject> m_nestedProjects;
};

} // namespace Internal
} // namespace VisualStudioProjectNS

#endif // VISUALSTUDIOPROJECT_INTERNAL_GLOBALSECTION_H
