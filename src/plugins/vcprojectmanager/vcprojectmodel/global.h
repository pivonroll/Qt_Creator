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
#ifndef VCPROJECTMANAGER_INTERNAL_GLOBAL_H
#define VCPROJECTMANAGER_INTERNAL_GLOBAL_H

#include <visualstudiointerfaces/iglobal.h>

#include <QString>

QT_BEGIN_NAMESPACE
class QDomElement;
QT_END_NAMESPACE

namespace VcProjectManager {
namespace Internal {

class Global : public IGlobal
{
public:
    Global();
    Global(const Global &global);
    Global &operator=(const Global &global);
    ~Global();

    void processNode(const QDomNode &node);
    VcNodeWidget *createSettingsWidget();
    QDomNode toXMLDomNode(QDomDocument &domXMLDocument) const;

    QString displayName() const;
    void setDisplayName(const QString &name);
    QString value() const;
    void setValue(const QString &value);

    IGlobal *clone() const;

private:
    void processNodeAttributes(const QDomElement &element);

    QString m_name; // required
    QString m_value; // required
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_GLOBAL_H
