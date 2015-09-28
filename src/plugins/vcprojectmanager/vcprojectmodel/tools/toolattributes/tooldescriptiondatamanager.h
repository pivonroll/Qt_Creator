/**************************************************************************
**
** Copyright (c) 2014 Bojan Petrovic
** Copyright (c) 2014 Radovan Zivkovic
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
#ifndef VCPROJECTMANAGER_INTERNAL_TOOLDESCRIPTIONDATAMANAGER_H
#define VCPROJECTMANAGER_INTERNAL_TOOLDESCRIPTIONDATAMANAGER_H

#include <QList>
#include <QObject>
#include <QString>

QT_BEGIN_NAMESPACE
class QDomNode;
class QDomDocument;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class IToolDescription;
class ToolSectionDescription;

/*!
 * \brief The ToolInfo structure is used to store information about a tool
 * which are read from tool description XML file.
 * Every tool information consists of a tool's key and tool's display name.
 * Tool is uniquely identified by \a key.
 */
struct ToolInfo {
    QString m_key;
    QString m_displayName;

    bool operator==(const ToolInfo &toolInfo) {
        if (m_key != toolInfo.m_key)
            return false;

        return true;
    }

    bool isValid() {
        return !m_key.isEmpty();
    }
};

/*!
 * \brief The ToolDescriptionDataManager class is used to store tool information
 * read from tool description XML files.
 *
 */
class ToolDescriptionDataManager : public QObject
{
    friend class VcProjectManagerPlugin;
public:
    static ToolDescriptionDataManager *instance();

    /*!
     * \brief Reads tool description XML files stored in settings.
     */
    void readToolXMLFiles();
    ~ToolDescriptionDataManager();

    /*!
     * \return a count of tool descriptions available.
     */
    int toolDescriptionCount() const;

    /*!
     * \return a tool description at \a index.
     */
    IToolDescription *toolDescription(int index) const;

    /*!
     * \return a tool description with a key \a toolKey.
     */
    IToolDescription *toolDescription(const QString &toolKey) const;

    /*!
     * \brief readToolInfo
     * \param filePath
     * \param errorMsg
     * \param errorLine
     * \param errorColumn
     * \return a tool info read from a \a filePath.
     * If read fails errorMsg, errorLine and errorColumn will be set
     * to a proper error message and line and column number where error
     * originated in a file.
     */
    static ToolInfo readToolInfo(const QString &filePath, QString *errorMsg = 0, int *errorLine = 0, int *errorColumn = 0);

private:
    ToolDescriptionDataManager();

    /*!
     * \brief Reads an tool attribute data from file \a filePath.
     */
    void readAttributeDataFromFile(const QString &filePath);

    /*!
     * \brief Start the reading of an XML file.
     * It skips XML description at the begining of a file and
     * starts processing of a DOM node. See \a processDomNode(const QDomNode &node) for more information;
     */
    void processXMLDoc(const QDomDocument &xmlDoc);

    /*!
     * \brief Reads a tool description XML node.
     * After that it starts to process tool section description XML nodes.
     */
    void processDomNode(const QDomNode &node);

    /*!
     * \brief Reads tool section description XML nodes and
     * starts processing attribute description XML nodes.
     */
    void processToolSectionNode(IToolDescription *toolDescription, const QDomNode &domNode);

    /*!
     * \brief Reads tool attribute description XML node.
     */
    void processToolAttributeDescriptions(ToolSectionDescription *toolSectDesc, const QDomNode &domNode);

    /*!
     * \brief Reads tool description from tool description XML node.
     */
    IToolDescription *readToolDescription(const QDomNode &domNode);

    QList<IToolDescription *> m_toolDescriptions;
    static ToolDescriptionDataManager *m_instance;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_TOOLDESCRIPTIONDATAMANAGER_H
