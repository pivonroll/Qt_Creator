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
#include "binaryexpression.h"
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

BinaryExpression::BinaryExpression(ExpressionType expressionType, Expression *leftOp, Expression *rightOp)
    : Expression(expressionType),
      m_leftOperand(leftOp),
      m_rightOperand(rightOp)
{
}

BinaryExpression::BinaryExpression(const BinaryExpression &other)
    : Expression(other)
{
    m_leftOperand = other.m_leftOperand->clone();
    m_rightOperand = other.m_rightOperand->clone();
}

BinaryExpression::BinaryExpression(BinaryExpression &&other)
    : BinaryExpression()
{
    swap(*this, other);
}

BinaryExpression &BinaryExpression::operator=(BinaryExpression other)
{
    swap(*this, other);
    return *this;
}

BinaryExpression::~BinaryExpression()
{
    delete m_leftOperand;
    delete m_rightOperand;
}

Expression *BinaryExpression::leftOperand() const
{
    return m_leftOperand;
}

Expression *BinaryExpression::rightOperand() const
{
    return m_rightOperand;
}

Expression *BinaryExpression::clone() const
{
    return new BinaryExpression(*this);
}

QVariant BinaryExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    Q_UNUSED(evalArgs)
    return QVariant();
}

QString BinaryExpression::toString() const
{
    return QString();
}

BinaryExpression::BinaryExpression()
    : m_leftOperand(nullptr),
      m_rightOperand(nullptr)
{
}

void BinaryExpression::swap(BinaryExpression &first, BinaryExpression &second)
{
    Expression::swap(first, second);
    std::swap(first.m_leftOperand, second.m_leftOperand);
    std::swap(first.m_rightOperand, second.m_rightOperand);
}

} // namespace Internal
} // namespace VcProjectManager
