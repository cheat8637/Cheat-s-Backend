#include "Emitter.h"
#include "../Reporter/Reporter.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static void FreeEmitter(Emitter_s* This)
{
    free(This);
};
static void ByteEmitter(uint8_t Data, Buffer_s* Buf)
{
    if (Buf->Size >= Buf->Capacity)
    {
        Buf->Capacity = (Buf->Capacity == 0) ? 32 : Buf->Capacity * 2;
        uint8_t* NewBuffer = realloc(Buf->Data, Buf->Capacity);
        if (!NewBuffer)
        {
            Buf->Free(Buf);
            Error("Byte()", "out of memory, failed to allocate %zu bytes (realloc for Buffer_s structure).", Buf->Capacity);
        };
        Buf->Data = NewBuffer;
    };
    Buf->Data[Buf->Size] = Data;
    Buf->Size++;
};
static void BytesEmitter(const uint8_t* Data, size_t Length, Buffer_s* Buf)
{
    if (!Data) return;
    for (size_t ByteIndex = 0; ByteIndex < Length; ByteIndex++)
    {
        ByteEmitter(Data[ByteIndex], Buf);
    };
};
static void Byte4Emitter(uint32_t Data, int Mode, Buffer_s* Buf)
{
    switch (Mode)
    {
        case LITTLE_ENDIAN:
            ByteEmitter(Data & 0xFF, Buf);
            ByteEmitter((Data >> 8) & 0xFF, Buf);
            ByteEmitter((Data >> 16) & 0xFF, Buf);
            ByteEmitter((Data >> 24) & 0xFF, Buf);
            break;
        case BIG_ENDIAN:
            ByteEmitter((Data >> 24) & 0xFF, Buf);
            ByteEmitter((Data >> 16) & 0xFF, Buf);
            ByteEmitter((Data >> 8) & 0xFF, Buf);
            ByteEmitter(Data & 0xFF, Buf);
            break;
        default:
            Error("Byte4()", "invalid endianess mode: %d, try LITTLE- or BIG- ENDIANs.", Mode);
            break;
    };
};
static void Byte8Emitter(uint64_t Data, int Mode, Buffer_s* Buf)
{
    switch (Mode)
    {
        case LITTLE_ENDIAN:
            ByteEmitter(Data & 0xFF,  Buf);
            ByteEmitter((Data >> 8) & 0xFF, Buf);
            ByteEmitter((Data >> 16) & 0xFF, Buf);
            ByteEmitter((Data >> 24) & 0xFF, Buf);
            ByteEmitter((Data >> 32) & 0xFF, Buf);
            ByteEmitter((Data >> 40) & 0xFF, Buf);
            ByteEmitter((Data >> 48) & 0xFF, Buf);
            ByteEmitter((Data >> 56) & 0xFF, Buf);
            break;
        case BIG_ENDIAN:
            ByteEmitter((Data >> 56) & 0xFF, Buf);
            ByteEmitter((Data >> 48) & 0xFF, Buf);
            ByteEmitter((Data >> 40) & 0xFF, Buf);
            ByteEmitter((Data >> 32) & 0xFF, Buf);
            ByteEmitter((Data >> 24) & 0xFF, Buf);
            ByteEmitter((Data >> 16) & 0xFF, Buf);
            ByteEmitter((Data >> 8) & 0xFF, Buf);
            ByteEmitter(Data & 0xFF, Buf);
            break;
        default:
            Error("Byte8()", "invalid endianess mode: %d, try LITTLE- or BIG- ENDIANs.", Mode);
            break;
    };
};

Emitter_s* Emitter(void)
{
    Emitter_s* Object = (Emitter_s*)malloc(sizeof(Emitter_s));

    Object->Free = FreeEmitter;
    Object->Byte = ByteEmitter;
    Object->Bytes = BytesEmitter;
    Object->Byte4 = Byte4Emitter;
    Object->Byte8 = Byte8Emitter;
    
    return Object;
};