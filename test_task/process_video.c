#include "process_video.h"

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>

#include "prerequisite.h"

int decode_packet(AVFormatContext * format_context, AVPacket * packet, AVCodecContext * codex_context, AVFrame * frame) {
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
            snprintf(
                frame_filename, 
                sizeof(frame_filename), 
                "%s%c%s-%d.pgm", 
                _output_folder_path, 
                _path_separator, 
                "frame", 
                _frame_counter
            );

            if (frame->format != AV_PIX_FMT_YUV420P) {
                printf("Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB");
            }

            save_gray_frame(frame->data[0], frame->linesize[0], frame->width, frame->height, frame_filename);
        }
    }
}

void save_gray_frame(unsigned char * buf, int wrap, int xsize, int ysize, char * filename) {
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
