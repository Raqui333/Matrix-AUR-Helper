#include "../include/utils.h"

// directories
const std::string matrix::root_dir = fs::read_symlink("/proc/self/exe").parent_path();
const std::string matrix::cache_dir = "cache";
const std::string matrix::install_dir = "install";

// alias
const std::string root_cache_dir = matrix::root_dir + "/" + matrix::cache_dir;
const std::string root_install_dir = matrix::root_dir + "/" + matrix::install_dir;

const std::string matrix::AUR = "https://aur.archlinux.org";

void matrix::run_command(const char **args) {
	pid_t pid;
	int exit_code = 0;
	
	if ((pid = fork()) < 0)
		std::cerr << "forking child process failed\n";
	else if (pid == 0) {
		if (execvp(args[0], const_cast<char **const>(args)) < 0)
			std::cerr << "executing command failed\n";
	} else
		while (wait(&exit_code) != pid);
			if (exit_code != 0)
				exit(-1);
}

// ignore exit codes (pacman legacy)
void matrix::run_command(const char **args, bool) {
	pid_t pid;
	int exit_code = 0;
	
	if ((pid = fork()) < 0)
		std::cerr << "forking child process failed\n";
	else if (pid == 0) {
		if (execvp(args[0], const_cast<char **const>(args)) < 0)
			std::cerr << "executing command failed\n";
	} else
		while (wait(&exit_code) != pid);
}

// options functions
void matrix::search_pkg(std::string str) {
	if (!fs::exists(fs::path(root_cache_dir + "/packages.mah")))
		matrix::update_pkg_list();
	
	// pacman legacy
	std::cout << "\033[1;37mPacman packages:\n\033[1;34m" << std::flush;
	
	const char *pacman_search[] = {"pacman", "-S", "-s", "-q", str.c_str(), NULL};
	matrix::run_command(pacman_search, true);
	
	// aur
	std::cout << "\n\033[1;37mAUR packages:\n" << std::flush;
	
	std::regex reg (".*" + str + ".*");
	
	std::ifstream pkgs_list(root_cache_dir + "/packages.mah");
	
	std::string line;
	if (pkgs_list.is_open()) {
		while(pkgs_list >> line)
			if(std::regex_match(line, reg)) {
				std::cout << "\033[1;32m" << line;
				
				if (fs::exists(fs::path(root_install_dir + "/" + line)))
					std::cout << "\033[1;37m (installed)\033[00m\n";
				else
					std::cout << "\033[00m\n";
			}
		pkgs_list.close();
	}
}

void matrix::update_pkg_list() {
	const std::string url = matrix::AUR + "/packages.gz";
	const std::string list_dir = root_cache_dir + "/packages.mah.gz";
	
	const char *download_aur_pkg_list[] = {"curl", "--create-dirs", url.c_str(), "-o", list_dir.c_str(), NULL};
	const char *unzip_aur_pkg_list[] = {"gzip", "-d", "-f", "-v", list_dir.c_str(), NULL};
	
	std::cout << "\033[1;32m:: \033[1;37mSyncing packages list...\033[00m\n";
	matrix::run_command(download_aur_pkg_list);
	
	std::cout << "\033[1;32m:: \033[1;37mDescrompessing...\033[00m\n";
	matrix::run_command(unzip_aur_pkg_list);
	
	std::cout << "\033[1;32m:: \033[1;37mUpdate finished\033[00m\n";
}

void matrix::download_pkg(std::string pkg_name) {
	const std::string url = matrix::AUR + "/" + pkg_name + ".git";
	const std::string download_dir = root_cache_dir + "/" + pkg_name;
	
	const char *download_aur_pkg[] = {"git", "clone", "-q", url.c_str(), download_dir.c_str(), NULL};

	if (fs::exists(fs::path(download_dir))) {
		std::cout << "package \033[1;32m" << pkg_name << "\033[00m already downloaded\n";
		return;
	}
	
	std::cout << "\033[1;32m:: \033[1;37mDownloading \033[1;32m" << pkg_name << "\033[1;37m please wait...\033[00m\n";
	
	std::ifstream pkg_list(root_cache_dir + "/packages.mah");
	
	std::string line;
	int exit_code = 0;
	
	if (pkg_list.is_open()) {
		while (pkg_list >> line)
			if (std::regex_match(line, std::regex("\\b" + pkg_name + "\\b"))) {
				matrix::run_command(download_aur_pkg);
				break;
			}
		
		if (pkg_list.eof())
				exit_code = 1;
		
		pkg_list.close();
	}
	
	if (exit_code == 0)
		std::cout << "\033[1;32m:: \033[1;37mDownload finished\033[00m\n";
	else
		std::cout << "\033[1;32m:: \033[1;31mDownload failed!\033[00m\n";
}

void matrix::install_pkg(std::string pkg_name) {
	const std::string pkg_current_location = root_cache_dir + "/" + pkg_name;
	const std::string pkg_install_location = root_install_dir + "/" + pkg_name;
	
	const char *install_aur_pkg[] = {"makepkg", "-s", "-i", NULL};

	if (!fs::exists(fs::path(pkg_current_location)))
		matrix::download_pkg(pkg_name);
	
	if (fs::exists(fs::path(pkg_install_location))) {
		std::cout << "package \033[1;32m" << pkg_name << "\033[00m already installed\n";
		return;
	}
	
	std::cout << "\033[1;32m:: \033[1;37mInstalling \033[1;32m" << pkg_name << "\033[1;37m please wait...\033[00m\n";
	chdir(pkg_current_location.c_str());
	matrix::run_command(install_aur_pkg);
	
	std::cout << "\033[1;32m:: \033[1;37mInstall finished\033[00m\n";
	fs::copy(fs::path(pkg_current_location), fs::path(pkg_install_location));
	fs::remove_all(fs::path(pkg_current_location));
}

void matrix::uninstall_pkg(std::string pkg_name) {
	const std::string installed_pkg = root_install_dir + "/" + pkg_name;
	const char *uninstall_pkg[] = {"sudo", "pacman", "-R", "-s", "-n", pkg_name.c_str(), NULL};
	
	std::cout << "\033[1;32m:: \033[1;37mUninstalling \033[1;32m" << pkg_name << "\033[1;37m please wait...\033[00m\n";
	matrix::run_command(uninstall_pkg);
	
	if (fs::exists(fs::path(installed_pkg)))
		fs::remove_all(fs::path(installed_pkg));
	
	std::cout << "\033[1;32m:: \033[1;37mUninstall completed\033[00m\n";
}

// init
void matrix::initialize_dirs() {
	if (!fs::is_directory(fs::path(root_cache_dir)))
		fs::create_directory(root_cache_dir);
		
	if (!fs::is_directory(fs::path(root_install_dir)))
		fs::create_directory(root_install_dir);
}
