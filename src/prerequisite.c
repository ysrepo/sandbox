#include "prerequisite.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char * _OUTPUT_FOLDER_NAME = "dispatch";

char * _cwd = NULL;
char _path_separator = 0;

char * _output_folder_path = NULL;


unsigned char _init_platform_consts() {    
    
    if (_init_cwd() != SUCCESS) {
        // TODO: add fprintf stderr
        return FAIL;
    }

    if (_init_path_separator() != SUCCESS) {
        // TODO: add fprintf stderr
        return FAIL;
    }

    return SUCCESS;
}

unsigned char _init_cwd() {
    char cwd[PATH_LIMIT];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "getcwd() error\n");
        return FAIL;
    }

    _cwd = malloc(strlen(cwd) + 1);

    if (_cwd == NULL) {
        fprintf(stderr, "error during memory allocation for _cwd variable\n");
        return FAIL;
    }

    strcpy(_cwd, cwd);

    printf("_cwd variable initialized successfully, current working directory: %s\n", _cwd);

    return SUCCESS;
}

unsigned char _init_path_separator() {

    if (PATH_SEPARATOR == FAIL) {
        // os not found
        // therefore PATH_SEPARATOR not defined
        // TODO: add fprintf stderr
        return FAIL;
    }

    _path_separator = PATH_SEPARATOR;

    return SUCCESS;
}

unsigned char _init_output_folder() {

    if (_init_output_folder_path() != SUCCESS) {
        // TODO: add fprintf stderr
        return FAIL;
    }

    if (_resolve_output_folder_path() != SUCCESS) {
        // TODO: add fprintf stderr
        return FAIL;
    }

    printf("output folder initialized successfully: %s\n", _output_folder_path);

    return SUCCESS;
}

unsigned char _init_output_folder_path() {

    _output_folder_path = malloc(strlen(_cwd) + 1 + strlen(_OUTPUT_FOLDER_NAME) + 1); // (.. + path separator + .. + termination symbol)

    if (_output_folder_path == NULL) {
        // malloc may return NULL
        // always check and in such case throw (lead to) error
        // TODO: add fprintf stderr
        return FAIL;
    }

    strcpy(_output_folder_path, _cwd);
    strcat(_output_folder_path, (char *) &_path_separator); // path separator is single char, convert it to (char *) for concatenation
    strcat(_output_folder_path, _OUTPUT_FOLDER_NAME);

    return SUCCESS;
}

unsigned char _resolve_output_folder_path() {

    // TODO: possibly handle here errors if failed

    return _resolve_path_OS(_output_folder_path);

} 
