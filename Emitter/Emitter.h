#ifndef CHEAT_EMITTER_H
#define CHEAT_EMITTER_H

#include "../CheatAPI/Types.h"
#include <stdint.h>
#include <stddef.h>

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1

typedef struct Emitter_s Emitter_s;

struct Emitter_s
{
    void (*Free)(Emitter_s* This);
    void (*Byte)(uint8_t Data, Buffer_s* Buf);
    void (*Bytes)(const uint8_t* Data, size_t Length, Buffer_s* Buf);
    void (*Byte4)(uint32_t Data, int Mode, Buffer_s* Buf);
    void (*Byte8)(uint64_t Data, int Mode, Buffer_s* Buf);
};

Emitter_s* Emitter(void);

#endif