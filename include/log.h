#include <stdarg.h>

extern const char* PLATFORM_CONSTANTS_INITIALIZATION_ERROR;
extern const char* OUTPUT_FOLDER_INITIALIZATION_ERROR;
extern const char* INPUT_FILE_MISSED_ERROR;
extern const char* VIDEO_PROCESSING_ERROR;

extern const char* CURRENT_EXECUTION_INFO;

enum LOG_TYPE {
    INFO,
    ERROR
};

void label_log(enum LOG_TYPE log_type, const char * message, ...);

void log_info(const char * message, va_list args);

void log_error(const char * message, va_list args);
