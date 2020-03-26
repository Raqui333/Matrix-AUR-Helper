#ifndef MATRIX_UTILS_H_
#define MATRIX_UTILS_H_

#include <sys/wait.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <string>

namespace fs = std::filesystem;

namespace matrix {
// directories
extern const std::string root_dir;
extern const std::string cache_dir;
extern const std::string install_dir;

extern const std::string AUR;

// utils
void run_command(const char **);
void cache_clear();
bool path_exists(std::string);

// init
void initialize_dirs();
} // namespace matrix

#endif /* MATRIX_UTILS_H_ */
