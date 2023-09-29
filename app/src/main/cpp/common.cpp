//
// Created by sunhang on 2023/9/29.
//

#include <android/log.h>
#include "common.h"

void video_log(char *logMessage) {
    __android_log_write(ANDROID_LOG_DEBUG, LOG_TAG,
                        logMessage); //Or ANDROID_LOG_INFO, ...
}