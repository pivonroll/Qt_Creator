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
#ifndef VCPROJECTMANAGER_INTERNAL_STRING_COMPARE_H
#define VCPROJECTMANAGER_INTERNAL_STRING_COMPARE_H

#include "binaryexpression.h"

namespace VcProjectManager {
namespace Internal {

class StringCompare : public BinaryExpression
{
public:
    enum CompareOperation {
        OP_EQUAL,
        OP_NOT_EQUAL
    };

    StringCompare(Expression *left, CompareOperation oper, Expression *right);
    StringCompare(const StringCompare &other);
    StringCompare(StringCompare &&other);
    StringCompare& operator=(StringCompare other);
    ~StringCompare();

    // Expression interface
    QVariant evaluate(const EvaluateArguments &evalArgs) const;
    QString toString() const;
    Expression *clone() const;
    Expression* leftOperand() const;
    Expression* rightOperand() const;

private:
    StringCompare();
    static void swap(StringCompare &first, StringCompare &second);
    CompareOperation m_operation;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // VCPROJECTMANAGER_INTERNAL_STRING_COMPARE_H
