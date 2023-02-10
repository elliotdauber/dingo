#include <cassert>
#include <cctype>
#include <fstream>

#include "orchestrator.hh"
#include "visitor.hh"

DSN::Orchestrator::~Orchestrator()
{
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
}

void DSN::Orchestrator::parse(const char* const filename)
{
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    parse_helper(in_file);
}

void DSN::Orchestrator::parse(std::istream& stream)
{
    if (!stream.good() && stream.eof()) {
        return;
    }
    parse_helper(stream);
}

void DSN::Orchestrator::parse_helper(std::istream& stream)
{

    delete (scanner);
    try {
        scanner = new Scanner(&stream);
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try {
        parser = new Parser((*scanner) /* scanner */,
            (*this) /* orchestrator */);
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept) {
        std::cerr << "Parse failed!!\n";
    } else {
        cout << "parsed successfully!" << endl;
    }
    return;
}

std::ostream&
DSN::Orchestrator::print(std::ostream& stream)
{
    // Visitor *v = new PrintingVisitor(stream);
    // NodeGenVisitor* nodegen = new NodeGenVisitor(stream);
    // program->accept(nodegen);

    // EdgeGenVisitor* edgegen = new EdgeGenVisitor(stream);
    // program->accept(edgegen);

    LoweringVisitor* lowerer = new LoweringVisitor(stream);
    program->accept(lowerer);
    delete lowerer;
    // delete (nodegen);
    // delete (edgegen);

    return stream;
}

void DSN::Orchestrator::add_module_decl(ModuleDecl* decl)
{
    program->add_module_decl(decl);
}

void DSN::Orchestrator::add_pattern_defn(PatternDefinition* defn)
{
    program->add_pattern_defn(defn);
}
void DSN::Orchestrator::add_pattern_appl(PatternApplication* appl)
{
    program->add_pattern_appl(appl);
}