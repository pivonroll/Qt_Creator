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
#include "logicalbinaryexpression.h"
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

LogicalBinaryExpression::LogicalBinaryExpression(Expression *left, OperationType opType, Expression *right)
    : BinaryExpression(left, right),
      m_opType(opType)
{
}

Expression::ExpressionType LogicalBinaryExpression::type() const
{
    return LOGICAL_BINARY;
}

QVariant LogicalBinaryExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    bool left = false;
    bool right = false;

    if (m_leftOperand)
        left = m_leftOperand->evaluate(evalArgs).toBool();
    if (m_rightOperand)
        right = m_rightOperand->evaluate(evalArgs).toBool();

    if (m_opType == OP_AND)
        return left && right;
    return left || right;
}

QString LogicalBinaryExpression::toString() const
{
    if (m_leftOperand && m_rightOperand) {
        if (m_opType == OP_AND)
            return m_leftOperand->toString() + QLatin1String(" And ") + m_rightOperand->toString();
        else
            return m_leftOperand->toString() + QLatin1String(" Or ") + m_rightOperand->toString();
    }
    return QString();
}

} // namespace Internal
} // namespace VcProjectManager
