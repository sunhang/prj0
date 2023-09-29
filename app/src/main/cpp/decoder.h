//
// Created by sunhang on 2023/9/25.
//

#ifndef FFMPEGPARSE_VIDEO_H
#define FFMPEGPARSE_VIDEO_H


#ifdef __cplusplus
extern "C"
{
#endif

extern void (*onYu12)(uint8_t *data, int w, int h);

void decoder_setup();
void video_decode(uint8_t *data, size_t data_size);
void decoder_dispose();

#ifdef __cplusplus
}
#endif
#endif //FFMPEGPARSE_VIDEO_H
