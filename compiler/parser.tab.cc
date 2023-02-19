// A Bison parser, made by GNU Bison 3.7.6.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 56 "parser.yy"

   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   #include "orchestrator.hh"

#undef yylex
#define yylex scanner.yylex

#line 57 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "parser.yy"
namespace DSN {
#line 150 "parser.tab.cc"

  /// Build a parser object.
  Parser::Parser (Scanner       &scanner_yyarg, Orchestrator  &orchestrator_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      orchestrator (orchestrator_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_module_item: // module_item
        value.copy< AbstractModuleMethod* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decorators: // decorators
        value.copy< DecoratorList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_for_each: // for_each
        value.copy< ForEach* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_decl: // pattern_member_decl
        value.copy< Member* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_assignment: // pattern_member_assignment
        value.copy< MemberAssignment* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_assignments: // pattern_member_assignments
        value.copy< MemberAssignmentList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_decls: // pattern_member_decls
        value.copy< MemberList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_method_arg: // method_arg
        value.copy< MethodArg* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_method_args: // method_args
        value.copy< MethodArgList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_decl_no_keyword: // module_decl_no_keyword
      case symbol_kind::S_module_decl: // module_decl
        value.copy< ModuleDecl* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_list: // module_list
      case symbol_kind::S_modules: // modules
        value.copy< ModuleList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_method: // module_method
        value.copy< ModuleMethod* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_items: // module_items
        value.copy< ModuleMethodList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_application: // pattern_application
        value.copy< PatternApplication* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_definition: // pattern_definition
        value.copy< PatternDefinition* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_spec_decl: // pattern_spec_decl
        value.copy< Spec* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_spec_decls: // pattern_spec_decls
        value.copy< SpecList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type: // type
        value.copy< Type* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_header: // module_header
        value.copy< std::pair<Module*, ModuleList*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STENCILED_ID: // STENCILED_ID
      case symbol_kind::S_DECORATOR: // DECORATOR
      case symbol_kind::S_QUALIFIERS: // QUALIFIERS
      case symbol_kind::S_single_type: // single_type
      case symbol_kind::S_method_name: // method_name
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_module_item: // module_item
        value.move< AbstractModuleMethod* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_decorators: // decorators
        value.move< DecoratorList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_for_each: // for_each
        value.move< ForEach* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_member_decl: // pattern_member_decl
        value.move< Member* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_member_assignment: // pattern_member_assignment
        value.move< MemberAssignment* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_member_assignments: // pattern_member_assignments
        value.move< MemberAssignmentList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_member_decls: // pattern_member_decls
        value.move< MemberList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_method_arg: // method_arg
        value.move< MethodArg* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_method_args: // method_args
        value.move< MethodArgList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_module_decl_no_keyword: // module_decl_no_keyword
      case symbol_kind::S_module_decl: // module_decl
        value.move< ModuleDecl* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_module_list: // module_list
      case symbol_kind::S_modules: // modules
        value.move< ModuleList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_module_method: // module_method
        value.move< ModuleMethod* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_module_items: // module_items
        value.move< ModuleMethodList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_application: // pattern_application
        value.move< PatternApplication* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_definition: // pattern_definition
        value.move< PatternDefinition* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_spec_decl: // pattern_spec_decl
        value.move< Spec* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern_spec_decls: // pattern_spec_decls
        value.move< SpecList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_type: // type
        value.move< Type* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_module_header: // module_header
        value.move< std::pair<Module*, ModuleList*> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STENCILED_ID: // STENCILED_ID
      case symbol_kind::S_DECORATOR: // DECORATOR
      case symbol_kind::S_QUALIFIERS: // QUALIFIERS
      case symbol_kind::S_single_type: // single_type
      case symbol_kind::S_method_name: // method_name
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  Parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_module_item: // module_item
        value.YY_MOVE_OR_COPY< AbstractModuleMethod* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decorators: // decorators
        value.YY_MOVE_OR_COPY< DecoratorList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_for_each: // for_each
        value.YY_MOVE_OR_COPY< ForEach* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_decl: // pattern_member_decl
        value.YY_MOVE_OR_COPY< Member* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_assignment: // pattern_member_assignment
        value.YY_MOVE_OR_COPY< MemberAssignment* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_assignments: // pattern_member_assignments
        value.YY_MOVE_OR_COPY< MemberAssignmentList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_decls: // pattern_member_decls
        value.YY_MOVE_OR_COPY< MemberList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_method_arg: // method_arg
        value.YY_MOVE_OR_COPY< MethodArg* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_method_args: // method_args
        value.YY_MOVE_OR_COPY< MethodArgList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_decl_no_keyword: // module_decl_no_keyword
      case symbol_kind::S_module_decl: // module_decl
        value.YY_MOVE_OR_COPY< ModuleDecl* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_list: // module_list
      case symbol_kind::S_modules: // modules
        value.YY_MOVE_OR_COPY< ModuleList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_method: // module_method
        value.YY_MOVE_OR_COPY< ModuleMethod* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_items: // module_items
        value.YY_MOVE_OR_COPY< ModuleMethodList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_application: // pattern_application
        value.YY_MOVE_OR_COPY< PatternApplication* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_definition: // pattern_definition
        value.YY_MOVE_OR_COPY< PatternDefinition* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_spec_decl: // pattern_spec_decl
        value.YY_MOVE_OR_COPY< Spec* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_spec_decls: // pattern_spec_decls
        value.YY_MOVE_OR_COPY< SpecList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type: // type
        value.YY_MOVE_OR_COPY< Type* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_header: // module_header
        value.YY_MOVE_OR_COPY< std::pair<Module*, ModuleList*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STENCILED_ID: // STENCILED_ID
      case symbol_kind::S_DECORATOR: // DECORATOR
      case symbol_kind::S_QUALIFIERS: // QUALIFIERS
      case symbol_kind::S_single_type: // single_type
      case symbol_kind::S_method_name: // method_name
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_module_item: // module_item
        value.move< AbstractModuleMethod* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decorators: // decorators
        value.move< DecoratorList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_for_each: // for_each
        value.move< ForEach* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_decl: // pattern_member_decl
        value.move< Member* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_assignment: // pattern_member_assignment
        value.move< MemberAssignment* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_assignments: // pattern_member_assignments
        value.move< MemberAssignmentList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_member_decls: // pattern_member_decls
        value.move< MemberList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_method_arg: // method_arg
        value.move< MethodArg* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_method_args: // method_args
        value.move< MethodArgList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_decl_no_keyword: // module_decl_no_keyword
      case symbol_kind::S_module_decl: // module_decl
        value.move< ModuleDecl* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_list: // module_list
      case symbol_kind::S_modules: // modules
        value.move< ModuleList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_method: // module_method
        value.move< ModuleMethod* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_items: // module_items
        value.move< ModuleMethodList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_application: // pattern_application
        value.move< PatternApplication* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_definition: // pattern_definition
        value.move< PatternDefinition* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_spec_decl: // pattern_spec_decl
        value.move< Spec* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern_spec_decls: // pattern_spec_decls
        value.move< SpecList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type: // type
        value.move< Type* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_module_header: // module_header
        value.move< std::pair<Module*, ModuleList*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STENCILED_ID: // STENCILED_ID
      case symbol_kind::S_DECORATOR: // DECORATOR
      case symbol_kind::S_QUALIFIERS: // QUALIFIERS
      case symbol_kind::S_single_type: // single_type
      case symbol_kind::S_method_name: // method_name
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_module_item: // module_item
        value.copy< AbstractModuleMethod* > (that.value);
        break;

      case symbol_kind::S_decorators: // decorators
        value.copy< DecoratorList* > (that.value);
        break;

      case symbol_kind::S_for_each: // for_each
        value.copy< ForEach* > (that.value);
        break;

      case symbol_kind::S_pattern_member_decl: // pattern_member_decl
        value.copy< Member* > (that.value);
        break;

      case symbol_kind::S_pattern_member_assignment: // pattern_member_assignment
        value.copy< MemberAssignment* > (that.value);
        break;

      case symbol_kind::S_pattern_member_assignments: // pattern_member_assignments
        value.copy< MemberAssignmentList* > (that.value);
        break;

      case symbol_kind::S_pattern_member_decls: // pattern_member_decls
        value.copy< MemberList* > (that.value);
        break;

      case symbol_kind::S_method_arg: // method_arg
        value.copy< MethodArg* > (that.value);
        break;

      case symbol_kind::S_method_args: // method_args
        value.copy< MethodArgList* > (that.value);
        break;

      case symbol_kind::S_module_decl_no_keyword: // module_decl_no_keyword
      case symbol_kind::S_module_decl: // module_decl
        value.copy< ModuleDecl* > (that.value);
        break;

      case symbol_kind::S_module_list: // module_list
      case symbol_kind::S_modules: // modules
        value.copy< ModuleList* > (that.value);
        break;

      case symbol_kind::S_module_method: // module_method
        value.copy< ModuleMethod* > (that.value);
        break;

      case symbol_kind::S_module_items: // module_items
        value.copy< ModuleMethodList* > (that.value);
        break;

      case symbol_kind::S_pattern_application: // pattern_application
        value.copy< PatternApplication* > (that.value);
        break;

      case symbol_kind::S_pattern_definition: // pattern_definition
        value.copy< PatternDefinition* > (that.value);
        break;

      case symbol_kind::S_pattern_spec_decl: // pattern_spec_decl
        value.copy< Spec* > (that.value);
        break;

      case symbol_kind::S_pattern_spec_decls: // pattern_spec_decls
        value.copy< SpecList* > (that.value);
        break;

      case symbol_kind::S_type: // type
        value.copy< Type* > (that.value);
        break;

      case symbol_kind::S_module_header: // module_header
        value.copy< std::pair<Module*, ModuleList*> > (that.value);
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STENCILED_ID: // STENCILED_ID
      case symbol_kind::S_DECORATOR: // DECORATOR
      case symbol_kind::S_QUALIFIERS: // QUALIFIERS
      case symbol_kind::S_single_type: // single_type
      case symbol_kind::S_method_name: // method_name
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_module_item: // module_item
        value.move< AbstractModuleMethod* > (that.value);
        break;

      case symbol_kind::S_decorators: // decorators
        value.move< DecoratorList* > (that.value);
        break;

      case symbol_kind::S_for_each: // for_each
        value.move< ForEach* > (that.value);
        break;

      case symbol_kind::S_pattern_member_decl: // pattern_member_decl
        value.move< Member* > (that.value);
        break;

      case symbol_kind::S_pattern_member_assignment: // pattern_member_assignment
        value.move< MemberAssignment* > (that.value);
        break;

      case symbol_kind::S_pattern_member_assignments: // pattern_member_assignments
        value.move< MemberAssignmentList* > (that.value);
        break;

      case symbol_kind::S_pattern_member_decls: // pattern_member_decls
        value.move< MemberList* > (that.value);
        break;

      case symbol_kind::S_method_arg: // method_arg
        value.move< MethodArg* > (that.value);
        break;

      case symbol_kind::S_method_args: // method_args
        value.move< MethodArgList* > (that.value);
        break;

      case symbol_kind::S_module_decl_no_keyword: // module_decl_no_keyword
      case symbol_kind::S_module_decl: // module_decl
        value.move< ModuleDecl* > (that.value);
        break;

      case symbol_kind::S_module_list: // module_list
      case symbol_kind::S_modules: // modules
        value.move< ModuleList* > (that.value);
        break;

      case symbol_kind::S_module_method: // module_method
        value.move< ModuleMethod* > (that.value);
        break;

      case symbol_kind::S_module_items: // module_items
        value.move< ModuleMethodList* > (that.value);
        break;

      case symbol_kind::S_pattern_application: // pattern_application
        value.move< PatternApplication* > (that.value);
        break;

      case symbol_kind::S_pattern_definition: // pattern_definition
        value.move< PatternDefinition* > (that.value);
        break;

      case symbol_kind::S_pattern_spec_decl: // pattern_spec_decl
        value.move< Spec* > (that.value);
        break;

      case symbol_kind::S_pattern_spec_decls: // pattern_spec_decls
        value.move< SpecList* > (that.value);
        break;

      case symbol_kind::S_type: // type
        value.move< Type* > (that.value);
        break;

      case symbol_kind::S_module_header: // module_header
        value.move< std::pair<Module*, ModuleList*> > (that.value);
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STENCILED_ID: // STENCILED_ID
      case symbol_kind::S_DECORATOR: // DECORATOR
      case symbol_kind::S_QUALIFIERS: // QUALIFIERS
      case symbol_kind::S_single_type: // single_type
      case symbol_kind::S_method_name: // method_name
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_module_item: // module_item
        yylhs.value.emplace< AbstractModuleMethod* > ();
        break;

      case symbol_kind::S_decorators: // decorators
        yylhs.value.emplace< DecoratorList* > ();
        break;

      case symbol_kind::S_for_each: // for_each
        yylhs.value.emplace< ForEach* > ();
        break;

      case symbol_kind::S_pattern_member_decl: // pattern_member_decl
        yylhs.value.emplace< Member* > ();
        break;

      case symbol_kind::S_pattern_member_assignment: // pattern_member_assignment
        yylhs.value.emplace< MemberAssignment* > ();
        break;

      case symbol_kind::S_pattern_member_assignments: // pattern_member_assignments
        yylhs.value.emplace< MemberAssignmentList* > ();
        break;

      case symbol_kind::S_pattern_member_decls: // pattern_member_decls
        yylhs.value.emplace< MemberList* > ();
        break;

      case symbol_kind::S_method_arg: // method_arg
        yylhs.value.emplace< MethodArg* > ();
        break;

      case symbol_kind::S_method_args: // method_args
        yylhs.value.emplace< MethodArgList* > ();
        break;

      case symbol_kind::S_module_decl_no_keyword: // module_decl_no_keyword
      case symbol_kind::S_module_decl: // module_decl
        yylhs.value.emplace< ModuleDecl* > ();
        break;

      case symbol_kind::S_module_list: // module_list
      case symbol_kind::S_modules: // modules
        yylhs.value.emplace< ModuleList* > ();
        break;

      case symbol_kind::S_module_method: // module_method
        yylhs.value.emplace< ModuleMethod* > ();
        break;

      case symbol_kind::S_module_items: // module_items
        yylhs.value.emplace< ModuleMethodList* > ();
        break;

      case symbol_kind::S_pattern_application: // pattern_application
        yylhs.value.emplace< PatternApplication* > ();
        break;

      case symbol_kind::S_pattern_definition: // pattern_definition
        yylhs.value.emplace< PatternDefinition* > ();
        break;

      case symbol_kind::S_pattern_spec_decl: // pattern_spec_decl
        yylhs.value.emplace< Spec* > ();
        break;

      case symbol_kind::S_pattern_spec_decls: // pattern_spec_decls
        yylhs.value.emplace< SpecList* > ();
        break;

      case symbol_kind::S_type: // type
        yylhs.value.emplace< Type* > ();
        break;

      case symbol_kind::S_module_header: // module_header
        yylhs.value.emplace< std::pair<Module*, ModuleList*> > ();
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STENCILED_ID: // STENCILED_ID
      case symbol_kind::S_DECORATOR: // DECORATOR
      case symbol_kind::S_QUALIFIERS: // QUALIFIERS
      case symbol_kind::S_single_type: // single_type
      case symbol_kind::S_method_name: // method_name
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 6: // declaration: pattern_definition
#line 137 "parser.yy"
                                 {orchestrator.add_pattern_defn(yystack_[0].value.as < PatternDefinition* > ()); }
#line 1243 "parser.tab.cc"
    break;

  case 7: // declaration: pattern_application
#line 138 "parser.yy"
                                  {orchestrator.add_pattern_appl(yystack_[0].value.as < PatternApplication* > ()); }
#line 1249 "parser.tab.cc"
    break;

  case 8: // declaration: module_decl
#line 139 "parser.yy"
                          { orchestrator.add_module_decl(yystack_[0].value.as < ModuleDecl* > ()); }
#line 1255 "parser.tab.cc"
    break;

  case 9: // pattern_definition: DEFINE PATTERN ID LBRACKET pattern_member_decls pattern_spec_decls RBRACKET
#line 141 "parser.yy"
                                                                                                 {yylhs.value.as < PatternDefinition* > () = new PatternDefinition(yystack_[4].value.as < std::string > (), yystack_[2].value.as < MemberList* > (), yystack_[1].value.as < SpecList* > ());}
#line 1261 "parser.tab.cc"
    break;

  case 10: // pattern_member_decls: %empty
#line 143 "parser.yy"
                              {yylhs.value.as < MemberList* > () = new MemberList(); }
#line 1267 "parser.tab.cc"
    break;

  case 11: // pattern_member_decls: pattern_member_decl pattern_member_decls
#line 144 "parser.yy"
                                                                {yylhs.value.as < MemberList* > () = new MemberList(yystack_[0].value.as < MemberList* > (), yystack_[1].value.as < Member* > ());}
#line 1273 "parser.tab.cc"
    break;

  case 12: // pattern_member_decl: ID COLON type SEMICOLON
#line 146 "parser.yy"
                                              {yylhs.value.as < Member* > () = new Member(yystack_[3].value.as < std::string > (), yystack_[1].value.as < Type* > ());}
#line 1279 "parser.tab.cc"
    break;

  case 13: // pattern_spec_decls: %empty
#line 148 "parser.yy"
                            {yylhs.value.as < SpecList* > () = new SpecList();}
#line 1285 "parser.tab.cc"
    break;

  case 14: // pattern_spec_decls: pattern_spec_decl pattern_spec_decls
#line 149 "parser.yy"
                                                          {yylhs.value.as < SpecList* > () = new SpecList(yystack_[0].value.as < SpecList* > (), yystack_[1].value.as < Spec* > ());}
#line 1291 "parser.tab.cc"
    break;

  case 15: // pattern_spec_decl: SPEC module_decl_no_keyword
#line 152 "parser.yy"
                                                {yylhs.value.as < Spec* > () = new Spec(yystack_[0].value.as < ModuleDecl* > ());}
#line 1297 "parser.tab.cc"
    break;

  case 16: // type: single_type
#line 156 "parser.yy"
                   {yylhs.value.as < Type* > () = new Type(yystack_[0].value.as < std::string > (), false);}
#line 1303 "parser.tab.cc"
    break;

  case 17: // type: single_type LSQUARE RSQUARE
#line 157 "parser.yy"
                                   {yylhs.value.as < Type* > () = new Type(yystack_[2].value.as < std::string > (), true);}
#line 1309 "parser.tab.cc"
    break;

  case 18: // single_type: MODULE
#line 159 "parser.yy"
                     {yylhs.value.as < std::string > () = "module";}
#line 1315 "parser.tab.cc"
    break;

  case 19: // pattern_application: APPLY PATTERN ID LBRACKET pattern_member_assignments RBRACKET
#line 163 "parser.yy"
                                                                                    {yylhs.value.as < PatternApplication* > () = new PatternApplication(yystack_[3].value.as < std::string > (), yystack_[1].value.as < MemberAssignmentList* > ());}
#line 1321 "parser.tab.cc"
    break;

  case 20: // pattern_member_assignments: %empty
#line 165 "parser.yy"
                                    {yylhs.value.as < MemberAssignmentList* > () = new MemberAssignmentList(); }
#line 1327 "parser.tab.cc"
    break;

  case 21: // pattern_member_assignments: pattern_member_assignment pattern_member_assignments
#line 166 "parser.yy"
                                                                       {yylhs.value.as < MemberAssignmentList* > () = new MemberAssignmentList(yystack_[0].value.as < MemberAssignmentList* > (), yystack_[1].value.as < MemberAssignment* > ());}
#line 1333 "parser.tab.cc"
    break;

  case 22: // pattern_member_assignment: ID COLON ID SEMICOLON
#line 169 "parser.yy"
                                                  {yylhs.value.as < MemberAssignment* > () = new MemberAssignment(yystack_[3].value.as < std::string > (), new Module(yystack_[1].value.as < std::string > ())); }
#line 1339 "parser.tab.cc"
    break;

  case 23: // pattern_member_assignment: ID COLON module_list SEMICOLON
#line 170 "parser.yy"
                                                           {yylhs.value.as < MemberAssignment* > () = new MemberAssignment(yystack_[3].value.as < std::string > (), yystack_[1].value.as < ModuleList* > ()); }
#line 1345 "parser.tab.cc"
    break;

  case 24: // module_list: LSQUARE modules RSQUARE
#line 174 "parser.yy"
                                      {yylhs.value.as < ModuleList* > () = yystack_[1].value.as < ModuleList* > ();}
#line 1351 "parser.tab.cc"
    break;

  case 25: // module_list: LSQUARE RSQUARE
#line 175 "parser.yy"
                              {yylhs.value.as < ModuleList* > () = new ModuleList();}
#line 1357 "parser.tab.cc"
    break;

  case 26: // modules: ID
#line 177 "parser.yy"
             {yylhs.value.as < ModuleList* > () = new ModuleList(new Module(yystack_[0].value.as < std::string > ()));}
#line 1363 "parser.tab.cc"
    break;

  case 27: // modules: modules COMMA ID
#line 178 "parser.yy"
                           {yylhs.value.as < ModuleList* > () = new ModuleList(yystack_[2].value.as < ModuleList* > (), new Module(yystack_[0].value.as < std::string > ()));}
#line 1369 "parser.tab.cc"
    break;

  case 28: // module_decl_no_keyword: module_header SEMICOLON
#line 180 "parser.yy"
                                                 {yylhs.value.as < ModuleDecl* > () = new ModuleDecl(yystack_[1].value.as < std::pair<Module*, ModuleList*> > ().first, yystack_[1].value.as < std::pair<Module*, ModuleList*> > ().second, new ModuleList(), new ModuleMethodList());}
#line 1375 "parser.tab.cc"
    break;

  case 29: // module_decl_no_keyword: module_header COLON LBRACKET module_items RBRACKET
#line 181 "parser.yy"
                                                                            {yylhs.value.as < ModuleDecl* > () = new ModuleDecl(yystack_[4].value.as < std::pair<Module*, ModuleList*> > ().first, yystack_[4].value.as < std::pair<Module*, ModuleList*> > ().second, new ModuleList(), yystack_[1].value.as < ModuleMethodList* > ());}
#line 1381 "parser.tab.cc"
    break;

  case 30: // module_decl_no_keyword: module_header COLON module_list SEMICOLON
#line 182 "parser.yy"
                                                                   {yylhs.value.as < ModuleDecl* > () = new ModuleDecl(yystack_[3].value.as < std::pair<Module*, ModuleList*> > ().first, yystack_[3].value.as < std::pair<Module*, ModuleList*> > ().second, yystack_[1].value.as < ModuleList* > (), new ModuleMethodList());}
#line 1387 "parser.tab.cc"
    break;

  case 31: // module_decl_no_keyword: module_header COLON module_list LBRACKET module_items RBRACKET
#line 183 "parser.yy"
                                                                                        {yylhs.value.as < ModuleDecl* > () = new ModuleDecl(yystack_[5].value.as < std::pair<Module*, ModuleList*> > ().first, yystack_[5].value.as < std::pair<Module*, ModuleList*> > ().second, yystack_[3].value.as < ModuleList* > (), yystack_[1].value.as < ModuleMethodList* > ());}
#line 1393 "parser.tab.cc"
    break;

  case 32: // module_decl: MODULE module_decl_no_keyword
#line 185 "parser.yy"
                                            {yylhs.value.as < ModuleDecl* > () = yystack_[0].value.as < ModuleDecl* > ();}
#line 1399 "parser.tab.cc"
    break;

  case 33: // module_header: ID LESSTHAN modules
#line 187 "parser.yy"
                                    {yylhs.value.as < std::pair<Module*, ModuleList*> > () = std::pair<Module*, ModuleList*>(new Module(yystack_[2].value.as < std::string > ()), yystack_[0].value.as < ModuleList* > ());}
#line 1405 "parser.tab.cc"
    break;

  case 34: // module_header: ID
#line 188 "parser.yy"
                   {yylhs.value.as < std::pair<Module*, ModuleList*> > () = std::pair<Module*, ModuleList*>(new Module(yystack_[0].value.as < std::string > ()), new ModuleList());}
#line 1411 "parser.tab.cc"
    break;

  case 35: // module_item: module_method
#line 191 "parser.yy"
                            {yylhs.value.as < AbstractModuleMethod* > () = yystack_[0].value.as < ModuleMethod* > ();}
#line 1417 "parser.tab.cc"
    break;

  case 36: // module_item: for_each
#line 192 "parser.yy"
                       {yylhs.value.as < AbstractModuleMethod* > () = yystack_[0].value.as < ForEach* > ();}
#line 1423 "parser.tab.cc"
    break;

  case 37: // module_items: %empty
#line 194 "parser.yy"
                      {yylhs.value.as < ModuleMethodList* > () = new ModuleMethodList();}
#line 1429 "parser.tab.cc"
    break;

  case 38: // module_items: module_item module_items
#line 195 "parser.yy"
                                          {yylhs.value.as < ModuleMethodList* > () = new ModuleMethodList(yystack_[0].value.as < ModuleMethodList* > (), yystack_[1].value.as < AbstractModuleMethod* > ());}
#line 1435 "parser.tab.cc"
    break;

  case 39: // module_method: decorators ID method_name LPAREN method_args RPAREN SEMICOLON
#line 197 "parser.yy"
                                                                              {yylhs.value.as < ModuleMethod* > () = new ModuleMethod(yystack_[6].value.as < DecoratorList* > (), yystack_[5].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[2].value.as < MethodArgList* > ());}
#line 1441 "parser.tab.cc"
    break;

  case 40: // method_name: ID
#line 199 "parser.yy"
                 {yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();}
#line 1447 "parser.tab.cc"
    break;

  case 41: // method_name: STENCILED_ID
#line 200 "parser.yy"
                           {yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();}
#line 1453 "parser.tab.cc"
    break;

  case 42: // decorators: %empty
#line 202 "parser.yy"
                    {yylhs.value.as < DecoratorList* > () = new DecoratorList();}
#line 1459 "parser.tab.cc"
    break;

  case 43: // decorators: DECORATOR decorators
#line 203 "parser.yy"
                                  {yylhs.value.as < DecoratorList* > () = new DecoratorList(yystack_[0].value.as < DecoratorList* > (), new Decorator(yystack_[1].value.as < std::string > ()));}
#line 1465 "parser.tab.cc"
    break;

  case 44: // method_args: %empty
#line 206 "parser.yy"
                     {yylhs.value.as < MethodArgList* > () = new MethodArgList();}
#line 1471 "parser.tab.cc"
    break;

  case 45: // method_args: method_arg
#line 207 "parser.yy"
                         {yylhs.value.as < MethodArgList* > () = new MethodArgList(yystack_[0].value.as < MethodArg* > ());}
#line 1477 "parser.tab.cc"
    break;

  case 46: // method_args: method_arg COMMA method_args
#line 208 "parser.yy"
                                           {yylhs.value.as < MethodArgList* > () = new MethodArgList(yystack_[0].value.as < MethodArgList* > (), yystack_[2].value.as < MethodArg* > ());}
#line 1483 "parser.tab.cc"
    break;

  case 47: // method_arg: ID QUALIFIERS
#line 210 "parser.yy"
                           {yylhs.value.as < MethodArg* > () = new MethodArg(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ());}
#line 1489 "parser.tab.cc"
    break;

  case 48: // method_arg: ID
#line 211 "parser.yy"
                {yylhs.value.as < MethodArg* > () = new MethodArg(yystack_[0].value.as < std::string > (), "");}
#line 1495 "parser.tab.cc"
    break;

  case 49: // for_each: FOR ID IN module_list COLON LBRACKET module_items RBRACKET
#line 213 "parser.yy"
                                                                      {yylhs.value.as < ForEach* > () = new ForEach(yystack_[6].value.as < std::string > (), yystack_[4].value.as < ModuleList* > (), yystack_[1].value.as < ModuleMethodList* > ());}
#line 1501 "parser.tab.cc"
    break;


#line 1505 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0





  const signed char Parser::yypact_ninf_ = -54;

  const signed char Parser::yytable_ninf_ = -43;

  const signed char
  Parser::yypact_[] =
  {
       2,   -54,    -6,    15,    24,    32,     8,   -54,   -54,   -54,
     -54,    30,    31,    14,   -54,    -9,   -54,   -54,   -54,    29,
      33,    34,   -54,    19,    35,    36,   -54,    18,     0,    11,
      -1,    20,    23,    35,    22,    37,    36,    43,    42,    45,
       0,    44,   -54,    47,   -54,   -54,     1,     0,   -54,    38,
      24,    46,    23,   -54,     6,   -54,   -54,   -54,   -54,    39,
     -54,   -54,    26,   -54,    48,   -54,    40,    50,   -54,   -54,
     -54,    49,    51,    52,   -54,   -54,    54,   -54,   -54,    41,
     -54,   -54,    53,    57,   -54,    56,    60,    55,    58,     0,
     -54,    59,    57,    61,   -54,   -54,   -54
  };

  const signed char
  Parser::yydefact_[] =
  {
       0,     2,     0,     0,     0,     0,     0,     4,     6,     7,
       8,     0,     0,    34,    32,     0,     1,     3,     5,     0,
       0,     0,    28,     0,    10,    20,    26,    33,    37,     0,
       0,     0,    13,    10,     0,     0,    20,     0,    42,     0,
      37,     0,    35,     0,    36,    25,     0,    37,    30,     0,
       0,     0,    13,    11,     0,    19,    21,    27,    43,     0,
      38,    29,     0,    24,     0,    18,     0,    16,    15,     9,
      14,     0,     0,     0,    40,    41,     0,    31,    12,     0,
      22,    23,     0,    44,    17,     0,    48,     0,    45,    37,
      47,     0,    44,     0,    39,    46,    49
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -54,   -54,   -54,    62,   -54,    63,   -54,     5,   -54,   -54,
     -54,   -54,    64,   -54,   -53,    65,    21,   -54,   -54,   -54,
     -40,   -54,   -54,    66,   -22,   -54,   -54
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     5,     6,     7,     8,    32,    33,    51,    52,    66,
      67,     9,    35,    36,    30,    27,    14,    10,    15,    40,
      41,    42,    76,    43,    87,    88,    44
  };

  const signed char
  Parser::yytable_[] =
  {
      60,    72,     1,   -42,    22,    38,    47,    64,    17,    71,
      11,    63,    48,    23,    26,    29,     2,     3,    39,     4,
      82,    45,     2,     3,    37,     4,    28,    13,    29,    74,
      75,    12,    16,    19,    20,    21,    24,    26,    31,    34,
      25,    37,    49,    50,    54,    55,    57,    38,    59,    93,
      62,    84,    61,    78,    69,    65,    77,    70,    73,    79,
      86,    29,    80,    89,    81,    83,    90,    91,    18,    96,
      95,    68,    94,     0,     0,    85,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    46,     0,    53,     0,     0,     0,
      56,     0,     0,     0,    58
  };

  const signed char
  Parser::yycheck_[] =
  {
      40,    54,     0,     3,    13,     5,     7,    47,     0,     3,
      16,    10,    13,    22,     3,     9,    14,    15,    18,    17,
      73,    10,    14,    15,    23,    17,     7,     3,     9,     3,
       4,    16,     0,     3,     3,    21,     7,     3,     3,     3,
       7,    23,    22,    20,    22,     8,     3,     5,     3,    89,
       3,    10,     8,    13,     8,    17,     8,    52,    19,     9,
       3,     9,    13,     7,    13,    11,     6,    12,     6,     8,
      92,    50,    13,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    33,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    38
  };

  const signed char
  Parser::yystos_[] =
  {
       0,     0,    14,    15,    17,    25,    26,    27,    28,    35,
      41,    16,    16,     3,    40,    42,     0,     0,    27,     3,
       3,    21,    13,    22,     7,     7,     3,    39,     7,     9,
      38,     3,    29,    30,     3,    36,    37,    23,     5,    18,
      43,    44,    45,    47,    50,    10,    39,     7,    13,    22,
      20,    31,    32,    29,    22,     8,    36,     3,    47,     3,
      44,     8,     3,    10,    44,    17,    33,    34,    40,     8,
      31,     3,    38,    19,     3,     4,    46,     8,    13,     9,
      13,    13,    38,    11,    10,    22,     3,    48,    49,     7,
       6,    12,    23,    44,    13,    48,     8
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    24,    25,    25,    26,    26,    27,    27,    27,    28,
      29,    29,    30,    31,    31,    32,    33,    33,    34,    35,
      36,    36,    37,    37,    38,    38,    39,    39,    40,    40,
      40,    40,    41,    42,    42,    43,    43,    44,    44,    45,
      46,    46,    47,    47,    48,    48,    48,    49,    49,    50
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     2,     1,     1,     1,     7,
       0,     2,     4,     0,     2,     2,     1,     3,     1,     6,
       0,     2,     4,     4,     3,     2,     1,     3,     2,     5,
       4,     6,     2,     3,     1,     1,     1,     0,     2,     7,
       1,     1,     0,     2,     0,     1,     3,     2,     1,     8
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "ID", "STENCILED_ID",
  "DECORATOR", "QUALIFIERS", "LBRACKET", "RBRACKET", "LSQUARE", "RSQUARE",
  "LPAREN", "RPAREN", "SEMICOLON", "DEFINE", "APPLY", "PATTERN", "MODULE",
  "FOR", "IN", "SPEC", "LESSTHAN", "COLON", "COMMA", "$accept",
  "program_option", "program", "declaration", "pattern_definition",
  "pattern_member_decls", "pattern_member_decl", "pattern_spec_decls",
  "pattern_spec_decl", "type", "single_type", "pattern_application",
  "pattern_member_assignments", "pattern_member_assignment", "module_list",
  "modules", "module_decl_no_keyword", "module_decl", "module_header",
  "module_item", "module_items", "module_method", "method_name",
  "decorators", "method_args", "method_arg", "for_each", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,   133,   133,   133,   135,   135,   137,   138,   139,   141,
     143,   144,   146,   148,   149,   152,   156,   157,   159,   163,
     165,   166,   169,   170,   174,   175,   177,   178,   180,   181,
     182,   183,   185,   187,   188,   191,   192,   194,   195,   197,
     199,   200,   202,   203,   206,   207,   208,   210,   211,   213
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
    };
    // Last valid token kind.
    const int code_max = 278;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 5 "parser.yy"
} // DSN
#line 1922 "parser.tab.cc"

#line 215 "parser.yy"



void DSN::Parser::error( const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
