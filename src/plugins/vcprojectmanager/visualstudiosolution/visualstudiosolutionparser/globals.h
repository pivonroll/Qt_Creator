#ifndef VISUALSTUDIOPROJECT_INTERNAL_GLOBALS_H
#define VISUALSTUDIOPROJECT_INTERNAL_GLOBALS_H

#include "globalsection.h"

namespace VisualStudioProjectNS {
namespace Internal {

struct Globals
{
    QList<GlobalSection> m_sections;
};

} // namespace Internal
} // namespace VisualStudioProjectNS

#endif // VISUALSTUDIOPROJECT_INTERNAL_GLOBALS_H
