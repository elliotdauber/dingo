#ifndef __PARSE_HH__
#define __PARSE_HH__

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"
#include <clang/Tooling/CommonOptionsParser.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "dir.hh"

using namespace clang;
using namespace tooling;
using namespace std;

string cleanup_type(string type);

vector<string> get_all_types(string type);

string type_qualifiers(string type);

class DingoCppASTVisitor : public RecursiveASTVisitor<DingoCppASTVisitor> {
public:
    void init(ASTContext& context, set<string> mods_to_process);

    bool isInMainFile(SourceLocation loc);
    //used for out-of-class function decls
    bool VisitFunctionDecl(FunctionDecl* f);

    bool VisitNamespaceDecl(NamespaceDecl* nd);

    bool VisitDeclRefExpr(DeclRefExpr* expr);

    //custom function, used for class function decls
    bool MyVisitFunctionDecl(FunctionDecl* f);

    bool VisitCXXRecordDecl(CXXRecordDecl* Decl);

    bool VisitVarDecl(VarDecl* v);

    bool VisitCXXConstructExpr(CXXConstructExpr* c);

    bool VisitCXXMemberCallExpr(CXXMemberCallExpr* expr);

    void add_dependency(string dependency_name);

    DIR::Module* curr_module;
    map<string, DIR::Module*> modules;
    ASTContext* astContext;
    set<string> modules_to_process;
    string curr_namespace;
};

class DingoCppASTConsumer : public ASTConsumer {
public:
    DingoCppASTConsumer()
        : Visitor()
    {
    }

    virtual void HandleTranslationUnit(ASTContext& Context);
    DingoCppASTVisitor Visitor;
};

class DingoCppFrontendAction : public ASTFrontendAction {
public:
    virtual unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance& Compiler, llvm::StringRef InFile)
    {
        return make_unique<DingoCppASTConsumer>();
    }
};

map<string, DIR::Module*> parse(string filename, set<string> modules_to_process);

#endif