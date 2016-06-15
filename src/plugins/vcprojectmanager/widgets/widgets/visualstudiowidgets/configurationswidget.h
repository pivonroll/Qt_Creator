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
#ifndef VCPROJECTMANAGER_INTERNAL_CONFIGURATIONS_WIDGET_H
#define VCPROJECTMANAGER_INTERNAL_CONFIGURATIONS_WIDGET_H

#include <QWidget>

namespace VcProjectManager {
namespace Internal {

namespace Ui {
class ConfigurationsWidget;
}

class VcNodeWidget;

class ConfigurationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigurationsWidget(QWidget *parent = 0);
    ~ConfigurationsWidget();

    void addConfiguration(const QString &configName, QWidget *configWidget);
    VcNodeWidget *configWidget(const QString &configName);
    void removeConfiguration(const QString &configNameWithPlatform);
    void renameConfiguration(const QString &newconfigNameWithPlatform, const QString &oldConfigNameWithPlatform);
    QList<VcNodeWidget *> configWidgets();

private slots:
    void onAddNewConfig();
    void onRenameConfig();
    void onRemoveConfig();

signals:
    void addNewConfigSignal(QString configName, QString cloneFrom);
    void removeConfigSignal(QString configName);
    void renameConfigSignal(QString newName, QString oldName);

private:
    int indexOfConfig(const QString &configNameWithPlatform);

    Ui::ConfigurationsWidget *ui;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_CONFIGURATIONS_WIDGET_H
