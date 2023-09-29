//
// Created by sunhang on 2023/9/25.
//

#ifndef PRJ0_BYTELISTENER_H
#define PRJ0_BYTELISTENER_H

#include <cstdint>

class ByteListener {
public:
    ByteListener() {};

    virtual ~ByteListener() {};

    virtual void onByte(uint8_t *byte, size_t size) = 0;

};

#endif //PRJ0_BYTELISTENER_H
