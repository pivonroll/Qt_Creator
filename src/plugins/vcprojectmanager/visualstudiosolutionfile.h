#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONFILE_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONFILE_H

#include <coreplugin/idocument.h>

namespace VisualStudioProjectNS {
namespace Internal {
    class VisualStudioSolutionParser;
} // namespace Internal
} // namespace VisualStudioProject

namespace VcProjectManager {
namespace Internal {

class VisualStudioSolutionNode;
class IVisualStudioProject;

class VisualStudioSolutionFile : public Core::IDocument
{
    friend class VisualStudioSolutionNode;
public:
    VisualStudioSolutionFile(const QString &filePath);
    ~VisualStudioSolutionFile();

    // IDocument interface
    bool save(QString *errorString, const QString &fileName, bool autoSave);
    QString defaultPath() const;
    QString suggestedFileName() const;
    bool isModified() const;
    bool isSaveAsAllowed() const;
    bool reload(QString *errorString, ReloadFlag flag, ChangeType type);

    VisualStudioSolutionNode *createSolutionNode() const;

private:
    VisualStudioProjectNS::Internal::VisualStudioSolutionParser *m_visualStdioParser;
    QList<IVisualStudioProject *> m_visualStudioProjects;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOSOLUTIONFILE_H
