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
#include "configurationx.h"

#include "conditionmanipulation.h"
#include "vcprojx_constants.h"
#include "../condition_parser/expression/stringliteral.h"
#include "../condition_parser/expression/evaluatearguments.h"

#include "private/choose.h"
#include "private/import.h"
#include "private/importgroup.h"
#include "private/item.h"
#include "private/itemdefinition.h"
#include "private/itemdefinitiongroup.h"
#include "private/itemgroup.h"
#include "private/itemmetadata.h"
#include "private/onerror.h"
#include "private/otherwise.h"
#include "private/output.h"
#include "private/project.h"
#include "private/projectextensions.h"
#include "private/property.h"
#include "private/propertygroup.h"
#include "private/target.h"
#include "private/task.h"
#include "private/taskbody.h"
#include "private/usingtask.h"
#include "private/when.h"

#include <QStringList>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

QString newConditionConfiguration(const QString &condition, const QString &oldName, const QString &newName)
{
    ConditionManipulation condManipulation(condition);

    Expression *expr = condManipulation.findVariableEqRightExpr(QLatin1String(CONFIGURATION_VARIABLE));

    if (expr && expr->type() == Expression::STRING_LITERAL) {
        StringLiteral *stringLit = static_cast<StringLiteral *>(expr);
        if (stringLit->stringExp() == oldName)
            stringLit->setStringExpr(newName);
    }

    return condManipulation.conditionToString();
}

void setConfiguraitionName(Otherwise *node, const QString &oldName, const QString &newName);
void setConfiguraitionName(When *node, const QString &oldName, const QString &newName);
void setConfiguraitionName(ItemMetaData *node, const QString &oldName, const QString &newName);
void setConfiguraitionName(PropertyGroup *node, const QString &oldName, const QString &newName);
void setConfiguraitionName(Target *node, const QString &oldName, const QString &newName);
void setConfiguraitionName(UsingTask *node, const QString &oldName, const QString &newName);
void setConfiguraitionName(Task *node, const QString &oldName, const QString &newName);

void setConfiguraitionName(Choose *node, const QString &oldName, const QString &newName)
{
    if (node) {
        setConfiguraitionName(node->otherwise(), oldName, newName);

        for (int i = 0; i < node->whenElementCount(); ++i)
            setConfiguraitionName(node->whenElement(i), oldName, newName);
    }
}

void setConfiguraitionName(Import *node, const QString &oldName, const QString &newName)
{
    if (node)
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));
}

void setConfiguraitionName(ImportGroup *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->importElementCount(); ++i)
            setConfiguraitionName(node->importElement(i), oldName, newName);
    }
}

void setConfiguraitionName(Item *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->itemMetaDataCount(); ++i)
            setConfiguraitionName(node->itemMetaData(i), oldName, newName);
    }
}

void setConfiguraitionName(ItemDefinition *node, const QString &oldName, const QString &newName)
{
    if (node) {
        for (int i = 0; i < node->metaDataCount(); ++i)
            setConfiguraitionName(node->metaData(i), oldName, newName);
    }
}

void setConfiguraitionName(ItemDefinitionGroup *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->itemDefinitionCount(); ++i)
            setConfiguraitionName(node->itemDefinition(i), oldName, newName);
    }
}

void setConfiguraitionName(ItemGroup *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->itemCount(); ++i)
            setConfiguraitionName(node->item(i), oldName, newName);
    }
}

void setConfiguraitionName(ItemMetaData *node, const QString &oldName, const QString &newName)
{
    if (node)
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));
}

void setConfiguraitionName(OnError *node, const QString &oldName, const QString &newName)
{
    if (node)
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));
}

void setConfiguraitionName(Otherwise *node, const QString &oldName, const QString &newName)
{
    if (node) {
        for (int i = 0; i < node->chooseElementCount(); ++i)
            setConfiguraitionName(node->chooseElement(i), oldName, newName);

        for (int i = 0; i < node->itemGroupElementCount(); ++i)
            setConfiguraitionName(node->itemGroupElement(i), oldName, newName);

        for (int i = 0; i < node->propertyGroupElementCount(); ++i)
            setConfiguraitionName(node->propertyGroupElement(i), oldName, newName);
    }
}

void setConfiguraitionName(Output *node, const QString &oldName, const QString &newName)
{
    if (node)
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));
}

void setConfiguraitionName(Project *node, const QString &oldName, const QString &newName)
{
    for (int i = 0; i < node->importElementCount(); ++i)
        setConfiguraitionName(node->importElement(i), oldName, newName);

    for (int i = 0; i < node->importGroupCount(); ++i)
        setConfiguraitionName(node->importGroup(i), oldName, newName);

    for (int i = 0; i < node->itemGroupCount(); ++i)
        setConfiguraitionName(node->itemGroup(i), oldName, newName);

    for (int i = 0; i < node->propertyGroupCount(); ++i)
        setConfiguraitionName(node->propertyGroup(i), oldName, newName);

    for (int i = 0; i < node->targetCount(); ++i)
        setConfiguraitionName(node->target(i), oldName, newName);

    for (int i = 0; i < node->usingTaskCount(); ++i)
        setConfiguraitionName(node->usingTask(i), oldName, newName);

    for (int i = 0; i < node->itemDefinitionGroupCount(); ++i)
        setConfiguraitionName(node->itemDefinitionGroup(i), oldName, newName);
}

void setConfiguraitionName(Property *node, const QString &oldName, const QString &newName)
{
    if (node)
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));
}

void setConfiguraitionName(PropertyGroup *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->propertyCount(); ++i)
            setConfiguraitionName(node->property(i), oldName, newName);
    }
}

void setConfiguraitionName(Target *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->taskCount(); ++i)
            setConfiguraitionName(node->task(i), oldName, newName);

        for (int i = 0; i < node->propertyGroupCount(); ++i)
            setConfiguraitionName(node->propertyGroup(i), oldName, newName);

        for (int i = 0; i < node->itemGroupCount(); ++i)
            setConfiguraitionName(node->itemGroup(i), oldName, newName);

        for (int i = 0; i < node->onErrorElementCount(); ++i)
            setConfiguraitionName(node->onErrorElement(i), oldName, newName);
    }
}

void setConfiguraitionName(Task *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->outputCount(); ++i)
            setConfiguraitionName(node->output(i), oldName, newName);
    }
}

void setConfiguraitionName(UsingTask *node, const QString &oldName, const QString &newName)
{
    if (node)
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));
}

void setConfiguraitionName(When *node, const QString &oldName, const QString &newName)
{
    if (node) {
        node->setCondition(newConditionConfiguration(node->condition(), oldName, newName));

        for (int i = 0; i < node->chooseElementCount(); ++i)
            setConfiguraitionName(node->chooseElement(i), oldName, newName);

        for (int i = 0; i < node->itemGroupCount(); ++i)
            setConfiguraitionName(node->itemGroup(i), oldName, newName);

        for (int i = 0; i < node->propertyGroupCount(); ++i)
            setConfiguraitionName(node->propertyGroup(i), oldName, newName);
    }
}


IAttributeContainer *ConfigurationX::attributeContainer() const
{
    return 0;
}

QString ConfigurationX::fullName() const
{
    return displayname() + QLatin1String(CONFIGURATION_PLATFORM_DELIMITER) + platform();
}

QString ConfigurationX::displayname() const
{
    for (int i = 0; i < m_item->itemMetaDataCount(); ++i)
    {
        ItemMetaData* metaData = m_item->itemMetaData(i);

        if (metaData && metaData->name() == QLatin1String(CONFIGURATION))
            return metaData->value();
    }

    return QString();
}

QString ConfigurationX::platform() const
{
    for (int i = 0; i < m_item->itemMetaDataCount(); ++i) {
        ItemMetaData* metaData = m_item->itemMetaData(i);

        if (metaData && metaData->name() == QLatin1String(PLATFORM))
            return metaData->value();
    }

    return QString();
}

void ConfigurationX::setFullName(const QString &fullName)
{
    QStringList list = fullName.split(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER));
    if (list.size() != 2)
        return;

    setDisplayName(list[0]);
    setPlatform(list[1]);
}

void ConfigurationX::setDisplayName(const QString &newName)
{
    if (newName.contains(QLatin1String(CONFIGURATION_PLATFORM_DELIMITER)))
        return;

    QString oldName = displayname();

    for (int i = 0; i < m_item->itemMetaDataCount(); ++i) {
        ItemMetaData* metaData = m_item->itemMetaData(i);

        if (metaData && metaData->name() == QLatin1String(CONFIGURATION))
            metaData->setValue(newName);
    }

    // flush config name change through out the project
    setConfiguraitionName(m_project, oldName, newName);
}

void ConfigurationX::setPlatform(const QString &platform)
{
    for (int i = 0; i < m_item->itemMetaDataCount(); ++i) {
        ItemMetaData* metaData = m_item->itemMetaData(i);

        if (metaData && metaData->name() == QLatin1String(PLATFORM))
            metaData->setValue(platform);
    }
}

ITools *ConfigurationX::tools() const
{
    return 0;
}

IConfiguration *ConfigurationX::clone() const
{
    return new ConfigurationX(*this);
}

VcNodeWidget *ConfigurationX::createSettingsWidget()
{
    return 0;
}

void ConfigurationX::processNode(const QDomNode &node)
{
    Q_UNUSED(node);
}

QDomNode ConfigurationX::toXMLDomNode(QDomDocument &domXMLDocument) const
{
    Q_UNUSED(domXMLDocument);

    return QDomNode();
}

ConfigurationX::ConfigurationX()
    : m_item(0),
      m_itemDefinitionGroup(0),
      m_project(0)
{
}

void ConfigurationX::findAssociatedTools()
{
    EvaluateArguments evalArgs;
    evalArgs.addArgument(QLatin1String(CONFIGURATION_VARIABLE), QVariant(displayname()));
    evalArgs.addArgument(QLatin1String(PLATFORM_VARIABLE), QVariant(platform()));

    for (int i = 0; i < m_project->itemDefinitionGroupCount(); ++i) {
        ItemDefinitionGroup *itemDefGrp = m_project->itemDefinitionGroup(i);

        if (itemDefGrp) {
            QString condition = itemDefGrp->condition();
            ConditionManipulation condManip(condition);

            if (condManip.evaluate(evalArgs)) {
                processToolDefGrp(itemDefGrp);
            }
        }
    }
}

void ConfigurationX::processToolDefGrp(ItemDefinitionGroup *itemDefGrp)
{
    if (itemDefGrp) {
        int count = itemDefGrp->itemDefinitionCount();

        for (int i = 0; i < count; ++i) {
            ItemDefinition *itemDef = itemDefGrp->itemDefinition(i);

            if (itemDef) {
                QString toolKey = itemDef->name();

                // TODO(Radovan):
                // check if the tool key belongs to the build configuration tools
                // if so, use ToolDescriptionDataManager to create the tool adn add that tool into ITools interface
            }
        }
    }
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
