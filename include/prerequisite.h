#include <unistd.h>

#define FAIL 0x01
#define SUCCESS 0x00

#if defined(_WIN32)
    #include <windows.h>
    #define PATH_LIMIT MAX_PATH
    #define PATH_SEPARATOR '\\'

    #include <direct.h>
    #define _resolve_path_WIN32(path) _mkdir(path); 
    
    #define _resolve_path_OS(path) _resolve_path_WIN32(path)

#elif defined(__unix__)
    // use pathconf dynamically on POSIX, falling back to POSIX PATH_MAX
    #define PATH_LIMIT pathconf("/", _PC_PATH_MAX)
    #define PATH_SEPARATOR '/'

    #include <sys/stat.h>
    #define _resolve_path__unix__(path) mkdir(path, 0777); // TODO: maybe replace 0777 with right flags

    #define _resolve_path_OS(path) _resolve_path__unix__(path)

#endif
// in all other cases main terminates the execution
// since the OS hasn't been determined TODO: (see main.c:14,:25)

#if !defined(PATH_LIMIT)
    #define PATH_LIMIT FAIL
#endif

#if !defined(PATH_SEPARATOR)
    #define PATH_SEPARATOR FAIL
#endif


extern char * _cwd;
extern char _path_separator;

extern char * _output_folder_path;


unsigned char _init_platform_consts();

unsigned char _init_cwd();
unsigned char _init_path_separator();

unsigned char _init_output_folder();
unsigned char _init_output_folder_path();
unsigned char _resolve_output_folder_path();