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
#ifndef VCPROJECTMANAGER_INTERNAL_ATTRIBUTEDESCRIPTIONDATAITEM_H
#define VCPROJECTMANAGER_INTERNAL_ATTRIBUTEDESCRIPTIONDATAITEM_H

#include <QHash>
#include <QString>

namespace VcProjectManager {
namespace Internal {

class IToolAttribute;
class ToolAttributeOption;

class AttributeDescriptionDataItem
{
public:
    AttributeDescriptionDataItem(const QString &type,
                                 const QString &key,
                                 const QString &displayText,
                                 const QString &descriptionText,
                                 const QString &defaultVal,
                                 const QString &tag,
                                 const QString &version);
    ~AttributeDescriptionDataItem();

    // IAttributeDescriptionDataItem interface
    QString descriptionText() const;
    QString displayText() const;
    QString key() const;
    QString tag() const;
    QString version() const;
    ToolAttributeOption *firstOption() const;
    void setFirstOption(ToolAttributeOption *opt);
    QString defaultValue() const;
    QString optionalValue(const QString &key) const;
    void setOptionalValue(const QString &key, const QString &value);
    void removeOptionalValue(const QString &key);
    QString type() const;

private:
    QString m_key;
    QString m_displayText;
    QString m_descriptionText;
    QString m_defaultValue;
    QString m_tag;
    QString m_version;
    QString m_type;
    ToolAttributeOption *m_firstOption;
    QHash<QString, QString> m_optionalValues;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_ATTRIBUTEDESCRIPTIONDATAITEM_H
