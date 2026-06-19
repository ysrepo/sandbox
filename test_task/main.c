#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libavformat/avformat.h>

char * _arg_to_string(void * sample);

char * _avfmtctx_to_string(AVFormatContext sample);

int main(int argc, char *argv[]) {

    printf("0\n");

    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return -1;
    }

    printf("pre fmt_ctx\n");

    AVFormatContext *fmt_ctx = NULL; 

    printf("%-85s, %9s: \"0x%s\"\n", "address of pointer itself", "&fmt_ctx", _arg_to_string(&fmt_ctx));
    
    printf("%-85s, %9s: \"0x%s\"\n", "address the pointer points to (should be NULL since that is its initialization)", "fmt_ctx", _arg_to_string(fmt_ctx));

    // here next goes dereferencing of pointer, so at first must be checked if pointer is not NULL, otherwise it would lead to UB, e.g. Segmentation fault
    if (fmt_ctx != NULL) {
        printf(
            "%-85s, %9s: \"0x%s\"\n", 
            "derefenced value of pointer (should be NULL and should lead to undefined behaviour)", 
            "* fmt_ctx", 
            _avfmtctx_to_string(*fmt_ctx)
        );
    } else {
        printf(
            "%-85s, %9s: \"0x%s\"\n", 
            "derefenced value of pointer (should be NULL and should lead to undefined behaviour)", 
            "* fmt_ctx", 
            "0 (struct is not allocated in memory at all)"
        );
    }

    printf("size of struct variable before allocation: %zu bytes\n", sizeof(*fmt_ctx));

    fmt_ctx = avformat_alloc_context();

    // it prints the same size as before allocation as it is calculated by struct fields 
    // at compile-time regardless whether the fmt_ctx was initialized or not
    printf("size of struct variable after allocation: %zu bytes\n", sizeof(*fmt_ctx)); 

    if (!fmt_ctx) {
        perror("fmt_ctx error\n");
    }

    printf("input <filename>: %s\n", argv[1]);

    if (avformat_open_input(&fmt_ctx, argv[1], NULL, NULL) != 0) {
        perror("file opening error\n");
    }

    printf("post fmt_ctx\n");

    perror("error stream attempt 1\n");

    // 1. Open the file and read its header 
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        printf("av_log\n");
        av_log(NULL, AV_LOG_ERROR, "Could not open source\n");
        return -1;
    }

    // dump details of the file to standart error output
    av_dump_format(fmt_ctx, 0, argv[1], 0);

    printf("3\n");

    // 3. close the context and free resources
    avformat_close_input(&fmt_ctx);

    printf("4\n");

    return 0;
}

char *_arg_to_string(void * sample) {
    char * result = malloc(sizeof(char[32]));
    if (sample != NULL) { 
        snprintf(result, sizeof(char[32]), "%.10p", sample);
        return result;
    }
    return "0000000000";
}

char *_avfmtctx_to_string(AVFormatContext sample) {
    char * result = malloc(sizeof(char[32]));
    snprintf(result, sizeof(char[32]), "%.10p", &sample);
    return result;
}
