#ifndef VISUALSTUDIOPROJECT_INTERNAL_PROJECTREFERENCE_H
#define VISUALSTUDIOPROJECT_INTERNAL_PROJECTREFERENCE_H

#include <QString>

namespace VisualStudioProjectNS {
namespace Internal {

struct ProjectReference
{
    QString m_id;
    QString m_projectName;
    QString m_relativeProjectPath;
    QString m_referenceId;
};

} // namespace Internal
} // namespace VisualStudioProjectNS

#endif // VISUALSTUDIOPROJECT_INTERNAL_PROJECTREFERENCE_H
