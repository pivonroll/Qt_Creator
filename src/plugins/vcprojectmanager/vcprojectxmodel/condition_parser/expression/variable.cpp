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
#include "variable.h"
#include "evaluatearguments.h"
#include <QVariant>

namespace VcProjectManager {
namespace Internal {

Variable::Variable(const QString &str)
    : StringExpression(VARIABLE, str)
{
}

Variable::Variable(const Variable &other)
    : StringExpression(other)
{
}

Variable::Variable(Variable &&other)
    : Variable()
{
    swap(*this, other);
}

Variable &Variable::operator=(Variable other)
{
    swap(*this, other);
    return *this;
}

Variable::~Variable()
{
}

QVariant Variable::evaluate(const EvaluateArguments &evalArgs) const
{
    return evalArgs.value(m_str);
}

QString Variable::toString() const
{
    return QLatin1String("'") + m_str + QLatin1String("'");;
}

Expression *Variable::clone() const
{
    return new Variable(*this);
}

void Variable::swap(Variable &first, Variable &second)
{
    StringExpression::swap(first, second);
}

} // namespace Internal
} // namespace VcProjectManager
