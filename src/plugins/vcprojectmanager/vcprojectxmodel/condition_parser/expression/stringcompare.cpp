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
#include "stringcompare.h"
#include <QString>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

StringCompare::StringCompare(Expression *left, CompareOperation oper, Expression *right)
    : BinaryExpression(STRING_COMPARE, left, right),
      m_operation(oper)
{
}

StringCompare::StringCompare(const StringCompare &other)
    : BinaryExpression(other)
{
    m_operation = other.m_operation;
}

StringCompare::StringCompare(StringCompare &&other)
    : StringCompare()
{
    swap(*this, other);
}

StringCompare &StringCompare::operator=(StringCompare other)
{
    swap(*this, other);
    return *this;
}

StringCompare::~StringCompare()
{
}

QVariant StringCompare::evaluate(const EvaluateArguments &evalArgs) const
{
    QString left;
    QString right;

    if (m_leftOperand)
        left = m_leftOperand->evaluate(evalArgs).toString();
    if (m_rightOperand)
        right = m_rightOperand->evaluate(evalArgs).toString();

    if (m_operation == OP_EQUAL)
        return left == right;
    return left != right;
}

QString StringCompare::toString() const
{
    if (m_leftOperand && m_rightOperand) {
        if (m_operation == OP_EQUAL)
            return m_leftOperand->toString() + QLatin1String(" == ") + m_rightOperand->toString();
        else
            return m_leftOperand->toString() + QLatin1String(" != ") + m_rightOperand->toString();
    }

    return QString();
}

Expression *StringCompare::clone() const
{
    return new StringCompare(*this);
}

Expression *StringCompare::leftOperand() const
{
    return m_leftOperand;
}

Expression *StringCompare::rightOperand() const
{
    return m_rightOperand;
}

StringCompare::StringCompare()
    : BinaryExpression()
{
}

void StringCompare::swap(StringCompare &first, StringCompare &second)
{
    BinaryExpression::swap(first, second);
    std::swap(first.m_operation, second.m_operation);
}

} // namespace Internal
} // namespace VcProjectManager
