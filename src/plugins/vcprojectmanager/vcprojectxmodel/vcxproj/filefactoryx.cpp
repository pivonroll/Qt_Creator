#include "filefactoryx.h"

#include "filex.h"
#include "filecontainerx.h"

#include "private/item.h"
#include "private/itemmetadata.h"
#include "vcprojectdocumentx.h"
#include "vcprojx_constants.h"

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

FileFactoryX::FileFactoryX(VcProjectDocumentX *parentProjectDocument)
    : m_parentProjectDocument(parentProjectDocument)
{
}

IFile *FileFactoryX::createFile(const QString &relativePath, ProjectExplorer::FileType fileType) const
{
    FileX *file = new FileX;
    file->setRelativePath(relativePath);
    file->m_parentProjectDocument = m_parentProjectDocument;
    file->m_project = m_parentProjectDocument->m_project;
    file->m_filters = m_parentProjectDocument->m_filters;
    file->m_item = new Item;
    file->m_filterItem = new Item;

    switch (fileType) {
    case ProjectExplorer::UnknownFileType:
        file->m_item->setName(QLatin1String(FILE_ITEM_NONE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_NONE));
        break;
    case ProjectExplorer::HeaderType:
        file->m_item->setName(QLatin1String(FILE_ITEM_CL_INCLUDE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_CL_INCLUDE));
        break;
    case ProjectExplorer::SourceType:
        file->m_item->setName(QLatin1String(FILE_ITEM_CL_COMPILE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_CL_COMPILE));
        break;
    case ProjectExplorer::FormType:
        break;
    case ProjectExplorer::ResourceType:
        file->m_item->setName(QLatin1String(FILE_ITEM_RESOURCE_COMPILE));
        file->m_filterItem->setName(QLatin1String(FILE_ITEM_RESOURCE_COMPILE));
        break;
    case ProjectExplorer::QMLType:
        break;
    case ProjectExplorer::ProjectFileType:
        break;
    case ProjectExplorer::FileTypeSize:
        break;
    default:
        break;
    }

    file->m_item->setInclude(relativePath);
    file->m_filterItem->setInclude(relativePath);

    return file;
}

IFileContainer *FileFactoryX::createFileContainer(const QString &containerPath, const QString &containerType) const
{
    FileContainerX *fileContainer = new FileContainerX;
    fileContainer->m_project = m_parentProjectDocument->m_project;
    fileContainer->m_filters = m_parentProjectDocument->m_filters;
    fileContainer->m_filterItem = new Item;
    fileContainer->m_filterItem->setName(containerType);
    fileContainer->m_filterItem->setInclude(containerPath);

    ItemMetaData *itemMetaData = new ItemMetaData;
    itemMetaData->setName(QLatin1String(UNIQUE_IDENTIFIER));
    // TODO(Radovan): add unique identifier id value
    fileContainer->m_filterItem->addItemMetaData(itemMetaData);

    return fileContainer;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

