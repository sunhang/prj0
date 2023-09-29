#include <jni.h>
#include <android/log.h>
#include "decoder.h"
#include "Network.h"

JNIEnv *env;
jobject thiz;
jmethodID methodId;

class ByteListenerImpl : public ByteListener {
public:
    ByteListenerImpl() {};

    virtual ~ByteListenerImpl() {};

    virtual void onByte(uint8_t *byte, size_t size) {
        video_decode(byte, size);
    }
};

void onYu12Impl(uint8_t *yu12, int w, int h) {
    size_t size = w * h * 1.5;
    jbyteArray byte_arr = env->NewByteArray(size);

    void *temp = env->GetPrimitiveArrayCritical((jarray) byte_arr, 0);
    memcpy(temp, yu12, size);
    env->ReleasePrimitiveArrayCritical(byte_arr, temp, 0);
    env->CallVoidMethod(thiz, methodId, byte_arr, (jint)w, (jint)h);

    free(yu12);
    env->DeleteLocalRef(byte_arr);
}

extern "C"
JNIEXPORT void JNICALL
Java_sunhang_prj0_Manager_nativeWork(JNIEnv *env, jobject thiz) {
    ::env = env;
    ::thiz = thiz;
    jclass cls = env->GetObjectClass(thiz);
    methodId = env->GetMethodID(cls, "handleYu12Data", "([BII)V");

    onYu12 = onYu12Impl;

    decoder_setup();

    std::shared_ptr<ByteListener> ptr = std::make_shared<ByteListenerImpl>();
    Network network;
    network.setByteListener(ptr);
    network.startWork();

    decoder_dispose();
}

