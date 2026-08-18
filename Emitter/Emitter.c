#include "Emitter.h"
#include "../Reporter/Reporter.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

uint8_t IsAlreadyCreated = 0;
Emitter_s* This = NULL;

void FreeEmitter(void)
{
    if (This->Buffer) free(This->Buffer);
    free(This);
};
void ByteEmitter(uint8_t Data)
{
    if (This->Size >= This->Capacity)
    {
        This->Capacity = (This->Capacity == 0) ? 32 : This->Capacity * 2;
        uint8_t* NewBuffer = realloc(This->Buffer, This->Capacity);
        if (!NewBuffer)
        {
            This->Free();
            Error("Byte()", "out of memory, failed to allocate %zu bytes (realloc for buffer).", This->Capacity);
        };
        This->Buffer = NewBuffer;
    };
    This->Buffer[This->Size] = Data;
    This->Size++;
};
void BytesEmitter(const uint8_t* Data, size_t Length)
{
    if (!Data) return;
    for (size_t ByteIndex = 0; ByteIndex < Length; ByteIndex++)
    {
        This->Byte(Data[ByteIndex]);
    };
};
void Byte4Emitter(uint32_t Data, int Mode)
{
    switch (Mode)
    {
        case LITTLE_ENDIAN:
            This->Byte(Data & 0xFF);
            This->Byte((Data >> 8) & 0xFF);
            This->Byte((Data >> 16) & 0xFF);
            This->Byte((Data >> 24) & 0xFF);
            break;
        case BIG_ENDIAN:
            This->Byte((Data >> 24) & 0xFF);
            This->Byte((Data >> 16) & 0xFF);
            This->Byte((Data >> 8) & 0xFF);
            This->Byte(Data & 0xFF);
            break;
        default:
            Error("Byte4()", "invalid endianess mode: %d, try LITTLE- or BIG- ENDIANs.", Mode);
            break;
    };
};
void Byte8Emitter(uint64_t Data, int Mode)
{
    switch (Mode)
    {
        case LITTLE_ENDIAN:
            This->Byte(Data & 0xFF);
            This->Byte((Data >> 8) & 0xFF);
            This->Byte((Data >> 16) & 0xFF);
            This->Byte((Data >> 24) & 0xFF);
            This->Byte((Data >> 32) & 0xFF);
            This->Byte((Data >> 40) & 0xFF);
            This->Byte((Data >> 48) & 0xFF);
            This->Byte((Data >> 56) & 0xFF);
            break;
        case BIG_ENDIAN:
            This->Byte((Data >> 56) & 0xFF);
            This->Byte((Data >> 48) & 0xFF);
            This->Byte((Data >> 40) & 0xFF);
            This->Byte((Data >> 32) & 0xFF);
            This->Byte((Data >> 24) & 0xFF);
            This->Byte((Data >> 16) & 0xFF);
            This->Byte((Data >> 8) & 0xFF);
            This->Byte(Data & 0xFF);
            break;
        default:
            Error("Byte8()", "invalid endianess mode: %d, try LITTLE- or BIG- ENDIANs.", Mode);
            break;
    };
};

Emitter_s* Emitter(void)
{
    if (IsAlreadyCreated == 1)
    {
        Error("Emitter()", "You can't have more than 1 emitter at a time.");
    }
    Emitter_s* Object = (Emitter_s*)malloc(sizeof(Emitter_s));
    
    Object->Buffer = NULL;
    Object->Size = 0;
    Object->Capacity = 0;
    
    Object->Free = FreeEmitter;
    Object->Byte = ByteEmitter;
    Object->Bytes = BytesEmitter;
    Object->Byte4 = Byte4Emitter;
    Object->Byte8 = Byte8Emitter;
    
    IsAlreadyCreated = 1;
    This = Object;
    
    return Object;
};