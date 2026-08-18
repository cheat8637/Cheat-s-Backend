#ifndef CHEAT_EMITTER_H
#define CHEAT_EMITTER_H

#include <stdint.h>
#include <stddef.h>

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1

typedef struct Emitter_s Emitter_s;

struct Emitter_s
{
    uint8_t* Buffer;
    size_t Capacity;
    size_t Size;
    void (*Free)(void);
    void (*Byte)(uint8_t Data);
    void (*Bytes)(const uint8_t* Data, size_t Length);
    void (*Byte4)(uint32_t Data, int Mode);
    void (*Byte8)(uint64_t Data, int Mode);
};

Emitter_s* Emitter(void);

#endif