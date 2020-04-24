#include <getopt.h>

#include "../include/packages.h"

int main(int argc, char **argv) {
    const std::string version = "v1.3";

    // cache & install
    matrix::initialize_dirs();

    std::string usage_str =
        "Usage: matrix [-dhiqrsuy] [...]\n\n"
        "Help:\n"
        "  -h, --help           \t Show this message\n"
        "  -s, --search   <pkg> \t Search for 'arg' in AUR\n"
        "  -u, --update   <pkg> \t Update the package especified\n"
        "  -d, --download <pkg> \t Download the package especified\n"
        "  -i, --install  <pkg> \t Install the package especified\n"
        "  -r, --remove   <pkg> \t Uninstall the package especified\n"
        "  -q, --query    <pkg> \t Show info about the package especified\n"
        "  -y, --sync           \t Sync the packages list\n\n"
        "  --ccache             \t Clear the cache directory\n"
        "  --version            \t Print this software version\n";

    // enum long only options
    enum {
        CCACHE  = 1000,
        VERSION = 1001,
        // dev options
        CACHEDIR   = 2000,
        INSTALLDIR = 2001,
    };

    // clang-format off
    struct option opt_list[] = {
        {"help"    , no_argument      , NULL, 'h'},
        {"search"  , required_argument, NULL, 's'},
        {"update"  , required_argument, NULL, 'u'},
        {"download", required_argument, NULL, 'd'},
        {"install" , required_argument, NULL, 'i'},
        {"remove"  , required_argument, NULL, 'r'},
        {"query"   , required_argument, NULL, 'q'},
        {"sync"    , no_argument      , NULL, 'y'},
        // long options only
        {"ccache" , no_argument, NULL, CCACHE},
        {"version", no_argument, NULL, VERSION},
        // dev options only
        {"cache-dir-location"  , no_argument, NULL, CACHEDIR},
        {"install-dir-location", no_argument, NULL, INSTALLDIR},
    };
    // clang-format on

    int opt_handler = getopt_long(argc, argv, "d:hi:r:s:u:q:y", opt_list, NULL);

    switch (opt_handler) {
    case 's':
        matrix::search_pkg(optarg);
        break;
    case 'u':
        matrix::update_pkg(optarg);
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
    case 'y':
        matrix::sync_pkg_list();
    // long options
    case CCACHE:
        matrix::cache_clear();
        break;
    case VERSION:
        std::cout << version << std::endl;
        break;
    // dev option
    case CACHEDIR:
        std::cout << matrix::cache_dir << std::endl;
        break;
    case INSTALLDIR:
        std::cout << matrix::install_dir << std::endl;
        break;
    case 'h': /* fall through */
    default:
        std::cout << usage_str << std::endl;
        break;
    }

    return 0;
}
