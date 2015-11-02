#include "visualstudiosolutionfile.h"

#include "vcprojectmanagerconstants.h"
#include "visualstudiosolution/visualstudiosolutionparser/visualstudiosolutionparser.h"
#include "vcprojectxmodel/vcxproj/vcprojectdocumentx.h"
#include "vcprojectmodel/vcprojectdocument.h"
#include "utils.h"

#include <utils/fileutils.h>
#include <utils/mimetypes/mimedatabase.h>

#include <QFileInfo>

namespace VcProjectManager {
namespace Internal {

VisualStudioSolutionFile::VisualStudioSolutionFile(const QString &filePath)
{
    m_visualStdioParser = new VisualStudioProjectNS::Internal::VisualStudioSolutionParser(filePath);
    m_visualStdioParser->parse();

    setFilePath(Utils::FileName::fromString(filePath));

    Utils::MimeDatabase mdb;

    QString canonicalSolutionDirectoryPath = QFileInfo(filePath).absolutePath();
    foreach (const VisualStudioProjectNS::Internal::ProjectReference &projectReference, m_visualStdioParser->m_projectReferences) {
        QString fullProjectPath = canonicalSolutionDirectoryPath + QLatin1Char('/') + projectReference.m_relativeProjectPath;

        Utils::MimeType projectMimeType = mdb.mimeTypeForFile(fullProjectPath);
        IVisualStudioProject *newProject = nullptr;

        if (projectMimeType.matchesName(QLatin1String(Constants::VCPROJ_MIMETYPE)))
            newProject = new VcProjectDocumentX(fullProjectPath);

        else if (projectMimeType.matchesName(QLatin1String(Constants::VC_X_PROJ_MIMETYPE))) {
            VcDocConstants::DocumentVersion docVersion = VisualStudioUtils::getProjectVersion(fullProjectPath);
            newProject = new VcProjectDocument(fullProjectPath, docVersion);
        }

        m_visualStudioProjects.append(newProject);
    }
}

VisualStudioSolutionFile::~VisualStudioSolutionFile()
{
}

bool VisualStudioSolutionFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    Q_UNUSED(errorString)
    Q_UNUSED(fileName)
    Q_UNUSED(autoSave)
    return false;
}

QString VisualStudioSolutionFile::defaultPath() const
{
    return QString();
}

QString VisualStudioSolutionFile::suggestedFileName() const
{
    return QString();
}

bool VisualStudioSolutionFile::isModified() const
{
    return false;
}

bool VisualStudioSolutionFile::isSaveAsAllowed() const
{
    return false;
}

bool VisualStudioSolutionFile::reload(QString *errorString, Core::IDocument::ReloadFlag flag, Core::IDocument::ChangeType type)
{
    Q_UNUSED(errorString)
    Q_UNUSED(flag)
    Q_UNUSED(type)
    return false;
}

VisualStudioSolutionNode *VisualStudioSolutionFile::createSolutionNode() const
{
    return nullptr;
}

} // namespace Internal
} // namespace VcProjectManager

