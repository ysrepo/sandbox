#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>

#include "prerequisite.h"
#include "log.h"
#include "process_video.h"

int process_video(char * filename);

int main(int argc, char * argv[]) { 

    if (_init_platform_consts() != SUCCESS) {

        label_log(ERROR, PLATFORM_CONSTANTS_INITIALIZATION_ERROR);

        return EXIT_FAILURE;
    }

    if (_init_output_folder() != SUCCESS) {

        label_log(ERROR, OUTPUT_FOLDER_INITIALIZATION_ERROR);

        return EXIT_FAILURE;

    }

    // argc - argument count - the total numer of arguments passed to the program
    // argv[] - argument vector - the list of values of these arguments passed to the program
    if (argc < 2) {

        label_log(ERROR, INPUT_FILE_MISSED_ERROR, argv[0]);

        return EXIT_FAILURE;
    }

    label_log(INFO, CURRENT_EXECUTION_INFO, argv[1], _output_folder_path);

    if (process_video(argv[1]) != SUCCESS) {

        label_log(ERROR, VIDEO_PROCESSING_ERROR);

        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}

// TODO: refactor the process_video(...), split code into blocks, move to separate files
int process_video(char * filename) {
    AVFormatContext *fmt_ctx = NULL; 
    int video_stream_index = -1;

    fmt_ctx = avformat_alloc_context();

    // it prints the same size as before allocation as it is calculated by struct fields 
    // at compile-time regardless whether the fmt_ctx was initialized or not
    // printf("size of struct variable after allocation: %zu bytes\n", sizeof(*fmt_ctx)); 
    if (!fmt_ctx) {
        fprintf(stderr, "fmt_ctx error\n");
    }

    if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) != 0) {
        fprintf(stderr, "file opening error\n");
    }

    // 1. Open the file and read its header 
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        printf("av_log\n");
        av_log(NULL, AV_LOG_ERROR, "Could not open source\n");
        return FAIL;
    }
    
    // search video stream and store the codec context out of it into _video_codec_context pointer variable
    AVCodecContext * _video_codec_context = NULL;
	for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_id) {
            AVCodecParameters *_current_codec_params = fmt_ctx->streams[i]->codecpar;

            enum AVCodecID _current_codec_id = _current_codec_params->codec_id;

            const AVCodec *_current_codec = avcodec_find_decoder(_current_codec_id);

            printf(
                "-----------\n%s codec: %s\n---------------------\n", 
                av_get_media_type_string(_current_codec_params->codec_type), 
                avcodec_get_name(fmt_ctx->streams[i]->codecpar->codec_id)
            );

            if (_current_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
                video_stream_index = i;
                // allocate memory for the AVCodecContext, which will hold the context for our decode/encode process
                // and fill the codec context with CODEC parameters (av_parameters_to_context(..))
                _video_codec_context = avcodec_alloc_context3(_current_codec);
                avcodec_parameters_to_context(_video_codec_context, _current_codec_params);
                // once the codec context is filled, we need to open the codec
                // we call the function avcodec_open2() and then we can use it
                avcodec_open2(_video_codec_context, _current_codec, NULL);
            }

            // general
            printf("Codec %s ID %d bit_rate %lld\n\n", _current_codec->long_name, _current_codec->id, _current_codec_params->bit_rate);
        }
    }

    // read the packets from the stream and decode them into frames
    // for this at first allocate AVPacket, AVFrame vars
    AVPacket * _packet = av_packet_alloc();
    AVFrame * _frame = av_frame_alloc();

    int response = 0;
    int how_many_packets_to_process = 8;

    while (av_read_frame(fmt_ctx, _packet) >= 0) {
        if (_packet->stream_index == video_stream_index) {
            printf("AVPacket->pts %d\n", _packet->pts);
            response = decode_packet(fmt_ctx, _packet, _video_codec_context, _frame);
            if (response < 0)
                break;
            // stop it, otherwise we'll be saving hundreds of frames
            if (--how_many_packets_to_process <= 0) 
                break;
        }
        av_packet_unref(_packet);
    }

    // dump details of the file to standart error output
    // av_dump_format(fmt_ctx, 0, argv[1], 0);

    // 3. close the context and free resources
    avformat_close_input(&fmt_ctx);

    return SUCCESS;
}
