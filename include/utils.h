#ifndef MATRIX_UTILS_H_
#define MATRIX_UTILS_H_

#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace matrix {
	// directories
	extern const std::string root_dir;
	extern const std::string cache_dir;
	extern const std::string install_dir;
	
	extern const std::string AUR;
	
	void run_command(const char**);
	void run_command(const char**, bool); // ignore exit codes
	
	// options functions
	void search_pkg(std::string);
	void update_pkg_list();
	void download_pkg(std::string);
	void install_pkg(std::string);
	void uninstall_pkg(std::string);
	
	// init
	void initialize_dirs();
}

#endif
