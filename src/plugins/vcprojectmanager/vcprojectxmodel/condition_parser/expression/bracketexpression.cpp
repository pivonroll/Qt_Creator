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
#include "bracketexpression.h"
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

BracketExpression::BracketExpression(Expression *expr)
    : Expression(BRACKET),
      m_expr(expr)
{
}

BracketExpression::BracketExpression(const BracketExpression &other)
    : Expression(other)
{
    m_expr = other.m_expr->clone();
}

BracketExpression::BracketExpression(BracketExpression &&other)
    : BracketExpression()
{
    swap(*this, other);
}

BracketExpression &BracketExpression::operator=(BracketExpression other)
{
    swap(*this, other);
    return *this;
}

BracketExpression::~BracketExpression()
{
    delete m_expr;
}

QVariant BracketExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    if (m_expr)
        return m_expr->evaluate(evalArgs);
    return QVariant();
}

QString BracketExpression::toString() const
{
    QString str;
    if (m_expr)
        str =  QLatin1String("(") + m_expr->toString() + QLatin1String(")");
    return str;
}

Expression *BracketExpression::clone() const
{
    return new BracketExpression(*this);
}

Expression *BracketExpression::expression() const
{
    return m_expr;
}

void BracketExpression::swap(BracketExpression &first, BracketExpression &second)
{
    Expression::swap(first, second);
    std::swap(first.m_expr, second.m_expr);
}

} // namespace Internal
} // namespace VcProjectManager
