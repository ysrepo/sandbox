#include "prerequisite.h"
#include "log.h"
#include "process_video.h"

int main(int argc, char * argv[]) { 

    if (_init_platform_consts() != SUCCESS) {

        label_log(ERROR_MESSAGE, PLATFORM_CONSTANTS_INITIALIZATION_ERROR);

        return EXIT_FAILURE;

    }

    if (_init_output_folder() != SUCCESS) {

        label_log(ERROR_MESSAGE, OUTPUT_FOLDER_INITIALIZATION_ERROR);

        return EXIT_FAILURE;

    }

    if (argc < 2) {
        // argc - argument count - the total number of arguments passed to the program
        // argv[] - argument vector - the list of values of these arguments passed to the program

        label_log(ERROR_MESSAGE, INPUT_FILE_MISSED_ERROR, argv[0]);

        return EXIT_FAILURE;

    }

    label_log(INFO_MESSAGE, CURRENT_EXECUTION_INFO, argv[1], _output_folder_path);

    if (process_video(argv[1]) != SUCCESS) {

        label_log(ERROR_MESSAGE, VIDEO_PROCESSING_ERROR);

        return EXIT_FAILURE;

    }

    label_log(INFO_MESSAGE, EXIT_SUCCESS_INFO);

    return EXIT_SUCCESS;
} 
