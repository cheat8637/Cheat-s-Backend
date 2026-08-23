#include "Emitter.h"
#include "../Reporter/Reporter.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool IsAlreadyCreated = false;
static Emitter_s* This = NULL;

static void FreeEmitter(void)
{
    free(This);
};
static void ByteEmitter(uint8_t Data, Buffer_s* Buffer)
{
    if (Buffer->Size >= Buffer->Capacity)
    {
        Buffer->Capacity = (Buffer->Capacity == 0) ? 32 : Buffer->Capacity * 2;
        uint8_t* NewBuffer = realloc(Buffer->Data, Buffer->Capacity);
        if (!NewBuffer)
        {
            Buffer->Free(Buffer);
            Error("Byte()", "out of memory, failed to allocate %zu bytes (realloc for Buffer_s structure).", Buffer->Capacity);
        };
        Buffer->Data = NewBuffer;
    };
    Buffer->Data[Buffer->Size] = Data;
    Buffer->Size++;
};
static void BytesEmitter(const uint8_t* Data, size_t Length, Buffer_s* Buffer)
{
    if (!Data) return;
    for (size_t ByteIndex = 0; ByteIndex < Length; ByteIndex++)
    {
        This->Byte(Data[ByteIndex], Buffer);
    };
};
static void Byte4Emitter(uint32_t Data, int Mode, Buffer_s* Buffer)
{
    switch (Mode)
    {
        case LITTLE_ENDIAN:
            This->Byte(Data & 0xFF, Buffer);
            This->Byte((Data >> 8) & 0xFF, Buffer);
            This->Byte((Data >> 16) & 0xFF, Buffer);
            This->Byte((Data >> 24) & 0xFF, Buffer);
            break;
        case BIG_ENDIAN:
            This->Byte((Data >> 24) & 0xFF, Buffer);
            This->Byte((Data >> 16) & 0xFF, Buffer);
            This->Byte((Data >> 8) & 0xFF, Buffer);
            This->Byte(Data & 0xFF, Buffer);
            break;
        default:
            Error("Byte4()", "invalid endianess mode: %d, try LITTLE- or BIG- ENDIANs.", Mode);
            break;
    };
};
static void Byte8Emitter(uint64_t Data, int Mode, Buffer_s* Buffer)
{
    switch (Mode)
    {
        case LITTLE_ENDIAN:
            This->Byte(Data & 0xFF,  Buffer);
            This->Byte((Data >> 8) & 0xFF, Buffer);
            This->Byte((Data >> 16) & 0xFF, Buffer);
            This->Byte((Data >> 24) & 0xFF, Buffer);
            This->Byte((Data >> 32) & 0xFF, Buffer);
            This->Byte((Data >> 40) & 0xFF, Buffer);
            This->Byte((Data >> 48) & 0xFF, Buffer);
            This->Byte((Data >> 56) & 0xFF, Buffer);
            break;
        case BIG_ENDIAN:
            This->Byte((Data >> 56) & 0xFF, Buffer);
            This->Byte((Data >> 48) & 0xFF, Buffer);
            This->Byte((Data >> 40) & 0xFF, Buffer);
            This->Byte((Data >> 32) & 0xFF, Buffer);
            This->Byte((Data >> 24) & 0xFF, Buffer);
            This->Byte((Data >> 16) & 0xFF, Buffer);
            This->Byte((Data >> 8) & 0xFF, Buffer);
            This->Byte(Data & 0xFF, Buffer);
            break;
        default:
            Error("Byte8()", "invalid endianess mode: %d, try LITTLE- or BIG- ENDIANs.", Mode);
            break;
    };
};

Emitter_s* Emitter(void)
{
    if (IsAlreadyCreated == true)
    {
        Error("Emitter()", "you can't have more than 1 emitter at a time.");
    }
    Emitter_s* Object = (Emitter_s*)malloc(sizeof(Emitter_s));

    Object->Free = FreeEmitter;
    Object->Byte = ByteEmitter;
    Object->Bytes = BytesEmitter;
    Object->Byte4 = Byte4Emitter;
    Object->Byte8 = Byte8Emitter;
    
    IsAlreadyCreated = true;
    This = Object;
    
    return Object;
};