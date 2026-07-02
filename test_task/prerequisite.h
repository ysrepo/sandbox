#include <unistd.h>

#if defined(_WIN32)
    #include <windows.h>
    #define PATH_LIMIT MAX_PATH
    #define PATH_SEPARATOR '\\'
#elif defined(__unix__)
    // use pathconf dynamically on POSIX, falling back to POSIX PATH_MAX
    #define PATH_LIMIT pathconf("/", _PC_PATH_MAX)
    #define PATH_SEPARATOR '/'
#endif

#if !defined(PATH_LIMIT)
    #define PATH_LIMIT -1
#endif

#if !defined(PATH_SEPARATOR)
    #define PATH_SEPARATOR -1
#endif

extern char _platform_dependencies_processed;

extern char * _cwd;
extern char _path_separator;

void _init_cwd();
void _init_path_separator();
void _init_platform_consts();