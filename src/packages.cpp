#include "../include/packages.h"

// options
void matrix::search_pkg(std::string name) {
	if (!matrix::path_exists(matrix::cache_dir + "/packages.mah"))
		matrix::update_pkg_list();
	
	// pacman legacy
	const char *pacman_search[] = {"pacman", "-S", "-s", "-q", name.c_str(), NULL};
	
	std::cout << "\033[1;37mPacman packages:\n\033[1;34m" << std::flush;
	matrix::run_command(pacman_search);
	
	// aur
	std::cout << "\n\033[1;37mAUR packages:\n" << std::flush;
	
	std::ifstream pkgs_list(matrix::cache_dir + "/packages.mah");
	
	std::string line;
	
	if (pkgs_list.is_open()) {
		while(pkgs_list >> line) {
			if(std::regex_search(line, std::regex(name))) {
				if (matrix::path_exists(matrix::install_dir + "/" + line)) {
					line += "\033[1;37m (installed)";
				}
				std::cout << "\033[1;32m" << line << "\033[00m\n";
			}
		}
		pkgs_list.close();
	}
}

void matrix::update_pkg_list() {
	const std::string url = matrix::AUR + "/packages.gz";
	const std::string output_file = matrix::cache_dir + "/packages.mah.gz";
	
	const char *download_aur_pkg_list[] = {"curl", url.c_str(), "-o", output_file.c_str(), NULL};
	const char *unzip_aur_pkg_list[] = {"gzip", "-d", "-f", "-v", output_file.c_str(), NULL};
	
	std::cout << "\033[1;32m:: \033[1;38mSyncing packages list...\033[00m\n";
	matrix::run_command(download_aur_pkg_list);
	
	std::cout << "\033[1;32m:: \033[1;38mDescrompessing...\033[00m\n";
	matrix::run_command(unzip_aur_pkg_list);
	
	std::cout << "\033[1;32m:: \033[1;38mSyncing completed\033[00m\n";
}

void matrix::download_pkg(std::string name) {
	const std::string url = matrix::AUR + "/" + name + ".git";
	const std::string download_dir = matrix::cache_dir + "/" + name;
	
	const char *download_aur_pkg[] = {"git", "clone", "-q", url.c_str(), download_dir.c_str(), NULL};

	if (!matrix::path_exists(matrix::cache_dir + "/packages.mah"))
		matrix::update_pkg_list();
		
	if (matrix::path_exists(download_dir)) {
		std::cout << "package \033[1;32m" << name << "\033[00m already downloaded\n";
		return;
	}
	
	std::cout << "\033[1;32m:: \033[1;38mDownloading \033[1;32m" << name << "\033[1;38m please wait...\033[00m\n";
	
	int exit_code = 0;
	
	std::ifstream pkg_list(matrix::cache_dir + "/packages.mah");
	
	std::string line;
	
	if (pkg_list.is_open()) {
		while (pkg_list >> line) {
			if (std::regex_match(line, std::regex(name))) {
				matrix::run_command(download_aur_pkg);
				break;
			}
		}
		
		if (pkg_list.eof())
				exit_code = 1;
		
		pkg_list.close();
	}
	
	if (exit_code == 0)
		std::cout << "\033[1;32m:: \033[1;38mDownload finished\033[00m\n";
	else
		std::cout << "\033[1;32m:: \033[1;31mDownload failed!\033[00m\n";
}

void matrix::install_pkg(std::string name) {
	const std::string pkg_cache_location = matrix::cache_dir + "/" + name;
	const std::string pkg_install_location = matrix::install_dir + "/" + name;
	
	const char *install_aur_pkg[] = {"makepkg", "-s", "-i", NULL};

	if (matrix::path_exists(pkg_install_location)) {
		std::cout << "package \033[1;32m" << name << "\033[00m already installed\n";
		return;
	}
	
	if (!matrix::path_exists(pkg_cache_location))
		matrix::download_pkg(name);
	
	std::cout << "\033[1;32m:: \033[1;38mInstalling \033[1;32m" << name << "\033[1;38m please wait...\033[00m\n";
	chdir(pkg_cache_location.c_str());
	matrix::run_command(install_aur_pkg);
	
	std::cout << "\033[1;32m:: \033[1;38mFinishing installation...\033[00m\n";
	fs::copy(fs::path(pkg_cache_location), fs::path(pkg_install_location));
	fs::remove_all(fs::path(pkg_cache_location));
	
	std::cout << "\033[1;32m:: \033[1;38mInstall finished\033[00m\n";
}

void matrix::uninstall_pkg(std::string name) {
	const std::string installed_pkg = matrix::install_dir + "/" + name;
	const char *uninstall_pkg[] = {"sudo", "pacman", "-R", "-s", "-n", name.c_str(), NULL};
	
	std::cout << "\033[1;32m:: \033[1;38mUninstalling \033[1;32m" << name << "\033[1;38m please wait...\033[00m\n";
	matrix::run_command(uninstall_pkg);
	
	if (matrix::path_exists(installed_pkg))
		fs::remove_all(fs::path(installed_pkg));
	
	std::cout << "\033[1;32m:: \033[1;38mUninstall completed\033[00m\n";
}
