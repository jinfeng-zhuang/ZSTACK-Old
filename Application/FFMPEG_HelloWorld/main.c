#include <zstack/zstack.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    u32 len = SIZE_1MB;
    u8 *buffer;

    const AVCodec* codec;
    AVCodecParserContext* parser;
    AVCodecContext* ctx = NULL;
    AVPacket* pkt;

#if 0
    AVStream* stream;
    AVCodecParameters* par;
#endif

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    av_log_set_level(AV_LOG_TRACE);

    buffer = file_load_simple(app.param.input_filename, 0, &len);
    if (NULL == buffer)
        return -1;

    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    parser = av_parser_init(codec->id);
    ctx = avcodec_alloc_context3(codec);
    pkt = av_packet_alloc();

    avcodec_open2(ctx, codec, NULL);

    av_parser_parse2(parser, ctx,
        &pkt->data, &pkt->size,
        buffer, len,
        AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

    info("GET PKT: %d , %x\n", pkt->data - buffer, pkt->size);

    info("%s %s@%d\n", avcodec_get_name(codec->id), av_get_profile_name(codec, ctx->profile), ctx->level);

#if 0
    AVFrame* frame;
    frame = av_frame_alloc();

    avcodec_send_packet(ctx, pkt);
    avcodec_receive_frame(ctx, frame);

    av_frame_free(&frame);
#endif

    if (buffer) {
        free(buffer);
    }
    av_parser_close(parser);
    avcodec_free_context(&ctx);
    av_packet_free(&pkt);

    return 0;
}
