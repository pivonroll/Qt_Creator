/**************************************************************************
**
** Copyright (c) 2014 Bojan Petrovic
** Copyright (c) 2014 Radovan Zivkovic
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
#ifndef VCPROJECTMANAGER_INTERNAL_EXPRESSION_H
#define VCPROJECTMANAGER_INTERNAL_EXPRESSION_H

class QVariant;
class QString;

namespace VcProjectManager {
namespace Internal {

class EvaluateArguments;

class Expression
{
public:
    enum ExpressionType {
        LOGICAL_BINARY,
        BRACKET,
        COMPARE,
        STRING_COMPARE,
        EXISTS,
        HAS_TRAILING_SLASH,
        NOT,
        NUMERIC_BINARY,
        NUMBER,
        STRING_LITERAL,
        VARIABLE,
        VARIABLE_PIPE
    };

    Expression();
    Expression(const Expression &other);
    virtual ExpressionType type() const = 0;
    virtual ~Expression() {}
    virtual QVariant evaluate(const EvaluateArguments &evalArgs) const = 0;
    virtual QString toString() const = 0;
    virtual Expression* clone() const = 0;

protected:
    static void swap(Expression &first, Expression &second);
    ExpressionType m_type;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_EXPRESSION_H
