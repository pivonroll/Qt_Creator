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
#include "variablepipe.h"
#include <QString>
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

VariablePipe::VariablePipe(Expression *left, Expression *right)
    : BinaryExpression(VARIABLE_PIPE, left, right)
{
}

VariablePipe::VariablePipe(const VariablePipe &other)
    : BinaryExpression(other)
{
}

VariablePipe::VariablePipe(VariablePipe &&other)
    : VariablePipe()
{
    swap(*this, other);
}

VariablePipe &VariablePipe::operator=(VariablePipe other)
{
    swap(*this, other);
    return *this;
}

VariablePipe::~VariablePipe()
{
}

QVariant VariablePipe::evaluate(const EvaluateArguments &evalArgs) const
{
    QString left;
    QString right;

    if (leftOperand())
        left = leftOperand()->evaluate(evalArgs).toString();
    if (rightOperand())
        right = rightOperand()->evaluate(evalArgs).toString();

    return QVariant(left + QLatin1String("|") + right);
}

QString VariablePipe::toString() const
{
    QString str;
    if (leftOperand() && rightOperand()) {
        QString left = leftOperand()->toString();

        if (left.startsWith(QLatin1String("'")))
            left = left.remove(0, 1);
        if (left.endsWith(QLatin1String("'")))
            left.remove(left.length() - 1, 1);

        QString right = rightOperand()->toString();

        if (right.startsWith(QLatin1String("'")))
            right = right.remove(0, 1);
        if (right.endsWith(QLatin1String("'")))
            right.remove(right.length() - 1, 1);

        str = QLatin1String("'") + left + QLatin1String("|") + right + QLatin1String("'");
    }
    return str;
}

VariablePipe::VariablePipe()
    : BinaryExpression()
{
}

void VariablePipe::swap(VariablePipe &first, VariablePipe &second)
{
    BinaryExpression::swap(first, second);
}

} // namespace Internal
} // namespace VcProjectManager
