#ifndef __SCANNER_HH__
#define __SCANNER_HH__

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "location.hh"
#include "parser.tab.hh"

namespace DSN {

class Scanner : public yyFlexLexer {
public:
    Scanner(std::istream* in)
        : yyFlexLexer(in) {};
    virtual ~Scanner() {};

    //get rid of override virtual function warning
    using FlexLexer::yylex;

    virtual int yylex(DSN::Parser::semantic_type* const lval,
        DSN::Parser::location_type* location);
    // YY_DECL defined in lexer.l
    // Method body created by flex in vc_lexer.yy.cc

private:
    /* yyval ptr */
    DSN::Parser::semantic_type* yylval = nullptr;
};

}

#endif
