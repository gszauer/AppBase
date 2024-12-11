//#define SHA1_USE_RFC_METHOD_2
#define SHA1_IMPLEMENTATION
#include "mackron_sha1.h"
#undef SHA1_IMPLEMENTATION

#define MD5_IMPLEMENTATION
#include "mackron_md5.h"
#undef MD5_IMPLEMENTATION

#define UUID_NO_CRYPTORAND

#define UUID_IMPLEMENTATION
#include "mackron_uuid.h"
#undef UUID_IMPLEMENTATION

#include <random>
#include <iostream>

std::string GenerateGuid() {
    static unsigned char uuid[UUID_SIZE + 1] = { 0 };
    static char uuid_str[UUID_SIZE_FORMATTED + 1] = { 0 };
    static uuid_rng generator = uuid_rng();
    uuid4(uuid, &generator);
    uuid_format(uuid_str, UUID_SIZE_FORMATTED, uuid);
    return uuid_str;
}

extern "C" uuid_result uuid_rng_generate(uuid_rand* pRNG, void* pBufferOut, size_t byteCount) {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<std::mt19937::result_type> rndDist(0, UINT_MAX);

    int numberOfInts = (int)byteCount / sizeof(int);
    unsigned int* intBufferOut = (unsigned int*)pBufferOut;
    for (int i = 0; i < numberOfInts; ++i) {
        intBufferOut[i] = rndDist(rng);
    }

    int bytesLeft = (int)byteCount - (numberOfInts * sizeof(int));
    unsigned char* charBufferOut = (unsigned char*)pBufferOut;
    unsigned int charBufferStart = (unsigned int)byteCount - (unsigned int)bytesLeft;
    charBufferOut = charBufferOut + charBufferStart;
    for (int i = 0; i < bytesLeft; ++i) {
        unsigned char rando = (unsigned char)(rndDist(rng) % 255);
        charBufferOut[i] = rando;
    }

    return UUID_SUCCESS;
}