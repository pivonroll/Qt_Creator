INCLUDEPATH +=  $$PWD/lexertl

HEADERS += \
    $$PWD/expression/variablepipe.h \
    $$PWD/expression/variable.h \
    $$PWD/expression/stringliteral.h \
    $$PWD/expression/stringexpression.h \
    $$PWD/expression/number.h \
    $$PWD/expression/notexpression.h \
    $$PWD/expression/hastrailingslashexpression.h \
    $$PWD/expression/expression.h \
    $$PWD/expression/existsexpression.h \
    $$PWD/expression/evaluatearguments.h \
    $$PWD/expression/compareexpression.h \
    $$PWD/expression/bracketexpression.h \
    $$PWD/expression/binaryexpression.h \
    $$PWD/parser/vcproj_condition_parser.h \
    $$PWD/parser/vcproj_condition_grammar_p.h \
    $$PWD/tokenizer/tokenizer.h \
    $$PWD/tokenizer/token.h \
    $$PWD/tokenizer/conditiontokenizer.h \
    $$PWD/expression/stringcompare.h \
    $$PWD/expression/numericbinaryexpression.h \
    $$PWD/expression/logicalbinaryexpression.h

SOURCES += \
    $$PWD/expression/variablepipe.cpp \
    $$PWD/expression/variable.cpp \
    $$PWD/expression/stringliteral.cpp \
    $$PWD/expression/stringexpression.cpp \
    $$PWD/expression/number.cpp \
    $$PWD/expression/notexpression.cpp \
    $$PWD/expression/hastrailingslashexpression.cpp \
    $$PWD/expression/expression.cpp \
    $$PWD/expression/existsexpression.cpp \
    $$PWD/expression/evaluatearguments.cpp \
    $$PWD/expression/compareexpression.cpp \
    $$PWD/expression/bracketexpression.cpp \
    $$PWD/expression/binaryexpression.cpp \
    $$PWD/parser/vcproj_condition_parser.cpp \
    $$PWD/parser/vcproj_condition_grammar.cpp \
    $$PWD/tokenizer/tokenizer.cpp \
    $$PWD/tokenizer/token.cpp \
    $$PWD/tokenizer/conditiontokenizer.cpp \
    $$PWD/expression/stringcompare.cpp \
    $$PWD/expression/numericbinaryexpression.cpp \
    $$PWD/expression/logicalbinaryexpression.cpp

OTHER_FILES += \
    $$PWD/parser/parser.g
