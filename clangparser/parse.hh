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
#include <sstream>

#include "dir.hh"

using namespace clang;
using namespace tooling;
using namespace std;

//TODO: this is a hack, find a better way
map<string, DIR::Module*> global_modules;
set<string> global_modules_to_process;

string cleanup_type(string type)
{
    while (true) {
        if (type.find("class ") == 0) {
            type = type.substr(6);
        } else if (type.find("struct ") == 0) {
            type = type.substr(7);
        } else if (type.find("const ") == 0) {
            type = type.substr(6);
        } else {
            break;
        }
    }

    while (true) {
        char last_char = type.at(type.length() - 1);
        if (last_char == '*' or last_char == '&' or last_char == ' ') {
            type = type.substr(0, type.length() - 1);
        } else {
            break;
        }
    }
    return type;
}

string type_qualifiers(string type)
{
    ostringstream qualifiers;
    while (true) {
        char last_char = type.at(type.length() - 1);
        if (last_char == '*' or last_char == '&') {
            qualifiers << last_char;
            type = type.substr(0, type.length() - 1);
        } else if (last_char == ' ') {
            type = type.substr(0, type.length() - 1);
        } else
            break;
    }
    string qualifiers_str = qualifiers.str();
    reverse(qualifiers_str.begin(), qualifiers_str.end());
    return qualifiers_str;
}

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
    void init(ASTContext& context, set<string> mods_to_process)
    {
        modules_to_process = mods_to_process;
        astContext = &context;
    }

    bool isInMainFile(SourceLocation loc)
    {
        SourceManager& SM = astContext->getSourceManager();
        FileID mainFileID = SM.getMainFileID();
        FileID locFileID = SM.getFileID(loc);
        return mainFileID == locFileID;
    }

    //used for out-of-class function decls
    bool VisitFunctionDecl(FunctionDecl* f)
    {
        string func_name = f->getNameAsString();

        if (CXXRecordDecl* Class = dyn_cast<CXXRecordDecl>(f->getParent())) {
            string class_name = Class->getNameAsString();
            if (modules_to_process.count(class_name)) {
                if (!modules.count(class_name)) {
                    modules[class_name] = new DIR::Module(class_name);
                }
                curr_module = modules[class_name];
            } else {
                curr_module = nullptr;
            }
        } else {
            curr_module = nullptr;
        }
        return true;
    }

    //custom function, used for class function decls
    bool MyVisitFunctionDecl(FunctionDecl* f)
    {
        string func_name = f->getNameAsString();

        // cout << "function name: " << func_name << endl;

        // Check if this is a member function
        if (CXXRecordDecl* Class = dyn_cast<CXXRecordDecl>(f->getParent())) {
            string class_name = Class->getNameAsString();
            // cout
            // << "method " << func_name
            // << " belongs to class " << Class->getNameAsString() << endl;

            if (!modules.count(class_name)) {
                modules[class_name] = new DIR::Module(class_name);
            }
            curr_module = modules[class_name];

            // Get the number of parameters
            unsigned int num_params = f->getNumParams();

            vector<DIR::Type*> param_types;

            // Print the types of each parameter
            for (unsigned int i = 0; i < num_params; i++) {
                ParmVarDecl* param = f->getParamDecl(i);
                QualType type = param->getType();
                string param_type = cleanup_type(type.getAsString());
                string qualifiers = type_qualifiers(type.getAsString());
                // cout << "  argument " << i << " type: " << param_type << endl;
                add_dependency(param_type);
                param_types.push_back(new DIR::Type(param_type, qualifiers));
            }

            QualType QT = f->getReturnType();
            string return_type = cleanup_type(QT.getAsString());
            string qualifiers = type_qualifiers(QT.getAsString());
            // cout << "return type: " << return_type << endl;

            add_dependency(return_type);
            DIR::Type* return_full_type = new DIR::Type(return_type, qualifiers);

            //TODO: check this!
            AccessSpecifier access = f->getAccess();
            string accessSpecifierString;
            switch (access) {
            case AccessSpecifier::AS_public:
                accessSpecifierString = "+";
                break;
            case AccessSpecifier::AS_protected:
                accessSpecifierString = "=";
                break;
            case AccessSpecifier::AS_private:
                accessSpecifierString = "-";
                break;
            default:
                accessSpecifierString = "?";
                break;
            }

            // Get storage class specifier
            StorageClass storageClass = f->getStorageClass();
            string storageClassString;
            switch (storageClass) {
            case StorageClass::SC_Static:
                storageClassString = "$";
                break;
            default:
                storageClassString = "";
                break;
            }

            bool isVirtual = f->isVirtualAsWritten();

            string decorators = accessSpecifierString + storageClassString;
            if (isVirtual) {
                decorators += "@";
            }

            // cout << "Function " << func_name << " in class " << class_name << " has decorators " << decorators << endl;

            DIR::Method* method = new DIR::Method(decorators, return_full_type, func_name, param_types);

            curr_module->methods.push_back(method);
        } else {
            // This is not a member function
            // cout << "function " << f->getNameAsString()
            //  << " is not a member function" << endl;
            curr_module = nullptr;
        }
        // }
        // else {
        //     cout << "in function implementation" << endl;
        // }
        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl* Decl)
    {
        // if (!isInMainFile(Decl->getSourceRange().getBegin())) {
        //     cout << "AYO!!!! " << Decl->getNameAsString() << endl;
        //     return true;
        // }
        // if (Decl->getDefinition()) {
        string class_name = Decl->getNameAsString();

        if (!modules_to_process.count(class_name)) {
            curr_module = nullptr;
            return true;
        }

        // cout << "Found class: " << class_name << endl;
        ;
        if (!modules.count(class_name)) {
            modules[class_name] = new DIR::Module(class_name);
        }
        curr_module = modules[class_name];

        for (const auto& member : Decl->decls()) {
            if (CXXMethodDecl* methodDecl = dyn_cast<CXXMethodDecl>(member)) {
                if (methodDecl->isUserProvided()) {
                    FunctionDecl* funcDecl = methodDecl->getCanonicalDecl();
                    MyVisitFunctionDecl(funcDecl);
                }
            }
        }

        // Print the names of the parent classes
        for (auto& BaseSpec : Decl->bases()) {
            QualType BaseType = BaseSpec.getType();
            CXXRecordDecl* BaseDecl = BaseType->getAsCXXRecordDecl();
            string parent_name = BaseDecl->getNameAsString();
            // cout << "Parent class: " << parent_name << "\n";
            //TODO: inheriting from ignored classes
            DIR::Module* parent_module = modules[parent_name];
            modules[class_name]->parents.insert(parent_module);
        }

        // Iterate over the fields
        for (auto field = Decl->field_begin(); field != Decl->field_end(); ++field) {
            string field_type = cleanup_type(field->getType().getAsString());
            add_dependency(field_type);
        }
        // }
        return true;
    }

    bool VisitVarDecl(VarDecl* v)
    {
        // if (!isInMainFile(v->getSourceRange().getBegin()))
        //     return true;
        if (curr_module == nullptr) {
            return true;
        }
        // cout << "In variable decl: " << v->getNameAsString() << endl;
        QualType varType = v->getType();
        string var_type = cleanup_type(varType.getAsString());
        // cout << "Variable type: " << var_type << endl;
        add_dependency(var_type);
        if (v->hasInit()) {
            TraverseStmt(v->getInit());
        }
        return true;
    }

    bool VisitCXXConstructExpr(CXXConstructExpr* c)
    {
        // if (!isInMainFile(c->getSourceRange().getBegin()))
        //     return false;
        if (curr_module == nullptr) {
            return true;
        }

        CXXConstructorDecl* constructor = c->getConstructor();
        CXXRecordDecl* record = constructor->getParent();
        if (record) {
            string constructor_class = record->getNameAsString();
            // cout << "calling constructor of class " << constructor_class << endl;
            add_dependency(constructor_class);
        }
        return true;
    }

    bool VisitCXXMemberCallExpr(CXXMemberCallExpr* expr)
    {
        // if (!isInMainFile(expr->getSourceRange().getBegin()))
        //     return true;
        if (curr_module == nullptr) {
            return true;
        }
        // Get the method being called
        CXXMethodDecl* methodDecl = expr->getMethodDecl();

        // Get the parent class of the method
        CXXRecordDecl* classDecl = methodDecl->getParent();

        if (classDecl != nullptr) {
            // If the parent class is not null, print its name
            // cout << "Called method of class: " << classDecl->getNameAsString() << endl;
        }

        return true;
    }

    void add_dependency(string dependency_name)
    {
        if (curr_module == nullptr
            or curr_module->name == dependency_name
            or !modules.count(dependency_name)) {
            return;
        }

        // cout << "inserting dep on " << dependency_name << endl;
        curr_module->dependencies.insert(modules[dependency_name]);
    }

    // bool VisitStmt(Stmt* s)
    // {
    //     cout << "In statement: " << s->getStmtClassName() << endl;

    //     return true;
    // }

    DIR::Module* curr_module;
    map<string, DIR::Module*> modules;
    ASTContext* astContext;
    set<string> modules_to_process;
};

class MyASTConsumer : public ASTConsumer {
public:
    MyASTConsumer()
        : Visitor()
    {
    }

    virtual void HandleTranslationUnit(ASTContext& Context)
    {
        Visitor.init(Context, global_modules_to_process);
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        global_modules = Visitor.modules;
    }

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

map<string, DIR::Module*> parse(const char* filename, set<string> modules_to_process)
{
    global_modules_to_process = modules_to_process;
    llvm::cl::OptionCategory oc("my_option", "todo");
    int argc = 2;
    const char* argv[3] = { "bin/cppparser", filename, NULL };
    llvm::Expected<CommonOptionsParser> OptionsParser = CommonOptionsParser::create(argc, argv, oc);
    ClangTool Tool((*OptionsParser).getCompilations(),
        (*OptionsParser).getSourcePathList());
    unique_ptr<FrontendActionFactory> action_factory = newFrontendActionFactory<MyFrontendAction>();
    Tool.run(action_factory.get());
    return global_modules;
}

#endif