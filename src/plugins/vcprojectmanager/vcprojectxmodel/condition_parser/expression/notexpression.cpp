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
#include "notexpression.h"
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

NotExpression::NotExpression(Expression *expr)
    : Expression(NOT),
      m_expression(expr)
{
}

NotExpression::NotExpression(const NotExpression &other)
    : Expression(other)
{
}

NotExpression::NotExpression(NotExpression &&other)
    : NotExpression()
{
    swap(*this, other);
}

NotExpression &NotExpression::operator=(NotExpression other)
{
    swap(*this, other);
    return *this;
}

NotExpression::~NotExpression()
{
}

QVariant NotExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    bool val = false;
    if (m_expression)
        val = m_expression->evaluate(evalArgs).toBool();
    return !val;
}

QString NotExpression::toString() const
{
    QString str;
    if (m_expression)
        str = QLatin1String("!") + m_expression->toString();
    return str;
}

Expression *NotExpression::clone() const
{
    return new NotExpression(*this);
}

Expression *NotExpression::expression() const
{
    return m_expression;
}

void NotExpression::swap(NotExpression &first, NotExpression &second)
{
    Expression::swap(first, second);
    std::swap(first.m_expression, second.m_expression);
}

} // namespace Internal
} // namespace VcProjectManager
