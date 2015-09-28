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
#include "conditiontokenizer.h"
#include "../parser/vcproj_condition_grammar_p.h"
#include "token.h"
#include <QString>

namespace VcProjectManager {
namespace Internal {

ConditionTokenizer::ConditionTokenizer(const QString &expression)
{
    m_tokenizer.addRule(QLatin1String("\"(\""), vcproj_condition_grammar::TKN_BRACKET_O);
    m_tokenizer.addRule(QLatin1String("\")\""), vcproj_condition_grammar::TKN_BRACKET_C);
    m_tokenizer.addRule(QLatin1String("\"!\""), vcproj_condition_grammar::TKN_NOT);
    m_tokenizer.addRule(QLatin1String("!="), vcproj_condition_grammar::TKN_NOT_EQUAL);
    m_tokenizer.addRule(QLatin1String("=="), vcproj_condition_grammar::TKN_EQUAL);
    m_tokenizer.addRule(QLatin1String("\"-\""), vcproj_condition_grammar::TKN_MINUS);
    m_tokenizer.addRule(QLatin1String("\"+\""), vcproj_condition_grammar::TKN_PLUS);
    m_tokenizer.addRule(QLatin1String("\"/\""), vcproj_condition_grammar::TKN_DIVIDE);
    m_tokenizer.addRule(QLatin1String("\"*\""), vcproj_condition_grammar::TKN_TIMES);
    m_tokenizer.addRule(QLatin1String("And"), vcproj_condition_grammar::TKN_AND);
    m_tokenizer.addRule(QLatin1String("Or"), vcproj_condition_grammar::TKN_OR);
    m_tokenizer.addRule(QLatin1String("Exists"), vcproj_condition_grammar::TKN_EXISTS);
    m_tokenizer.addRule(QLatin1String("HasTrailingSlash"), vcproj_condition_grammar::TKN_HAS_TRAILING_SLASH);
    m_tokenizer.addRule(QLatin1String("&gt"), vcproj_condition_grammar::TKN_GREATER);
    m_tokenizer.addRule(QLatin1String("&lt"), vcproj_condition_grammar::TKN_LESS);
    m_tokenizer.addRule(QLatin1String("<="), vcproj_condition_grammar::TKN_LESS_OR_EQUAL);
    m_tokenizer.addRule(QLatin1String(">="), vcproj_condition_grammar::TKN_GREATER_OR_EQUAL);
    m_tokenizer.addRule(QLatin1String("'"), vcproj_condition_grammar::TKN_APOSTROPHE);
    m_tokenizer.addRule(QLatin1String("\"|\""), vcproj_condition_grammar::TKN_PIPE);

    m_tokenizer.addRule(QLatin1String("[0-9]+"), vcproj_condition_grammar::TKN_NUMBER);
    m_tokenizer.addRule(QLatin1String("([a-zA-Z]|_)([a-zA-Z0-9]|_)+"), vcproj_condition_grammar::TKN_STRING_CONSTANT);
    m_tokenizer.addRule(QLatin1String("$([a-zA-Z]|_)([a-zA-Z0-9]|_)+"), vcproj_condition_grammar::TKN_STRING_VARIABLE);
    m_tokenizer.addRule(QLatin1String("$\"(\"([a-zA-Z]|_)([a-zA-Z0-9]|_)+\")\""), vcproj_condition_grammar::TKN_STRING_VARIABLE);
    m_tokenizer.addRule(QLatin1String("\\s+"), 100);

    m_tokenizer.parse(expression);
}

Token ConditionTokenizer::nextToken() const
{
    Token token = m_tokenizer.nextToken();

    while (token.id() == 100)
        token = m_tokenizer.nextToken();

    if (token.isValid())
        return token;
    return Token(vcproj_condition_grammar::EOF_SYMBOL, QLatin1String(""));
}

} // namespace Internal
} // namespace VcProjectManager
