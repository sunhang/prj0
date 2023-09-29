//
// Created by sunhang on 2023/9/25.
//

#include "Network.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "decoder.h"
#include "libavcodec/defs.h"

#define INBUF_SIZE 1024 * 16

void Network::setByteListener(std::shared_ptr<ByteListener> &ptr) {
    this->mByteListener = ptr;
}

void Network::startWork() {
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(NET_IP);  //具体的IP地址
    serv_addr.sin_port = htons(NET_PORT);  //端口
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        video_log("connect failed");
        return;
    }

    //读取服务器传回的数据
    size_t buffer_size = INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE;

    uint8_t *buffer = (uint8_t *) malloc(buffer_size);
    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
    memset(buffer + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    ssize_t actual_size;
    while (true) {
        struct timeval tv;

        gettimeofday(&tv, NULL);
        long t0 = (tv.tv_sec*1000 + tv.tv_usec/1000);

        actual_size = read(sock, buffer, buffer_size);

        gettimeofday(&tv, NULL);
        long t1 = (tv.tv_sec*1000 + tv.tv_usec/1000);

        if (t1 - t0 > 100) {
            char logMessage[128];
            sprintf(logMessage, "actual_size:%d read time:%d", actual_size, (t1 - t0));
            video_log(logMessage);
        }

        if (actual_size <=0) break;
        size_t data_size = actual_size;
        uint8_t *data = buffer;

        mByteListener->onByte(data, data_size);
    }

    free(buffer);
    printf("Message form server: %s\n", buffer);

    //关闭套接字
    close(sock);
}
