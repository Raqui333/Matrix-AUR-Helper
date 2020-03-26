#include "../include/packages.h"

// options
void matrix::search_pkg(std::string name) {
    if (!matrix::path_exists(matrix::cache_dir + "/packages.mah"))
        matrix::update_pkg_list();

    // pacman legacy
    const char *pacman_search[] = {"pacman", "-S",         "-s",
                                   "-q",     name.c_str(), NULL};

    std::cout << "\033[1;37mPacman packages:\n\033[1;34m" << std::flush;
    matrix::run_command(pacman_search);

    // aur
    std::cout << "\n\033[1;37mAUR packages:\n" << std::flush;

    std::ifstream pkgs_list(matrix::cache_dir + "/packages.mah");

    if (pkgs_list.is_open()) {
        std::string line;
        while (pkgs_list >> line) {
            if (std::regex_search(line, std::regex(name))) {
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
    const std::string url         = matrix::AUR + "/packages.gz";
    const std::string output_file = matrix::cache_dir + "/packages.mah.gz";

    const char *download_aur_pkg_list[] = {"curl", url.c_str(), "-o",
                                           output_file.c_str(), NULL};

    const char *unzip_aur_pkg_list[] = {
        "gzip", "-d", "-f", "-v", output_file.c_str(), NULL};

    std::cout << "\033[1;32m:: \033[1;38mSyncing packages list...\033[00m\n";
    matrix::run_command(download_aur_pkg_list);

    std::cout << "\033[1;32m:: \033[1;38mDescrompessing...\033[00m\n";
    matrix::run_command(unzip_aur_pkg_list);

    std::cout << "\033[1;32m:: \033[1;38mSyncing completed\033[00m\n";
}

void matrix::download_pkg(std::string name) {
    const std::string url          = matrix::AUR + "/" + name + ".git";
    const std::string download_dir = matrix::cache_dir + "/" + name;

    const char *download_aur_pkg[] = {
        "git", "clone", "-q", url.c_str(), download_dir.c_str(), NULL};

    if (!matrix::path_exists(matrix::cache_dir + "/packages.mah"))
        matrix::update_pkg_list();

    if (matrix::path_exists(download_dir)) {
        std::cout << "package \033[1;32m" << name
                  << "\033[00m already downloaded\n";

        return;
    }

    std::cout << "\033[1;32m:: \033[1;38mDownloading \033[1;32m" << name
              << "\033[1;38m please wait...\033[00m\n";

    int exit_code = 0;

    std::ifstream pkg_list(matrix::cache_dir + "/packages.mah");

    if (pkg_list.is_open()) {
        std::string line;
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

    if (exit_code == 0) {
        std::cout << "\033[1;32m:: \033[1;38mDownload finished\033[00m\n";
    } else {
        std::cout << "\033[1;32m:: \033[1;31mPackage does not exist!\033[00m\n";
        exit(-1);
    }
}

void matrix::install_pkg(std::string name) {
    const std::string pkg_in_cache   = matrix::cache_dir + "/" + name;
    const std::string pkg_in_install = matrix::install_dir + "/" + name;

    const char *install_aur_pkg[] = {"makepkg", "-s", "-i", NULL};

    if (matrix::path_exists(pkg_in_install)) {
        std::cout << "package \033[1;32m" << name
                  << "\033[00m already installed\n";
        return;
    }

    if (!matrix::path_exists(pkg_in_cache))
        matrix::download_pkg(name);

    std::cout << "\033[1;32m:: \033[1;38mInstalling \033[1;32m" << name
              << "\033[1;38m please wait...\033[00m\n";

    chdir(pkg_in_cache.c_str());
    matrix::run_command(install_aur_pkg);

    std::cout << "\033[1;32m:: \033[1;38mFinishing installation...\033[00m\n";

    fs::copy(fs::path(pkg_in_cache), fs::path(pkg_in_install));
    fs::remove_all(fs::path(pkg_in_cache));

    std::cout << "\033[1;32m:: \033[1;38mInstall finished\033[00m\n";
}

void matrix::uninstall_pkg(std::string name) {
    const std::string pkg_in_install = matrix::install_dir + "/" + name;

    const char *uninstall_pkg[] = {"sudo", "pacman",     "-R", "-s",
                                   "-n",   name.c_str(), NULL};

    std::cout << "\033[1;32m:: \033[1;38mUninstalling \033[1;32m" << name
              << "\033[1;38m please wait...\033[00m\n";

    matrix::run_command(uninstall_pkg);

    if (matrix::path_exists(pkg_in_install))
        fs::remove_all(fs::path(pkg_in_install));

    std::cout << "\033[1;32m:: \033[1;38mUninstall completed\033[00m\n";
}

void matrix::query_pkg(std::string name) {
    const std::string pkg_in_cache   = matrix::cache_dir + "/" + name;
    const std::string pkg_in_install = matrix::install_dir + "/" + name;

    auto query_it = [&name](auto query_file) {
        std::cout << "\033[1;32m::\033[1;38m Info about \033[4;32m" << name
                  << "\033[0;1;38m package:\n\n";

        std::ifstream file(query_file + "/PKGBUILD");

        if (file.is_open()) {
            // clang-format off
            std::map<std::string, std::string> info_item = {
                {"pkgname", "Name"        },
                {"pkgver" , "Version"     },
                {"pkgdesc", "Description" },
                {"url"    , "URL"         },
                {"arch"   , "Architecture"},
                {"license", "Licenses"    },
                {"depends", "Depends On"  },
            };
            // clang-format on

            std::regex reg("^(pkg(name|ver|desc)|license|url|depends|arch)=");

            std::string line;
            while (std::getline(file, line)) {
                if (std::regex_search(line, reg)) {
                    std::size_t delim = line.find('=');

                    std::string first_field = line.substr(0, delim);
                    std::string second_field =
                        line.substr(delim + 1, line.length());

                    std::cout << "\t\033[1;32m" << std::left << std::setw(15)
                              << info_item[first_field] << "\033[00m "
                              << second_field << "\n";
                }
            }
            file.close();
        }
    };

    if (matrix::path_exists(pkg_in_install)) {
        query_it(pkg_in_install);
        std::cout << "\n\t\033[1;32m::\033[1;38m Package is installed\n\n";
    } else if (matrix::path_exists(pkg_in_cache)) {
        query_it(pkg_in_cache);
        std::cout << "\n\t\033[1;32m::\033[1;38m Package is not installed\n\n";
    } else {
        matrix::download_pkg(name);
        matrix::query_pkg(name);
    }
}
