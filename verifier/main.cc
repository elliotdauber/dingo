#include "dir.hh"
#include "orchestrator.hh"
#include "parse.hh"
#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "print help message")("view,v", po::value<string>()->default_value("public"), "view mode (public, private, all, none)")("output,o", po::value<string>()->default_value("output.png"), "output png filepath")("label,l", po::value<string>()->default_value(""), "Graph label")("dingofile,d", po::value<string>(), "Dingofile filepath")("source-file,s", po::value<string>(), "Source filepath");

    po::positional_options_description p;
    p.add("dingofile", 1);
    p.add("source-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    string output_file = vm["output"].as<string>();
    string dingofile = vm["dingofile"].as<string>();
    string source_file = vm["source-file"].as<string>();

    DIR::GraphContext context = DIR::GraphContext();
    context.label = vm["label"].as<string>();
    context.view_mode = vm["view"].as<string>();

    cout << "View mode: " << context.view_mode << "\n";
    cout << "Output file: " << output_file << "\n";
    cout << "Label: " << context.label << "\n";
    cout << "Dingofile: " << dingofile << "\n";
    cout << "Source file: " << source_file << "\n";

    //compile the .dsn file
    DSN::Orchestrator orchestrator;
    orchestrator.parse(dingofile);
    orchestrator.lower();
    map<string, DIR::Module*> target_modules = orchestrator.get_modules();

    set<string> modules_to_process;
    for (auto it = target_modules.begin(); it != target_modules.end(); ++it) {
        modules_to_process.insert(it->first);
    }

    map<string, DIR::Module*> tester_modules = parse(source_file, modules_to_process);

    DIR::Verifier v;
    if (v.do_modules_conform(target_modules, tester_modules)) {
        cout << "Success: everything looks good!" << endl;
    } else {
        cout << "Failure: check the errors" << endl;
    }

    // string dingofile_prefix = "verifier_dingofile";
    // string codefile_prefix = "verifier_codefile";
    // v.generate_graph_png(target_modules, dingofile_prefix);
    // v.generate_graph_png(tester_modules, codefile_prefix);

    v.generate_graph_png(tester_modules, context, output_file);
    return 0;
}