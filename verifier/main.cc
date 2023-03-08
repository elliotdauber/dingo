#include "dir.hh"
#include "orchestrator.hh"
#include "parse.hh"
#include <iostream>

#include "log.hh"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "print help message")("yaml,y", po::value<string>(), "yaml config file")("evaluate,e", po::value<bool>()->default_value(true), "evaulate source code (0, 1)")("interface,i", po::value<string>()->default_value("public"), "interface (public, private, all, none)")("view,v", po::value<string>()->default_value("ideal"), "view mode (ideal, real, none)")("patterns,p", po::value<bool>()->default_value(false), "show patterns (0, 1)")("output,o", po::value<string>()->default_value("output.png"), "output png filepath")("label,l", po::value<string>()->default_value(""), "Graph label")("dingofile,d", po::value<string>(), "Dingofile filepath")("source-file,s", po::value<string>()->default_value(""), "Source filepath");

    po::positional_options_description p;
    // p.add("dingofile", 1);
    // p.add("source-file", 1);

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
    bool evaluate_source = vm["evaluate"].as<bool>();

    DIR::GraphContext context = DIR::GraphContext();
    context.label = vm["label"].as<string>();
    context.interface = vm["interface"].as<string>();
    context.view_mode = vm["view"].as<string>();
    context.patterns = vm["patterns"].as<bool>();

    if (!evaluate_source and context.view_mode != "ideal") {
        cerr << "Cannot use -v <real, none> with -e 0, exiting" << endl;
        exit(1);
    }

    cout << "*----------------------------------*" << endl;
    cout << "Interface: " << context.interface << "\n";
    cout << "View mode: " << context.view_mode << "\n";
    cout << "Show patterns: " << context.patterns << "\n";
    cout << "Evaluate: " << evaluate_source << "\n";
    cout << "Output file: " << output_file << "\n";
    cout << "Label: " << context.label << "\n";
    cout << "Dingofile: " << dingofile << "\n";
    cout << "Source file: " << source_file << "\n";
    cout << "*----------------------------------*" << endl
         << endl;

    //compile the Dingofile
    DSN::Orchestrator orchestrator;
    orchestrator.parse(dingofile);
    orchestrator.lower();
    map<string, DIR::Module*> target_modules = orchestrator.get_modules();

    DIR::Verifier v;
    if (!evaluate_source and context.view_mode == "ideal") {
        v.generate_graph_png(target_modules, context, output_file);
        return 0;
    }

    set<string> modules_to_process;
    for (auto it = target_modules.begin(); it != target_modules.end(); ++it) {
        modules_to_process.insert(it->first);
    }

    map<string, DIR::Module*> tester_modules = parse(source_file, modules_to_process);

    if (v.do_modules_conform(target_modules, tester_modules)) {
        Logger(SUCCESS) << "Success: everything looks good!\n";
    } else {
        Logger(ERROR) << "Failure: check the errors\n";
    }

    if (context.view_mode == "ideal") {
        v.generate_graph_png(target_modules, context, output_file);
    } else if (context.view_mode == "real") {
        v.generate_graph_png(tester_modules, context, output_file);
    } else if (context.view_mode != "none") {
        Logger(WARNING) << "Warning: Unrecognized option set for --view, not generating a graph\n";
    }
    return 0;
}