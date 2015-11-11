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
#include <QString>
#include <QDebug>

#include "vcproj_condition_parser.h"
#include "../tokenizer/conditiontokenizer.h"
#include "../tokenizer/token.h"
#include "../expression/bracketexpression.h"
#include "../expression/compareexpression.h"
#include "../expression/binaryexpression.h"
#include "../expression/existsexpression.h"
#include "../expression/hastrailingslashexpression.h"
#include "../expression/logicalbinaryexpression.h"
#include "../expression/notexpression.h"
#include "../expression/number.h"
#include "../expression/numericbinaryexpression.h"
#include "../expression/stringcompare.h"
#include "../expression/stringliteral.h"
#include "../expression/variable.h"
#include "../expression/variablepipe.h"

VcProjConditionParser::VcProjConditionParser(const QString &expression)
    : m_stack_size(128),
      m_tos(0),
      m_condExpression(0),
      m_expression(expression)
{
    m_state_stack.resize(128);
    m_sym_stack.resize(128);
}

VcProjConditionParser::~VcProjConditionParser()
{
}

VcProjectManager::Internal::Expression *VcProjConditionParser::condExpression() const
{
    return m_condExpression;
}

bool VcProjConditionParser::parse()
{
    const int INITIAL_STATE = 0;
    VcProjectManager::Internal::ConditionTokenizer tokenizer(m_expression);

    int yytoken = -1;

    m_tos = 0;
    m_state_stack[++m_tos] = INITIAL_STATE;
    int act = m_state_stack[++m_tos] = INITIAL_STATE;

    VcProjectManager::Internal::Token token_p;
    while (true)
    {
        if (yytoken == -1 && - TERMINAL_COUNT != action_index [act]) {
            token_p = tokenizer.nextToken();

            if (token_p.isValid())
                yytoken = token_p.id();
        }

        act = t_action (act, yytoken);

        if (act == ACCEPT_STATE)
            return true;

        else if (act > 0) {
            if (++m_tos == m_stack_size)
                reallocateStack();
            m_state_stack [m_tos] = act;
            yytoken = -1;
        }

        else if (act < 0)
        {
            int r = - act - 1;

            m_tos -= rhs [r];
            act = m_state_stack [m_tos++];

            switch (r) {

            case 0: {
            m_condExpression = sym(1);
            break;
            }

            case 1: {
            sym(1) = new VcProjectManager::Internal::LogicalBinaryExpression(sym(1), VcProjectManager::Internal::LogicalBinaryExpression::OP_AND, sym(3));
            break;
            }

            case 2: {
            sym(1) = new VcProjectManager::Internal::LogicalBinaryExpression(sym(1), VcProjectManager::Internal::LogicalBinaryExpression::OP_OR, sym(3));
            break;
            }

            case 3: {
            sym(1) = new VcProjectManager::Internal::NotExpression(sym(2));
            break;
            }

            case 5: {
            sym(1) = new VcProjectManager::Internal::StringCompare(sym(1), VcProjectManager::Internal::StringCompare::OP_EQUAL, sym(3));
            break;
            }

            case 6: {
            sym(1) = new VcProjectManager::Internal::StringCompare(sym(1), VcProjectManager::Internal::StringCompare::OP_NOT_EQUAL, sym(3));
            break;
            }

            case 9: {
            sym(1) = new VcProjectManager::Internal::BracketExpression(sym(2));
            break;
            }

            case 11: {
            sym(1) = new VcProjectManager::Internal::CompareExpression(sym(1), VcProjectManager::Internal::CompareExpression::OP_LESS, sym(3));
            break;
            }

            case 12: {
            sym(1) = new VcProjectManager::Internal::CompareExpression(sym(1), VcProjectManager::Internal::CompareExpression::OP_GREATER, sym(3));
            break;
            }

            case 13: {
            sym(1) = new VcProjectManager::Internal::CompareExpression(sym(1), VcProjectManager::Internal::CompareExpression::OP_LESS_OR_EQUAL, sym(3));
            break;
            }

            case 14: {
            sym(1) = new VcProjectManager::Internal::CompareExpression(sym(1), VcProjectManager::Internal::CompareExpression::OP_GREATER_OR_EQUAL, sym(3));
            break;
            }

            case 15: {
            sym(1) = new VcProjectManager::Internal::NumericBinaryExpression(sym(1), VcProjectManager::Internal::NumericBinaryExpression::OP_ADD, sym(3));
            break;
            }

            case 16: {
            sym(1) = new VcProjectManager::Internal::NumericBinaryExpression(sym(1), VcProjectManager::Internal::NumericBinaryExpression::OP_MINUS, sym(3));
            break;
            }

            case 17: {
            sym(1) = new VcProjectManager::Internal::NumericBinaryExpression(sym(1), VcProjectManager::Internal::NumericBinaryExpression::OP_MULTIPLY, sym(3));
            break;
            }

            case 18: {
            sym(1) = new VcProjectManager::Internal::NumericBinaryExpression(sym(1), VcProjectManager::Internal::NumericBinaryExpression::OP_DIVIDE, sym(3));
            break;
            }

            case 19: {
            sym(1) = new VcProjectManager::Internal::Number(token_p.value().toDouble());
            break;
            }

            case 21: {
            sym(1) = new VcProjectManager::Internal::ExistsExpression(sym(1));
            break;
            }

            case 22: {
            sym(1) = new VcProjectManager::Internal::HasTrailingSlashExpression(sym(3));
            break;
            }

            case 23: {
            sym(1) = sym(2);
            break;
            }

            case 25: {
            sym(1) = new VcProjectManager::Internal::VariablePipe(sym(1), sym(3));
            break;
            }

            case 27: {
            sym(1) = new VcProjectManager::Internal::StringLiteral(token_p.value());
            break;
            }

            case 28: {
            sym(1) = new VcProjectManager::Internal::Variable(token_p.value());
            break;
            }

            } // switch

            act = m_state_stack [m_tos] = nt_action (act, lhs [r] - TERMINAL_COUNT);

            } else {
            return false;
        }
    }
}

void VcProjConditionParser::reallocateStack()
{
    m_stack_size <<= 1;

    m_state_stack.resize(m_stack_size);
    m_sym_stack.resize(m_stack_size);
}
