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
#include "tokenizer.h"
#include "token.h"

namespace VcProjectManager {
namespace Internal {

Tokenizer::Tokenizer()
    : m_results(0)
{
}

Tokenizer::~Tokenizer()
{
    delete m_results;
}

void Tokenizer::addRule(const QString &ruleRegExp, int id)
{
    m_rules.add(ruleRegExp.toStdString(), id);
}

void Tokenizer::parse(const QString &expression)
{
    lexertl::generator::build(m_rules, m_sm);
    m_expr = expression.toStdString();
    m_results = new lexertl::srmatch(m_expr.begin(), m_expr.end());
}

Token Tokenizer::nextToken() const
{
    if (m_results) {
        lexertl::lookup(m_sm, *m_results);

        if (m_results->id != 0 && m_results->id != m_results->npos())
            return Token(m_results->id, QString::fromStdString(m_results->str()));
    }

    return Token();
}

} // Internal
} // VcProjectManager
