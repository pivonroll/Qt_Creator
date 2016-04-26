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
#include "hastrailingslashexpression.h"
#include <QString>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

HasTrailingSlashExpression::HasTrailingSlashExpression(Expression *expr)
    : Expression(HAS_TRAILING_SLASH),
      m_expr(expr)
{
}

HasTrailingSlashExpression::HasTrailingSlashExpression(const HasTrailingSlashExpression &other)
{
    m_expr = other.m_expr->clone();
}

HasTrailingSlashExpression::HasTrailingSlashExpression(HasTrailingSlashExpression &&other)
{
    swap(*this, other);
}

HasTrailingSlashExpression &HasTrailingSlashExpression::operator=(HasTrailingSlashExpression other)
{
    swap(*this, other);
    return *this;
}

HasTrailingSlashExpression::~HasTrailingSlashExpression()
{
    delete m_expr;
}

QVariant HasTrailingSlashExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    QString str;
    if (m_expr)
        str = m_expr->evaluate(evalArgs).toString();

    return QVariant(str.endsWith(QLatin1String("\\")) || str.endsWith(QLatin1Char('/')));
}

QString HasTrailingSlashExpression::toString() const
{
    QString str;
    if (m_expr)
        str = QLatin1String("HasTrailingSlash(") + m_expr->toString() + QLatin1String(")");
    return str;
}

Expression *HasTrailingSlashExpression::clone() const
{
    return new HasTrailingSlashExpression(*this);
}

void HasTrailingSlashExpression::swap(HasTrailingSlashExpression &first, HasTrailingSlashExpression &second)
{
    Expression::swap(first, second);
    std::swap(first.m_expr, second.m_expr);
}

} // namespace Internal
} // namespace VcProjectManager
