//
// Created by sunhang on 2023/9/25.
//

//防止头文件重复包含
#pragma  once

#include <opencl-c-base.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>


static AVCodecContext *c = NULL;
static AVFrame *frame;
static AVCodecParserContext *parser;
static AVPacket *pkt;

void (*onYu12 )(uint8_t *data, int w, int h) = NULL;

uint8_t *concat(uint8_t *dataY, uint8_t *dataU, uint8_t *dataV, int w, int h) {
    int wh = w * h;
    uint8_t *data = (uint8_t *) malloc((size_t) (sizeof(uint8_t) * wh * 1.5f));
    memcpy(data, dataY, wh);
    memcpy(data + wh, dataU, wh / 4);
    memcpy(data + wh + wh / 4, dataV, wh / 4);
    return data;
}

void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt) {
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", dec_ctx->frame_number);
        fflush(stdout);

        uint8_t *data = concat(frame->data[0], frame->data[1], frame->data[2],
                               frame->width,
                               frame->height);
        onYu12(data, frame->width, frame->height);
    }
}

void decoder_setup() {
    const AVCodec *codec;

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* find the MPEG-1 video decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    parser = av_parser_init(codec->id);
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
}
void video_decode(uint8_t *data, size_t data_size) {
    while (data_size > 0) {
        int ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0) {
            fprintf(stderr, "Error while parsing\n");
            exit(1);
        }
        data += ret;
        data_size -= ret;

        if (pkt->size)
            decode(c, frame, pkt);
    }
}

void decoder_dispose() {
    decode(c, frame, NULL);

    av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}

#ifdef __cplusplus
}
#endif