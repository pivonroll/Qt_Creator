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
#ifndef VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONDITION_MANIPULATION_H
#define VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONDITION_MANIPULATION_H

#include <QString>

namespace VcProjectManager {
namespace Internal {

class Expression;
class VariablePipe;
class StringLiteral;
class StringCompare;
class EvaluateArguments;

namespace VisualStudioProjectX {

class ConditionManipulation
{
public:
    explicit ConditionManipulation(const QString &conditionExpression = QString());
    ConditionManipulation(const ConditionManipulation &other);
    ConditionManipulation(ConditionManipulation &&other);
    ConditionManipulation& operator=(ConditionManipulation other);
    ~ConditionManipulation();

    /*!
     * @brief findVariableEqRightExpr returns a pointer to a expression that matches variable but is on the right side of '==' expresion
     *
     * If you have an expression like this one: '($Configuration|$Platform)'=='(Debug|Win32)' and you want to fetch the expression
     * that is matched to the \b $Configuration which in this case is \b Debug then you need to use this function.
     */

    Expression* findVariableEqRightExpr(const QString &variable) const;
    QString conditionToString() const;
    bool evaluate(const EvaluateArguments &evalArgs) const;

private:
    static void swap(ConditionManipulation &first, ConditionManipulation &second);
    Expression* findVariableEqRightExpr(Expression *startExp, const QString &variable) const;
    Expression* findStrCmpRightExpr(StringCompare *strCmpExpr, const QString &variable) const;
    bool findInVariablePipeExpression(VariablePipe *varPipeExpr, const QString &variable, int &index) const;
    StringLiteral* getVariablePipeLiteral(VariablePipe *varPipeExpr, int index) const;

    QString m_conditionString;
    Expression *m_condExpression;
};

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_VISUALSTUDIOPROJECTX_CONDITION_MANIPULATION_H
