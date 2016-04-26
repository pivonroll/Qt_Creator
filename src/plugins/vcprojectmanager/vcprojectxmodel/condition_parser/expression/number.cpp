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
#include "number.h"

#include <QtCore>
#include "evaluatearguments.h"

namespace VcProjectManager {
namespace Internal {

Number::Number(int val)
    : Expression(NUMBER),
      m_value(val)
{
}

Number::Number(const Number &other)
    : Expression(other)
{
    m_value = other.m_value;
}

Number::Number(Number &&other)
    : Number()
{
    swap(*this, other);
}

Number &Number::operator=(Number other)
{
    swap(*this, other);
    return *this;
}

Number::~Number()
{
}

QVariant Number::evaluate(const EvaluateArguments &evalArgs) const
{
    Q_UNUSED(evalArgs)
    return QVariant(m_value);
}

QString Number::toString() const
{
    return QVariant(m_value).toString();
}

Expression *Number::clone() const
{
    return new Number(*this);
}

void Number::swap(Number &first, Number &second)
{
    Expression::swap(first, second);
}

} // namespace Internal
} // namespace VcProjectManager
