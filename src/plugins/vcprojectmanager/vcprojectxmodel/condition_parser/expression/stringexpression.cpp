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
#include "stringexpression.h"

namespace VcProjectManager {
namespace Internal {

StringExpression::StringExpression(ExpressionType type, const QString &str)
    : Expression(type),
      m_str(str)
{
    m_type = type;
}

StringExpression::StringExpression(const StringExpression &other)
{
    m_str = other.m_str;
}

StringExpression::StringExpression(StringExpression &&other)
    : StringExpression()
{
    swap(*this, other);
}

StringExpression &StringExpression::operator=(StringExpression other)
{
    swap(*this, other);
    return *this;
}

StringExpression::~StringExpression()
{
}

QString StringExpression::stringExp() const
{
    return m_str;
}

void StringExpression::setStringExpr(const QString &str)
{
    m_str = str;
}

QVariant StringExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    Q_UNUSED(evalArgs)
    return QString();
}

QString StringExpression::toString() const
{
    return QString();
}

Expression *StringExpression::clone() const
{
    return new StringExpression(*this);
}

StringExpression::StringExpression()
    : Expression()
{
}

void StringExpression::swap(StringExpression &first, StringExpression &second)
{
    Expression::swap(first, second);
    std::swap(first.m_str, second.m_str);
}

} // namespace Internal
} // namespace VcProjectManager
