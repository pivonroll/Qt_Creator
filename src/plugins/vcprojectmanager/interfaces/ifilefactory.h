#ifndef VCPROJECTMANAGER_INTERNAL_IFILEFACTORY
#define VCPROJECTMANAGER_INTERNAL_IFILEFACTORY

#include <QtGlobal>
#include <projectexplorer/projectnodes.h>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class IFile;
class IFileContainer;

class IFileFactory
{
public:
    virtual ~IFileFactory() {}
    virtual IFile *createFile(const QString &relativePath, ProjectExplorer::FileType fileType) const = 0;
    virtual IFileContainer *createFileContainer(const QString &containerPath, const QString &containerType) const = 0;
};
}
}
#endif // VCPROJECTMANAGER_INTERNAL_IFILEFACTORY

