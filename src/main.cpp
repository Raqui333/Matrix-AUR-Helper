#include <boost/program_options.hpp>

#include "../include/utils.h"

namespace opt = boost::program_options;

int main (int argc, char **argv) {
	matrix::initialize_dirs(); // cache & install
	
	std::string title;
	title += "Matrix AUR Helper\n\n";
	title += "Usage: matrix [-dhisu] [arg]\n\n";
	title += "Help";
	
	opt::options_description opt_list(title);
	opt::variables_map opt_map;

	opt_list.add_options()
		("help,h", "Show this message")
		("search,s", opt::value<std::string>(), "Search for 'arg' in AUR")
		("update,u","Update packages list")
		("download,d", opt::value<std::string>(), "Download the package especified")
		("install,i", opt::value<std::string>(), "Install the package especified")
		("remove,r", opt::value<std::string>(), "Uninstall the package especified")
	;
	
	try {
		opt::store(opt::parse_command_line(argc, argv, opt_list), opt_map);
		opt::notify(opt_map);
	} catch (opt::error& error) {
		std::cerr << error.what() << std::endl;
		return -1;
	}
	
	if (opt_map.count("help"))
		std::cout << opt_list << std::endl;
	
	if (opt_map.count("search"))
		matrix::search_pkg(opt_map["search"].as<std::string>());
	
	if (opt_map.count("update"))
		matrix::update_pkg_list();
	
	if (opt_map.count("download"))
		matrix::download_pkg(opt_map["download"].as<std::string>());
	
	if (opt_map.count("install"))
		matrix::install_pkg(opt_map["install"].as<std::string>());
	
	if (opt_map.count("remove"))
		matrix::uninstall_pkg(opt_map["remove"].as<std::string>());
	
    return 0;
}
