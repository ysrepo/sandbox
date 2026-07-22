#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>

// TODO: duplicate #includes ???

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

void _analyze_pointer_workflow(AVFormatContext * some_sample_fmt_ctx) {
    printf("%-85s, %9s: \"0x%s\"\n", "address of pointer itself", "&fmt_ctx", _arg_to_string(&some_sample_fmt_ctx));
    
    printf("%-85s, %9s: \"0x%s\"\n", "address the pointer points to (should be NULL since that is its initialization)", "fmt_ctx", _arg_to_string(some_sample_fmt_ctx));

    // here next goes dereferencing of pointer, so at first must be checked if pointer is not NULL, otherwise it would lead to UB, e.g. Segmentation fault
    if (some_sample_fmt_ctx != NULL) {
        printf(
            "%-85s, %9s: \"0x%s\"\n", 
            "derefenced value of pointer (should be NULL and should lead to undefined behaviour)", 
            "* fmt_ctx", 
            _avfmtctx_to_string((AVFormatContext) *some_sample_fmt_ctx)
        );
    } else {
        printf(
            "%-85s, %9s: \"0x%s\"\n", 
            "derefenced value of pointer (should be NULL and should lead to undefined behaviour)", 
            "* fmt_ctx", 
            "0 (struct is not allocated in memory at all)"
        );
    }
}