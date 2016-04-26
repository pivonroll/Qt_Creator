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
#include "existsexpression.h"
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

ExistsExpression::ExistsExpression(Expression *strExpr)
    : Expression(EXISTS),
      m_strExpr(strExpr)
{
}

ExistsExpression::ExistsExpression(const ExistsExpression &other)
{
    if (other.m_strExpr)
        m_strExpr = other.m_strExpr->clone();
}

ExistsExpression::ExistsExpression(ExistsExpression &&other)
{
    swap(*this, other);
}

ExistsExpression &ExistsExpression::operator=(ExistsExpression other)
{
    swap(*this, other);
    return *this;
}

ExistsExpression::~ExistsExpression()
{
    delete m_strExpr;
}

QVariant ExistsExpression::evaluate(const EvaluateArguments &evalArgs) const
{
    QString str;
    if (m_strExpr)
        str = m_strExpr->evaluate(evalArgs).toString();

    return QVariant(QFileInfo(str).exists() || QDir(str).exists());
}

QString ExistsExpression::toString() const
{
    QString str;
    if (m_strExpr)
        str = QLatin1String("Exists(") + m_strExpr->toString() + QLatin1String(")");
    return str;
}

Expression *ExistsExpression::clone() const
{
    return new ExistsExpression(*this);
}

void ExistsExpression::swap(ExistsExpression &first, ExistsExpression &second)
{
    Expression::swap(first, second);
    std::swap(first.m_strExpr, second.m_strExpr);
}

} // namespace Internal
} // namespace VcProjectManager
