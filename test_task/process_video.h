#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

static int _frame_counter = 0;

int decode_packet(
    AVFormatContext * format_context, 
    AVPacket * packet, 
    AVCodecContext * codex_context, 
    AVFrame * frame
);

void save_gray_frame(unsigned char * buf, int wrap, int xsize, int ysize, char * filename);
