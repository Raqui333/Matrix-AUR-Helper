#include "../include/utils.h"

// directories
const std::string matrix::root_dir = fs::read_symlink("/proc/self/exe").parent_path();
const std::string matrix::cache_dir = matrix::root_dir + "/cache";
const std::string matrix::install_dir = matrix::root_dir + "/install";

const std::string matrix::AUR = "https://aur.archlinux.org";

void matrix::run_command(const char **args) {
	pid_t pid;
	int exit_code = 0;
	
	if ((pid = fork()) < 0)
		std::cerr << "forking child process failed\n";
	else if (pid == 0) {
		if (execvp(args[0], const_cast<char **const>(args)) < 0)
			std::cerr << "executing command failed\n";
	} else {
		while(wait(&exit_code) != pid);
		if (WEXITSTATUS(exit_code) != 0 && std::string(args[0]) != "pacman") // ignore exit case with pacman search
			exit(WEXITSTATUS(exit_code));
	}
}

bool matrix::path_exists(std::string path) {
	return (fs::exists(fs::path(path)));
}

void matrix::cache_clear() {
	fs::remove_all(fs::path(matrix::cache_dir));
	matrix::initialize_dirs();
}

// init
void matrix::initialize_dirs() {
	if (!fs::is_directory(fs::path(cache_dir)))	fs::create_directory(cache_dir);

	if (!fs::is_directory(fs::path(install_dir))) fs::create_directory(install_dir);
}
