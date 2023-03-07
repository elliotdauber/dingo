%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "scanner.hh"
#undef  YY_DECL
#define YY_DECL int DSN::Scanner::yylex( DSN::Parser::semantic_type * const lval, DSN::Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = DSN::Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="DSN::Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
            /* int n = 0; */
%}

"define"       {return(token::DEFINE);}

"apply"       {return(token::APPLY);}

"pattern"       {return(token::PATTERN);}

"module"         {return(token::MODULE);}

"spec"          {return(token::SPEC);}

"for"           {return(token::FOR);}

"in"            {return(token::IN);}

"import"        {return(token::IMPORT);}

"as"            {return(token::AS);}

\<((\{[a-zA-Z_]+\})|[a-zA-Z])([a-zA-Z_]*(\{[a-zA-Z_]+\})*)*\> {
   yylval->build< std::string >( yytext );
   return( token::STENCILED_ID );
}

((\:\:)?[a-zA-Z_]+)+ {
               yylval->build< std::string >( yytext );
               return( token::ID );
             }

[a-zA-Z0-9_\/\.\-]+   {
               yylval->build< std::string >( yytext );
               return( token::FILEPATH );
             }

[\-\+\@\$]  {
               yylval->build< std::string >( yytext );
               return( token::DECORATOR );
            }

[\*\&]+     {
               yylval->build< std::string >( yytext );
               return( token::QUALIFIERS );
            }

[ \t\f\v\r]+ {}

\n          {
               // Update line number
               loc->lines();
            }

\{          {return(token::LBRACKET);}

\}          {return(token::RBRACKET);}

\(          {return(token::LPAREN);}

\)          {return(token::RPAREN);}

\;          {return(token::SEMICOLON);}

\:          {return(token::COLON);}

\,          {return(token::COMMA);}

\[          {return(token::LSQUARE);}

\]          {return(token::RSQUARE);}

\<          {return(token::LESSTHAN);}

.           {}

%%

