#ifndef MATRIX_UTILS_H_
#define MATRIX_UTILS_H_

#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include <fstream>
#include <map>
#include <iomanip>

namespace fs = std::filesystem;

namespace matrix {
	// directories
	extern const std::string root_dir;
	extern const std::string cache_dir;
	extern const std::string install_dir;
	
	extern const std::string AUR;
	
	void run_command(const char**);
	bool path_exists(std::string);
	void cache_clear();
	
	// init
	void initialize_dirs();
}

#endif
