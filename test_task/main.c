#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>

char * _arg_to_string(void * sample);

char * _avfmtctx_to_string(AVFormatContext sample);

void _analyze_pointer_workflow(AVFormatContext * some_sample_fmt_ctx);

static int _frame_counter = 0;

static int decode_packet(
    AVFormatContext * format_context, 
    AVPacket * packet, 
    AVCodecContext * codex_context, 
    AVFrame * frame, 
    char * output_directory
);

static void save_gray_frame(unsigned char * buf, int wrap, int xsize, int ysize, char * filename);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <output_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    AVFormatContext *fmt_ctx = NULL; 
    int video_stream_index = -1;

    // _analyze_pointer_workflow(fmt_ctx); // studing

    // printf("size of struct variable before allocation: %zu bytes\n", sizeof(*fmt_ctx));

    fmt_ctx = avformat_alloc_context();

    // it prints the same size as before allocation as it is calculated by struct fields 
    // at compile-time regardless whether the fmt_ctx was initialized or not
    // printf("size of struct variable after allocation: %zu bytes\n", sizeof(*fmt_ctx)); 
    if (!fmt_ctx) {
        perror("fmt_ctx error\n");
    }

    printf("\ninput <filename>: %s, <output_directory>: %s\n\n", argv[1], argv[2]);

    if (avformat_open_input(&fmt_ctx, argv[1], NULL, NULL) != 0) {
        perror("file opening error\n");
    }

    // 1. Open the file and read its header 
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        printf("av_log\n");
        av_log(NULL, AV_LOG_ERROR, "Could not open source\n");
        return EXIT_FAILURE;
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
            response = decode_packet(fmt_ctx, _packet, _video_codec_context, _frame, argv[2]);
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

    return EXIT_SUCCESS;
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

static int decode_packet(AVFormatContext * format_context, AVPacket * packet, AVCodecContext * codex_context, AVFrame * frame, char * output_directory) {
    // supply raw packet data as input to a decoder
    int response = avcodec_send_packet(codex_context, packet);

    if (response < 0) {
        printf("Error while sending a packet to the decoder: %s", av_err2str(response));
    }

    while (response >= 0) {
        // return decoded output data (into a frame) from a decoder
        response = avcodec_receive_frame(codex_context, frame);

        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            printf("Error here, error code: %d. AVERROR(EAGAIN): %d, AVERROR_EOF: %d.\n", response, AVERROR(EAGAIN), AVERROR_EOF);
            break;
        } else if (response < 0) {
            printf("Error while receiving a frame to the decoder: %s", av_err2str(response));
        }
        
        if (response >= 0) {
            int buffer_size = av_image_get_buffer_size(frame->format, frame->width, frame->height, 1);
            printf(
                "Frame %d (type=%c, size=%d bytes, format=%d) pts %d key_frame %d [DTS %d]\n",
                codex_context->frame_num,
                av_get_picture_type_char(frame->pict_type),
                buffer_size,
                frame->format,
                frame->pts,
                av_stream_get_parser(format_context->streams[0])->key_frame,
                _frame_counter++
            );

            char frame_filename[1024];
            snprintf(frame_filename, sizeof(frame_filename), "%s\\%s-%d.pgm", output_directory, "frame", codex_context->frame_num);

            if (frame->format != AV_PIX_FMT_YUV420P) {
                printf("Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB");
            }

            save_gray_frame(frame->data[0], frame->linesize[0], frame->width, frame->height, frame_filename);
        }
    }
}

static void save_gray_frame(unsigned char * buf, int wrap, int xsize, int ysize, char * filename) {
    FILE * f;
    int i;

    f = fopen(filename, "w");

    // writing the minimal required header for a pgm file format
    // portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example 
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    // writing line by line
    for (i = 0; i < ysize; i++) {
        fwrite(buf + i * wrap, 1, xsize, f);
    }

    fclose(f);
}