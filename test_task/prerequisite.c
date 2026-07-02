#include "prerequisite.h"

#include <stdio.h>

char _platform_dependencies_processed = 0;

char * _cwd = NULL;
char _path_separator = 0;

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

void _init_platform_consts() {    
    
    _init_cwd();
    _init_path_separator();

    if (_cwd && _path_separator) {
        _platform_dependencies_processed = 1;
    }

    return;
}