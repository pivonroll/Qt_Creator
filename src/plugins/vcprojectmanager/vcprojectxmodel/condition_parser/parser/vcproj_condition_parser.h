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
#ifndef VCPROJ_CONDITION_PARSER_H
#define VCPROJ_CONDITION_PARSER_H

#include <QString>
#include <QVarLengthArray>
#include "vcproj_condition_grammar_p.h"

class QString;

namespace VcProjectManager {
namespace Internal {
    class Expression;
}
}

class VcProjConditionParser : public vcproj_condition_grammar
{
public:
    VcProjConditionParser(const QString &expression);
    ~VcProjConditionParser();
    VcProjectManager::Internal::Expression *condExpression() const;
    bool parse();

protected:
    void reallocateStack();
    inline VcProjectManager::Internal::Expression* &sym(int index)
    { return m_sym_stack [m_tos + index - 1]; }

    int m_stack_size;
    int m_tos;
    VcProjectManager::Internal::Expression *m_condExpression;
    QVarLengthArray<int, 128> m_state_stack;
    QVarLengthArray<VcProjectManager::Internal::Expression *, 128> m_sym_stack;
    QString m_expression;
};

#endif // VCPROJ_CONDITION_PARSER_H
