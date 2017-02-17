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
#include "conditionmanipulation.h"
#include "../condition_parser/parser/vcproj_condition_parser.h"
#include "../condition_parser/expression/expression.h"
#include "../condition_parser/expression/variablepipe.h"
#include "../condition_parser/expression/variable.h"
#include "../condition_parser/expression/stringcompare.h"
#include "../condition_parser/expression/logicalbinaryexpression.h"
#include "../condition_parser/expression/stringliteral.h"
#include "../condition_parser/expression/notexpression.h"
#include "../condition_parser/expression/bracketexpression.h"

#include <QVariant>

namespace VcProjectManager {
namespace Internal {
namespace VisualStudioProjectX {

ConditionManipulation::ConditionManipulation(const QString &conditionExpression)
    : m_conditionString(conditionExpression),
      m_condExpression(nullptr)
{
    if (m_conditionString.isEmpty())
        return;

    VcProjConditionParser parser(m_conditionString);

    if (parser.parse())
        m_condExpression = parser.condExpression();
}

ConditionManipulation::ConditionManipulation(const ConditionManipulation &other)
{
    m_condExpression = other.m_condExpression->clone();
    m_conditionString = other.m_conditionString;
}

ConditionManipulation::ConditionManipulation(ConditionManipulation &&other)
    : ConditionManipulation()
{
    swap(*this, other);
}

ConditionManipulation &ConditionManipulation::operator=(ConditionManipulation other)
{
    swap(*this, other);
    return *this;
}

ConditionManipulation::~ConditionManipulation()
{
    delete m_condExpression;
}

Expression *ConditionManipulation::findVariableEqRightExpr(const QString &variable) const
{
    return findVariableEqRightExpr(m_condExpression, variable);
}

QString ConditionManipulation::conditionToString() const
{
    if (m_condExpression)
        return m_condExpression->toString();
    return m_conditionString;
}

bool ConditionManipulation::evaluate(const EvaluateArguments &evalArgs) const
{
    if (!m_condExpression)
        return false;

    return m_condExpression->evaluate(evalArgs).toBool();
}

void ConditionManipulation::swap(ConditionManipulation &first, ConditionManipulation &second)
{
    std::swap(first.m_condExpression, second.m_condExpression);
    std::swap(first.m_conditionString, second.m_conditionString);
}

Expression *ConditionManipulation::findVariableEqRightExpr(Expression *startExp, const QString &variable) const
{
    if (!startExp)
        return nullptr;

    if (startExp->type() == Expression::STRING_COMPARE) {
        StringCompare *strCmpExpr = static_cast<StringCompare *>(startExp);
        return findStrCmpRightExpr(strCmpExpr, variable);
    }
    if (startExp->type() == Expression::NOT) {
        NotExpression *expr = static_cast<NotExpression *>(startExp);
        return findVariableEqRightExpr(expr->expression(), variable);
    }
    if (startExp->type() == Expression::BRACKET) {
        BracketExpression *expr = static_cast<BracketExpression *>(startExp);
        return findVariableEqRightExpr(expr->expression(), variable);
    }
    if (startExp->type() == Expression::LOGICAL_BINARY) {
        LogicalBinaryExpression *logBinExpr = static_cast<LogicalBinaryExpression *>(startExp);

        if (logBinExpr->leftOperand() && logBinExpr->leftOperand()->type() == Expression::VARIABLE)
            return logBinExpr->rightOperand();
        return findVariableEqRightExpr(logBinExpr->leftOperand(), variable);
    }

    return nullptr;
}

Expression *ConditionManipulation::findStrCmpRightExpr(StringCompare *strCmpExpr, const QString &variable) const
{
    Expression *leftOp = strCmpExpr->leftOperand();

    if (!leftOp)
        return nullptr;

    if (leftOp->type() == Expression::VARIABLE) {
        Variable *var = static_cast<Variable *>(leftOp);
        if (var->stringExp() == variable)
            return strCmpExpr->rightOperand();
    }

    if (leftOp->type() == Expression::VARIABLE_PIPE) {
        VariablePipe *varPipe = static_cast<VariablePipe *>(leftOp);
        int index = 0;

        if (findInVariablePipeExpression(varPipe, variable, index)) {
            Expression *rightOp = strCmpExpr->rightOperand();

            if (rightOp && rightOp->type() == Expression::VARIABLE_PIPE) {
                varPipe = static_cast<VariablePipe *>(rightOp);

                return getVariablePipeLiteral(varPipe, index);
            }
        }
    }

    return nullptr;
}

bool ConditionManipulation::findInVariablePipeExpression(VariablePipe *varPipeExpr, const QString &variable, int &index) const
{
    if (!varPipeExpr)
        return false;

    Expression* leftOp = varPipeExpr->leftOperand();
    if (leftOp && leftOp->type() == Expression::VARIABLE) {
        Variable *varExpr = static_cast<Variable *>(leftOp);

        if (varExpr->stringExp() == variable)
            return true;
    }

    index++;

    Expression *rightOp = varPipeExpr->rightOperand();
    if (rightOp && rightOp->type() == Expression::VARIABLE) {
        Variable *varExpr = static_cast<Variable *>(rightOp);

        if (varExpr->stringExp() == variable)
            return true;
    }

    if (rightOp->type() == Expression::VARIABLE_PIPE) {
        VariablePipe *varExpr = static_cast<VariablePipe *>(rightOp);
        return findInVariablePipeExpression(varExpr, variable, index);
    }

    return false;
}

StringLiteral *ConditionManipulation::getVariablePipeLiteral(VariablePipe *varPipeExpr, int index) const
{
    if (!varPipeExpr)
        return nullptr;

    if (!index) {
        Expression* leftOp = varPipeExpr->leftOperand();
        if (leftOp && leftOp->type() == Expression::STRING_LITERAL) {
            StringLiteral *stringLiteralExpr = static_cast<StringLiteral *>(leftOp);
            return stringLiteralExpr;
        } else
            return nullptr;
    }

    index--;

    Expression *rightOp = varPipeExpr->rightOperand();

    if (rightOp) {
        if (!index) {
            if (rightOp->type() == Expression::STRING_LITERAL) {
                StringLiteral *stringLiteralExpr = static_cast<StringLiteral *>(rightOp);
                return stringLiteralExpr;
            } else
                return nullptr;
        }

        else if (rightOp->type() == Expression::VARIABLE_PIPE) {
            VariablePipe *varExpr = static_cast<VariablePipe *>(rightOp);
            return getVariablePipeLiteral(varExpr, index);
        }
    }

    return nullptr;
}

} // namespace VisualStudioProjectX
} // namespace Internal
} // namespace VcProjectManager
