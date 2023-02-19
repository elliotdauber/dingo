#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"
#include <clang/Tooling/CommonOptionsParser.h>

#include <fstream>
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
                curr_module = modules[class_name];
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

    // bool VisitStmt(Stmt* s)
    // {
    //     cout << "In statement: " << s->getStmtClassName() << endl;

    //     return true;
    // }

    bool VisitIfStmt(IfStmt* s)
    {
        cout << "ATTENTION: In if statement" << endl;
        TraverseStmt(s->getCond());
        TraverseStmt(s->getThen());
        if (s->getElse()) {
            TraverseStmt(s->getElse());
        }

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
            curr_module = modules[class_name];

            // Print the names of the parent classes
            for (auto& BaseSpec : Decl->bases()) {
                QualType BaseType = BaseSpec.getType();
                CXXRecordDecl* BaseDecl = BaseType->getAsCXXRecordDecl();
                string parent_name = BaseDecl->getNameAsString();
                cout << "Parent class: " << parent_name << "\n";
                //TODO: inheriting from ignored classes
                DIR::Module* parent_module = modules[parent_name];
                modules[class_name]->parents.insert(parent_module);
            }
        }
        return true;
    }

    bool VisitVarDecl(VarDecl* v)
    {
        cout << "In variable decl: " << v->getNameAsString() << endl;
        QualType varType = v->getType();
        string var_type = varType.getAsString();
        cout << "Variable type: " << var_type << endl;
        //TODO: wb struct?
        if (var_type.find("class ") == 0) {
            var_type = var_type.substr(6);
        }
        if (modules.count(var_type)) {
            cout << "inserting dep on " << var_type << endl;
            curr_module->dependencies.insert(modules[var_type]);
        }
        if (v->hasInit()) {
            TraverseStmt(v->getInit());
        }
        return true;
    }

    //TODO: do we need simple traversers like these?
    //or is there a default?
    bool VisitWhileStmt(WhileStmt* s)
    {
        TraverseStmt(s->getCond());
        TraverseStmt(s->getBody());
        return true;
    }

    bool VisitCXXMemberCallExpr(CXXMemberCallExpr* expr)
    {
        // Get the method being called
        CXXMethodDecl* methodDecl = expr->getMethodDecl();

        // Get the parent class of the method
        CXXRecordDecl* classDecl = methodDecl->getParent();

        if (classDecl != nullptr) {
            // If the parent class is not null, print its name
            cout << "Called method of class: " << classDecl->getNameAsString() << endl;
        }

        return true;
    }

    DIR::Module* curr_module;
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

        ofstream dotfile("output.dot");
        dotfile
            << "digraph example {" << endl;
        dotfile << "rankdir=LR;" << endl;
        dotfile << "node [shape=square];" << endl;

        DIR::NodeGenVisitor* node_gen = new DIR::NodeGenVisitor(dotfile);

        for (auto it = Visitor.modules.begin(); it != Visitor.modules.end(); ++it) {
            it->second->accept(node_gen);
        }

        dotfile << endl;

        DIR::EdgeGenVisitor* edge_gen = new DIR::EdgeGenVisitor(dotfile);

        for (auto it = Visitor.modules.begin(); it != Visitor.modules.end(); ++it) {
            it->second->accept(edge_gen);
        }

        dotfile << "label=\"The System\"" << endl;
        dotfile << "style=filled" << endl;
        dotfile << "fillcolor=yellow" << endl;
        dotfile << "}" << endl;

        dotfile.close();
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

// bool VisitCallExpr(CallExpr* e)
// {
//     cout << "In function call: " << e->getDirectCallee()->getNameAsString() << endl;

//     for (auto arg : e->arguments()) {
//         TraverseStmt(arg);
//     }

//     return true;
// }