#include "../include/utils.h"

int main (int argc, char **argv) {
	const std::string option = (argv[1]) ? std::string(argv[1]) : "";
	
	// cache & install
	matrix::initialize_dirs();
	
	std::string opt_title;
	opt_title += "Matrix AUR Helper\n\n";
	opt_title += "Usage: matrix [-dhirsu] [arg]\n\n";
	opt_title += "Help:\n";
	
	std::string opt_list;
	opt_list += "  -h, --help           \t Show this message\n";
	opt_list += "  -s, --search   <arg> \t Search for 'arg' in AUR\n";
	opt_list += "  -u, --update         \t Update packages list\n";
	opt_list += "  -d, --download <arg> \t Download the package especified\n";
	opt_list += "  -i, --install  <arg> \t Install the package especified\n";
	opt_list += "  -r, --remove   <arg> \t Uninstall the package especified\n";
	
	std::map<std::string, std::pair<std::string, std::string>> opt_map = {
		{"help"    , {"--help"    , "-h"}},
		{"search"  , {"--search"  , "-s"}},
		{"update"  , {"--update"  , "-u"}},
		{"download", {"--download", "-d"}},
		{"install" , {"--install" , "-i"}},
		{"remove"  , {"--remove"  , "-r"}},
	};
	
	auto check_option = [&option](auto opt) {
		return (option == opt.first || option == opt.second);
	};
	
	try {
	
		// handler options
		if (check_option(opt_map["help"]))
		{
			std::cout << opt_title << opt_list << std::endl;
		}
		else if (check_option(opt_map["search"]))
		{
			matrix::search_pkg(argv[2]);
		}
		else if (check_option(opt_map["update"]))
		{
			matrix::update_pkg_list();
		}
		else if (check_option(opt_map["download"]))
		{
			matrix::download_pkg(argv[2]);
		}
		else if (check_option(opt_map["install"]))
		{
			matrix::install_pkg(argv[2]);
		}
		else if (check_option(opt_map["remove"]))
		{
			matrix::uninstall_pkg(argv[2]);
		}
		
	} catch (std::logic_error) {
		std::cout << "\033[1;31m::\033[00m invalid argumment\n";
	} catch (std::exception& e) {
		std::cout << "error: " << e.what() << std::endl;
	}
	
    return 0;
}
