#include "parse.hh"

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
        } else if (type.find("enum ") == 0) {
            type = type.substr(5);
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

    // regex pattern("<.*?>");
    // return regex_replace(type, pattern, "");
    return type;
}

vector<string> get_all_types(string type)
{
    while (true) {
        if (type.find("class ") == 0) {
            type = type.substr(6);
        } else if (type.find("struct ") == 0) {
            type = type.substr(7);
        } else if (type.find("const ") == 0) {
            type = type.substr(6);
        } else if (type.find("enum ") == 0) {
            type = type.substr(5);
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

    size_t pos = type.find("<");
    // cout << "Here " << type << endl;
    if (pos == string::npos) {
        return { type };
    }
    // cout << "in template type: " << type << endl;

    // Extract class name
    string class_name = type.substr(0, pos);

    // Extract template parameters
    size_t end_pos = type.find_last_of(">");
    if (end_pos == string::npos) {
        return {};
    }

    vector<string> all_types { class_name };

    string params_string = type.substr(pos + 1, end_pos - pos - 1);

    // Split parameter string by commas
    stringstream ss(params_string);
    string param;
    while (getline(ss, param, ',')) {
        // Remove any leading or trailing whitespace
        param.erase(0, param.find_first_not_of(' '));
        param.erase(param.find_last_not_of(' ') + 1);
        vector<string> types = get_all_types(param);
        for (string t : types) {
            all_types.push_back(t);
        }
    }
    return all_types;
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

void DingoCppASTVisitor::init(ASTContext& context, set<string> mods_to_process)
{
    modules_to_process = mods_to_process;
    astContext = &context;
}

bool DingoCppASTVisitor::isInMainFile(SourceLocation loc)
{
    SourceManager& SM = astContext->getSourceManager();
    FileID mainFileID = SM.getMainFileID();
    FileID locFileID = SM.getFileID(loc);
    return mainFileID == locFileID;
}

bool DingoCppASTVisitor::VisitNamespaceDecl(NamespaceDecl* nd)
{
    string ns = nd->getName().str();
    cout << ns << endl;
    curr_namespace = ns + "::";
    return true;
}

//used for out-of-class function decls
bool DingoCppASTVisitor::VisitFunctionDecl(FunctionDecl* f)
{
    string func_name = f->getNameAsString();

    if (CXXRecordDecl* Class = dyn_cast<CXXRecordDecl>(f->getParent())) {
        string class_name = Class->getNameAsString();
        // cout << class_name << "::" << func_name << endl;
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

bool DingoCppASTVisitor::VisitDeclRefExpr(DeclRefExpr* expr)
{
    //TODO: finish this (e.g. raft Server class should see Message*)
    QualType type = expr->getType();
    add_dependency(type.getAsString());
    return true;
}

//custom function, used for class function decls
bool DingoCppASTVisitor::MyVisitFunctionDecl(FunctionDecl* f)
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
            add_dependency(type.getAsString());
            param_types.push_back(new DIR::Type(param_type, qualifiers));
        }

        QualType QT = f->getReturnType();
        string return_type = cleanup_type(QT.getAsString());
        string qualifiers = type_qualifiers(QT.getAsString());
        // cout << "return type: " << return_type << endl;

        add_dependency(QT.getAsString());
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

        if (CXXMethodDecl* method = dyn_cast<CXXMethodDecl>(f)) {
            if (method->size_overridden_methods() > 0) {
                decorators += "#";
            }
        }

        // cout << "Function " << func_name << " in class " << class_name << " has decorators " << decorators << endl;

        DIR::Method* method = new DIR::Method(decorators, return_full_type, func_name, param_types);

        curr_module->methods.push_back(method);
    } else {
        // This is not a member function
        curr_module = nullptr;
    }
    return true;
}

bool DingoCppASTVisitor::VisitCXXRecordDecl(CXXRecordDecl* Decl)
{
    string class_name = Decl->getNameAsString();
    cout << class_name << endl;
    if (!modules_to_process.count(class_name)) {
        curr_module = nullptr;
        return true;
    }

    // cout << class_name << endl;

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
        // if (class_name == "NetworkInterface") {
        //     cout << field->getType().getAsString() << endl;
        //     cout << field->getNameAsString() << endl;
        //     cout << field_type << endl;
        // }
        add_dependency(field->getType().getAsString());
    }
    return true;
}

bool DingoCppASTVisitor::VisitVarDecl(VarDecl* v)
{
    if (curr_module == nullptr) {
        return true;
    }
    QualType varType = v->getType();
    string var_type = cleanup_type(varType.getAsString());
    // cout << "Variable type: " << var_type << endl;
    cout << "In variable decl: " << var_type << endl;
    add_dependency(var_type);
    if (v->hasInit()) {
        TraverseStmt(v->getInit());
    }
    return true;
}

bool DingoCppASTVisitor::VisitCXXConstructExpr(CXXConstructExpr* c)
{
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

bool DingoCppASTVisitor::VisitCXXMemberCallExpr(CXXMemberCallExpr* expr)
{
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

void DingoCppASTVisitor::add_dependency(string dependency_name)
{
    if (curr_module == nullptr
        or curr_module->name == dependency_name) {
        return;
    }

    vector<string> all_types = get_all_types(dependency_name);
    for (string type : all_types) {
        if (modules.count(type)) {
            curr_module->dependencies.insert(modules[type]);
        }
    }
}

void DingoCppASTConsumer::HandleTranslationUnit(ASTContext& Context)
{
    Visitor.init(Context, global_modules_to_process);
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    global_modules = Visitor.modules;
}

map<string, DIR::Module*> parse(string filename, set<string> modules_to_process)
{
    global_modules_to_process = modules_to_process;
    llvm::cl::OptionCategory oc("todo", "todo");
    int argc = 2;
    const char* argv[3] = { "bin/cppparser", filename.c_str(), NULL };
    llvm::Expected<CommonOptionsParser> OptionsParser = CommonOptionsParser::create(argc, argv, oc);
    ClangTool Tool((*OptionsParser).getCompilations(),
        (*OptionsParser).getSourcePathList());
    unique_ptr<FrontendActionFactory> action_factory = newFrontendActionFactory<DingoCppFrontendAction>();
    Tool.run(action_factory.get());
    return global_modules;
}