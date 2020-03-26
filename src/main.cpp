#include <getopt.h>

#include "../include/packages.h"

int main(int argc, char **argv) {
    const std::string version = "v1.3";

    // cache & install
    matrix::initialize_dirs();

    std::string usage_str =
        "Matrix AUR Helper\n\n"
        "Usage: matrix [-dhiqrsu] [arg]\n\n"
        "Help:\n"
        "  -h, --help           \t Show this message\n"
        "  -s, --search   <arg> \t Search for 'arg' in AUR\n"
        "  -u, --update         \t Update packages list\n"
        "  -d, --download <arg> \t Download the package especified\n"
        "  -i, --install  <arg> \t Install the package especified\n"
        "  -r, --remove   <arg> \t Uninstall the package especified\n"
        "  -q, --query    <arg> \t Show info about the package especified\n\n"
        "  --ccache             \t Clear the cache directory\n"
        "  --version            \t Print this software version\n";

    // enum long only options
    enum {
        CCACHE  = 1000,
        VERSION = 1001,
    };

    // clang-format off
    struct option opt_list[] = {
        {"help"    , no_argument      , NULL, 'h'},
        {"search"  , required_argument, NULL, 's'},
        {"update"  , no_argument      , NULL, 'u'},
        {"download", required_argument, NULL, 'd'},
        {"install" , required_argument, NULL, 'i'},
        {"remove"  , required_argument, NULL, 'r'},
        {"query"   , required_argument, NULL, 'q'},
        // long only
        {"ccache" , no_argument, NULL, CCACHE},
        {"version", no_argument, NULL, VERSION},
    };
    // clang-format on

    int opt_handler = getopt_long(argc, argv, "d:hi:r:s:uq:", opt_list, NULL);

    switch (opt_handler) {
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
    case 'q':
        matrix::query_pkg(optarg);
        break;
    case CCACHE:
        matrix::cache_clear();
        break;
    case VERSION:
        std::cout << version << std::endl;
        break;
    case 'h': /* fall through */
    default:
        std::cout << usage_str << std::endl;
        break;
    }

    return 0;
}
