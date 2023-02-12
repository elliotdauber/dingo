%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {DSN}
/**
 * bison 3.3.2 change
 * %define parser_class_name to this, updated
 * should work for previous bison versions as 
 * well. -jcb 24 Jan 2020
 */
%define api.parser.class {Parser}

%code requires{
   namespace DSN {
      class Orchestrator;
      class Scanner;
   }
   
   class ModuleDecl;
   class Module;
   class ModuleList;
   class AbstractModuleMethod;
   class ModuleMethod;
   class ModuleMethodList;
   class ForEach;
   class Member;
   class MemberList;
   class MemberAssignment;
   class MemberAssignmentList;
   class Spec;
   class SpecList;
   class Decorator;
   class DecoratorList;
   class MethodArg;
   class MethodArgList;
   class PatternDefinition;
   class PatternApplication;
   class Type;


// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { Scanner       &scanner  }
%parse-param { Orchestrator  &orchestrator  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   #include "orchestrator.hh"

#undef yylex
#define yylex scanner.yylex
}

/* %union {
  int int_val;
  std::string str_val;
  MathExpr math_expr;
  Var var;
} */

%define api.value.type variant
%define parse.assert

%token               END    0     "end of file"
%token <std::string> ID
%token <std::string> STENCILED_ID
%token <std::string> DECORATOR 
%token <std::string> QUALIFIERS
%token               LBRACKET
%token               RBRACKET
%token               LSQUARE
%token               RSQUARE
%token               LPAREN
%token               RPAREN
%token               SEMICOLON
%token               DEFINE
%token               APPLY
%token               PATTERN
%token               MODULE
%token               FOR
%token               IN
%token               SPEC
%token               LESSTHAN
%token               COLON
%token               COMMA

%locations

%type <ModuleDecl*> module_decl
%type <ModuleDecl*> module_decl_no_keyword
%type <std::pair<Module*, ModuleList*>> module_header
%type <ModuleList*> modules
%type <ModuleList*> module_list
%type <std::string> single_type
%type <Type*> type

%type <ModuleMethod*> module_method
%type <ModuleMethodList*> module_items
%type <std::string> method_name
%type <ForEach*> for_each
%type <AbstractModuleMethod*> module_item

%type <Member*> pattern_member_decl
%type <MemberList*> pattern_member_decls
%type <Spec*> pattern_spec_decl;
%type <SpecList*> pattern_spec_decls;

%type <MemberAssignment*> pattern_member_assignment
%type <MemberAssignmentList*> pattern_member_assignments

%type <DecoratorList*> decorators  
%type <MethodArgList*> method_args
%type <MethodArg*> method_arg

%type <PatternDefinition*> pattern_definition
%type <PatternApplication*> pattern_application

%%

program_option : END | program END;

program : declaration | program declaration;

declaration : pattern_definition {orchestrator.add_pattern_defn($1); }
            | pattern_application {orchestrator.add_pattern_appl($1); }
            | module_decl { orchestrator.add_module_decl($1); };

pattern_definition : DEFINE PATTERN ID LBRACKET pattern_member_decls pattern_spec_decls RBRACKET {$$ = new PatternDefinition($3, $5, $6);};

pattern_member_decls : %empty {$$ = new MemberList(); } 
                     | pattern_member_decl pattern_member_decls {$$ = new MemberList($2, $1);};

pattern_member_decl : ID COLON type SEMICOLON {$$ = new Member($1, $3);};

pattern_spec_decls : %empty {$$ = new SpecList();} 
                   | pattern_spec_decl pattern_spec_decls {$$ = new SpecList($2, $1);};


pattern_spec_decl : SPEC module_decl_no_keyword {$$ = new Spec($2);};



type : single_type {$$ = new Type($1, false);} 
     | single_type LSQUARE RSQUARE {$$ = new Type($1, true);};

single_type : MODULE {$$ = "module";};



pattern_application : APPLY PATTERN ID LBRACKET pattern_member_assignments RBRACKET {$$ = new PatternApplication($3, $5);};

pattern_member_assignments : %empty {$$ = new MemberAssignmentList(); }
                | pattern_member_assignment pattern_member_assignments {$$ = new MemberAssignmentList($2, $1);};


pattern_member_assignment : ID COLON ID SEMICOLON {$$ = new MemberAssignment($1, new Module($3)); }
                          | ID COLON module_list SEMICOLON {$$ = new MemberAssignment($1, $3); };



module_list : LSQUARE modules RSQUARE {$$ = $2;}
            | LSQUARE RSQUARE {$$ = new ModuleList();}

modules : ID {$$ = new ModuleList(new Module($1));} 
        | modules COMMA ID {$$ = new ModuleList($1, new Module($3));};

module_decl_no_keyword : module_header SEMICOLON {$$ = new ModuleDecl($1.first, $1.second, new ModuleList(), new ModuleMethodList());}
                       | module_header COLON LBRACKET module_items RBRACKET {$$ = new ModuleDecl($1.first, $1.second, new ModuleList(), $4);}
                       | module_header COLON module_list SEMICOLON {$$ = new ModuleDecl($1.first, $1.second, $3, new ModuleMethodList());}
                       | module_header COLON module_list LBRACKET module_items RBRACKET {$$ = new ModuleDecl($1.first, $1.second, $3, $5);};

module_decl : MODULE module_decl_no_keyword {$$ = $2;};

module_header : ID LESSTHAN modules {$$ = std::pair<Module*, ModuleList*>(new Module($1), $3);}
              | ID {$$ = std::pair<Module*, ModuleList*>(new Module($1), new ModuleList());};


module_item : module_method {$$ = $1;}
            | for_each {$$ = $1;};

module_items : %empty {$$ = new ModuleMethodList();}
               | module_item module_items {$$ = new ModuleMethodList($2, $1);};

module_method : decorators ID method_name LPAREN method_args RPAREN SEMICOLON {$$ = new ModuleMethod($1, $2, $3, $5);};

method_name : ID {$$ = $1;}
            | STENCILED_ID {$$ = $1;};

decorators : %empty {$$ = new DecoratorList();}
           | DECORATOR decorators {$$ = new DecoratorList($2, new Decorator($1));};

//TODO: i think you could end up with (arg1, arg2,)
method_args : %empty {$$ = new MethodArgList();}
            | method_arg {$$ = new MethodArgList($1);}
            | method_arg COMMA method_args {$$ = new MethodArgList($3, $1);};

method_arg : ID QUALIFIERS {$$ = new MethodArg($1, $2);} 
           | ID {$$ = new MethodArg($1, "");};

for_each : FOR ID IN module_list COLON LBRACKET module_items RBRACKET {$$ = new ForEach($2, $4, $7);};

%%


void DSN::Parser::error( const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
