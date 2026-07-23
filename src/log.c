#include "log.h"

#include <stdio.h>

const char* PLATFORM_CONSTANTS_INITIALIZATION_ERROR = "\n\
The program entered the ERROR: \n\
platform identifier (operating system) is not detected. \n\
Neither Windows API nor Unix API found. \n\
The execution is terminated. \n\n";

const char* OUTPUT_FOLDER_INITIALIZATION_ERROR = "\n\
The program entered the ERROR: \n\
cannot initialize the output folder. \n\
The execution is terminated. \n\n";
            
const char* INPUT_FILE_MISSED_ERROR = "\n\
The program entered the ERROR: \n\
program usage: \"%s\" <input_file>. \n\
Please provide after the program name the filename. \n\
The execution is terminated. \n\n";

const char* VIDEO_PROCESSING_ERROR = "\n\
An ERROR has occurred during video processing. \n\
The execution is terminated. \n\n";

const char* CURRENT_EXECUTION_INFO = "\n\
input <filename>: %s, output <directory>: %s \n\n";

const char* EXIT_SUCCESS_INFO = "\n\
The program finished execution successfully. \n\
It returns to the calling environment. \n\n";

void label_log(enum LOG_TYPE log_type, const char * message, ...) {
    
    va_list args;
    va_start(args, message);

    switch (log_type) {
    case 0:
        log_info(message, args);
        break;
    case 1:
        log_error(message, args);
        break;
    default:
        log_info(message, args);
        break;
    }

    va_end(args);
}

void log_info(const char * message, va_list args) {
    vfprintf(stdout, message, args);
}

void log_error(const char * message, va_list args) {
    vfprintf(stderr, message, args);
}
