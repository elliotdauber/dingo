#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"
#include <clang/Tooling/CommonOptionsParser.h>

#include <iostream>

#include "dir.hh"

using namespace clang;
using namespace clang::tooling;
using namespace std;

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
    bool VisitFunctionDecl(FunctionDecl* f)
    {
        // cout << "in function decl! " << f->getNameAsString() << endl;
        // TraverseStmt(f->getBody());
        // return true;
        if (f->hasBody()) {
            cout << "in function declaration with body!" << endl;
            Stmt* FuncBody = f->getBody();
            //TODO: can we get number of statements to tell if its a declaration or implementation?

            string func_name = f->getNameAsString();

            cout << "function name: " << func_name << endl;

            // Check if this is a member function
            if (CXXRecordDecl* Class = dyn_cast<CXXRecordDecl>(f->getParent())) {
                string class_name = Class->getNameAsString();
                cout
                    << "method " << func_name
                    << " belongs to class " << Class->getNameAsString() << endl;

                if (!modules.count(class_name)) {
                    modules[class_name] = new DIR::Module(class_name);
                }
            } else {
                // This is not a member function
                cout << "function " << f->getNameAsString()
                     << " is not a member function" << endl;
            }

            // Type name as string
            QualType QT = f->getReturnType();
            string TypeStr = QT.getAsString();
            cout << "return type: " << TypeStr << endl;
        } else {
            cout << "in function declaration" << endl;
        }
        return true;
    }

    bool VisitStmt(Stmt* s)
    {
        cout << "In statement: " << s->getStmtClassName() << endl;

        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl* Decl)
    {
        if (Decl->getDefinition()) {
            string class_name = Decl->getNameAsString();
            cout << "Found class: " << class_name << endl;
            ;
            if (!modules.count(class_name)) {
                modules[class_name] = new DIR::Module(class_name);
            }
        }

        // Print the names of the parent classes
        for (auto& BaseSpec : Decl->bases()) {
            QualType BaseType = BaseSpec.getType();
            CXXRecordDecl* BaseDecl = BaseType->getAsCXXRecordDecl();
            llvm::outs() << "  Parent class: " << BaseDecl->getNameAsString() << "\n";
        }
        return true;
    }

    map<string, DIR::Module*> modules;
};

class MyASTConsumer : public ASTConsumer {
public:
    MyASTConsumer()
        : Visitor()
    {
    }

    virtual void HandleTranslationUnit(ASTContext& Context)
    {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    MyASTVisitor Visitor;
};

class MyFrontendAction : public ASTFrontendAction {
public:
    virtual unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance& Compiler, llvm::StringRef InFile)
    {
        return make_unique<MyASTConsumer>();
    }
};

int main(int argc, const char** argv)
{
    llvm::cl::OptionCategory oc("my_option", "todo");
    llvm::Expected<CommonOptionsParser> OptionsParser = CommonOptionsParser::create(argc, argv, oc);
    ClangTool Tool((*OptionsParser).getCompilations(),
        (*OptionsParser).getSourcePathList());
    return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}

// int main(int argc, const char** argv)
// {
//     string error;
//     unique_ptr<clang::tooling::CompilationDatabase> CompilationDB = clang::tooling::CompilationDatabase::loadFromDirectory("./programs/compile_commands.json", error);
//     if (error != "") {
//         cerr << "Failed to load compilation database.\n";
//         return -1;
//     }

//     return 0;
// }

// bool VisitVarDecl(VarDecl* v)
// {
//     cout << "In variable decl: " << v->getNameAsString() << endl;
//     if (v->hasInit()) {
//         TraverseStmt(v->getInit());
//     }
//     return true;
// }

// bool VisitIfStmt(IfStmt* s)
// {
//     cout << "In if statement" << endl;
//     TraverseStmt(s->getCond());
//     TraverseStmt(s->getThen());
//     if (s->getElse()) {
//         TraverseStmt(s->getElse());
//     }

//     return true;
// }

// bool VisitWhileStmt(WhileStmt* s)
// {
//     cout << "In while statement" << endl;
//     TraverseStmt(s->getCond());
//     TraverseStmt(s->getBody());

//     return true;
// }

// bool VisitCallExpr(CallExpr* e)
// {
//     cout << "In function call: " << e->getDirectCallee()->getNameAsString() << endl;

//     for (auto arg : e->arguments()) {
//         TraverseStmt(arg);
//     }

//     return true;
// }