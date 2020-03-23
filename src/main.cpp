#include <getopt.h>

#include "../include/utils.h"

int main (int argc, char **argv) {
	// cache & install
	matrix::initialize_dirs();
	
	std::string program_usage;
	program_usage += "Matrix AUR Helper\n\n";
	program_usage += "Usage: matrix [-dhirsu] [arg]\n\n";
	program_usage += "Help:\n";
	program_usage += "  -h, --help           \t Show this message\n";
	program_usage += "  -s, --search   <arg> \t Search for 'arg' in AUR\n";
	program_usage += "  -u, --update         \t Update packages list\n";
	program_usage += "  -d, --download <arg> \t Download the package especified\n";
	program_usage += "  -i, --install  <arg> \t Install the package especified\n";
	program_usage += "  -r, --remove   <arg> \t Uninstall the package especified\n";
	
	struct option opt_list[] = {
		{"help"    , no_argument      , NULL, 'h'},
		{"search"  , required_argument, NULL, 's'},
		{"update"  , no_argument      , NULL, 'u'},
		{"download", required_argument, NULL, 'd'},
		{"install" , required_argument, NULL, 'i'},
		{"remove"  , required_argument, NULL, 'r'}
	};
	
	int opt_handler = getopt_long(argc, argv, "d:hi:r:s:u", opt_list, NULL);
	
	switch (opt_handler) {
		case 'h':
			std::cout << program_usage << std::endl;
			break;
		case 's':
			matrix::search_pkg(optarg);
			break;
		case 'u':
			matrix::update_pkg_list();
			break;
		case 'd':
			matrix::download_pkg(optarg);
			break;
		case 'i':
			matrix::install_pkg(optarg);
			break;
		case 'r':
			matrix::uninstall_pkg(optarg);
			break;
	}
	
    return 0;
}
