/**************************************************************************
**
** Copyright (c) 2016 Bojan Petrovic
** Copyright (c) 2016 Radovan Zivkovic
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/
#include "vcdocumentmodel.h"
#include "vcprojectdocument.h"

#include <QDomDocument>
#include <QFile>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>

namespace VcProjectManager {
namespace Internal {

VcDocumentModel::VcDocumentModel(const QString &filePath, DocumentVersion version)
{
    m_vcProjectDocument = new VcProjectDocument(filePath, version);
}

VcDocumentModel::VcDocumentModel(const VcDocumentModel &other)
{
    m_document = other.m_document;
    m_vcProjectDocument = other.m_vcProjectDocument;
}

VcDocumentModel::VcDocumentModel(VcDocumentModel &&other)
    : VcDocumentModel()
{
    swap(*this, other);
}

VcDocumentModel &VcDocumentModel::operator=(VcDocumentModel other)
{
    swap(*this, other);
    return *this;
}

VcDocumentModel::~VcDocumentModel()
{
    delete m_vcProjectDocument;
}

IVisualStudioProject *VcDocumentModel::vcProjectDocument() const
{
    return m_vcProjectDocument;
}

bool VcDocumentModel::saveToFile(const QString &filePath) const
{
    if (m_vcProjectDocument)
        return m_vcProjectDocument->saveToFile(filePath);

    return false;
}

VcDocumentModel::VcDocumentModel()
    : m_document(nullptr),
      m_vcProjectDocument(nullptr)
{
}

void VcDocumentModel::swap(VcDocumentModel &first, VcDocumentModel &second)
{
    std::swap(first.m_document, second.m_document);
    std::swap(first.m_vcProjectDocument, second.m_vcProjectDocument);
}

} // namespace Internal
} // namespace VcProjectManager
