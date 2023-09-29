//
// Created by sunhang on 2023/9/25.
//

#ifndef PRJ0_NETWORK_H
#define PRJ0_NETWORK_H

#include <memory>
#include "common.h"
#include "ByteListener.h"

class Network {
public:
    void startWork();

    void setByteListener(std::shared_ptr<ByteListener> &ptr);

private:
    std::shared_ptr<ByteListener> mByteListener;
};


#endif //PRJ0_NETWORK_H
