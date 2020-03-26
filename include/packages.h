#ifndef MATRIX_PACKAGES_H_
#define MATRIX_PACKAGES_H_

#include "../include/utils.h"

namespace matrix {
// options
void search_pkg(std::string);
void update_pkg_list();
void download_pkg(std::string);
void install_pkg(std::string);
void uninstall_pkg(std::string);
void query_pkg(std::string);
} // namespace matrix

#endif /* MATRIX_PACKAGES_H_ */
