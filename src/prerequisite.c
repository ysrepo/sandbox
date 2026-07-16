#include "prerequisite.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h> // UNIX API

// define if UNIX API
// then _resolve_output_folder_path() expands to _resolve_output_folder_path_unix()
// end if

// define if Windows API
// then _resolve_output_folder_path() expands to _resolve_output_folder_path_win()
// end if


const char * _OUTPUT_FOLDER_NAME = "dispatch";


char _platform_dependencies_processed = 0;

char * _cwd = NULL;
char _path_separator = 0;

char * _output_folder_path = NULL;


void _init_platform_consts() {    
    
    _init_cwd();
    _init_path_separator();

    if (_cwd && _path_separator) {
        _platform_dependencies_processed = 1;
    }

    return;
}

void _init_cwd() {
    char cwd[PATH_LIMIT];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    _cwd = malloc(strlen(cwd) + 1);

    if (_cwd == NULL) {
        return;
    }

    strcpy(_cwd, cwd);

    printf("_cwd initialized successfully, current working directory: %s\n", _cwd);

    return;
}

void _init_path_separator() {
    _path_separator = 0;

    if (PATH_SEPARATOR == -1) {
        // os not found
        // therefore PATH_SEPARATOR not defined
        return;
    }

    _path_separator = PATH_SEPARATOR;

    return;
}

void _init_output_folder() {

    _init_output_folder_path();

    // init .. path
    _resolve_output_folder_path();

    printf("output folder initialized successfully: %s\n", _output_folder_path);

    return;
}

void _init_output_folder_path() {

    _output_folder_path = malloc(strlen(_cwd) + 1 + strlen(_OUTPUT_FOLDER_NAME) + 1); // (.. + path separator + .. + termination symbol)

    if (_output_folder_path == NULL) {
        // malloc may return NULL
        // always check and TODO: in such case throw (lead to) error
        return;
    }

    strcpy(_output_folder_path, _cwd);
    strcat(_output_folder_path, (char *) &_path_separator); // path separator is single char, convert it to (char *) for concatenation
    strcat(_output_folder_path, _OUTPUT_FOLDER_NAME);

    return;
}

void _resolve_output_folder_path() {

    // _resolve_output_folder_path_OS > expands to _resolve_output_folder_path_unix() or to _resolve_output_folder_path_win()

    // _mkdir(_output_folder); // Win API

    int retval = mkdir(_output_folder_path, 0777);

    if (retval == -1) {
        // error
    } else {
        // if retval equals 0 then success
    }

    return;
}