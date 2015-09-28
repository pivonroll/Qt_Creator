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
#include "numericbinaryexpression.h"
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

NumericBinaryExpression::NumericBinaryExpression(Expression *leftOp, OperationType opType, Expression *rigthOp)
    : BinaryExpression(leftOp, rigthOp),
      m_opType(opType)
{
}

Expression::ExpressionType NumericBinaryExpression::type() const
{
    return NUMERIC_BINARY;
}

QVariant NumericBinaryExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    int left = 0;
    int right = 0;
    if (m_leftOperand)
        left = m_leftOperand->evaluate(evalArgs).toInt();
    if (m_rightOperand)
        right = m_rightOperand->evaluate(evalArgs).toInt();

    switch (m_opType) {
    case OP_ADD:
        return QVariant(left + right);
    case OP_MINUS:
        return QVariant(left - right);
    case OP_MULTIPLY:
        return QVariant(left * right);
    case OP_DIVIDE: {
        if (right)
            return QVariant(left / right);
        else
            return QVariant();
    }
    }

    // should never be executed
    return QVariant();
}

QString NumericBinaryExpression::toString() const
{
    QString result;
    if (m_leftOperand && m_rightOperand) {
        QString oper;

        switch (m_opType) {
        case OP_ADD:
            oper = QLatin1String(" + ");
            break;
        case OP_MINUS:
            oper = QLatin1String(" - ");
            break;
        case OP_MULTIPLY:\
            oper = QLatin1String(" * ");
            break;
        case OP_DIVIDE:
            oper = QLatin1String(" / ");
            break;
        }

        result = m_leftOperand->toString() + oper + m_rightOperand->toString();
    }

    return result;
}

} // namespace Internal
} // namespace VcProjectManager
