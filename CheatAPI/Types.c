#include "Types.h"
#include "../Reporter/Reporter.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void FreeBuffer_s(Buffer_s* This)
{
    if (This->Data) free(This->Data);
    free(This);
};
static void FreeAddress_s(Address_s* This)
{
    free(This);
};
static void FreeLiteral_s(Literal_s* This)
{
    free(This);
};
static void FreeOperand_s(Operand_s* Object)
{
    if (Object->Address) FreeAddress_s(Object->Address);
    if (Object->Literal) FreeLiteral_s(Object->Literal);
    free(Object);
};

Buffer_s* Buffer(void)
{
    Buffer_s* Object = (Buffer_s*)malloc(sizeof(Buffer_s));
    if (!Object)
    {
        Error("Buffer()", "failed to allocate %zu bytes (for Buffer_s structure).", sizeof(Buffer_s));
    };
    Object->Free = FreeBuffer_s;
    return Object;
};
Address_s* Address(bool IsAbsolute, bool Is32Bit, void* Data, int64_t Offset)
{
    Address_s* Object = (Address_s*)malloc(sizeof(Address_s));
    if (!Object)
    {
        Error("Address()", "failed to allocate %zu bytes (for Address_s structure).", sizeof(Address_s));
    };
    Object->IsAbsolute = IsAbsolute;
    Object->Is32Bit = Is32Bit;
    if (IsAbsolute == true)
    {
        Object->Absolute = (uintptr_t)Data;
    }
    else
    {
        Object->Register = (RegisterNumber)(uintptr_t)Data;
    };
    Object->Offset = Offset;
    Object->Free = FreeAddress_s;
    return Object;
};
Literal_s* Literal(bool IsASCII, uint32_t ASCIILength, void* Data)
{
    Literal_s* Object = (Literal_s*)malloc(sizeof(Literal_s));
    if (!Object)
    {
        Error("Literal()", "failed to allocate %zu bytes (for Litral_s structure).", sizeof(Literal_s));
    }
    Object->IsASCII = IsASCII;
    Object->ASCIILength;
    if (IsASCII == true)
    {
        Object->ASCII = (const char*)Data;
    }
    else
    {
        Object->Number = (int64_t)(intptr_t)Data;
    };
    Object->Free = FreeLiteral_s;
    return Object;
};
Operand_s* Operand(OperandType Type, bool Is32Bit, void* Data)
{
    Operand_s* Object = (Operand_s*)malloc(sizeof(Operand_s));
    if (!Object)
    {
        Error("Operand()", "failed to allocate %zu bytes (for Operand_s structure).", sizeof(Operand_s));
    };
    Object->Type = Type;
    Object->Is32Bit = Is32Bit;
    switch (Type)
    {
        case REGISTER_OPERAND:
            Object->Register = (RegisterNumber)(uintptr_t)Data;
            break;
        case LITERAL_OPERAND:
            Object->Literal = (Literal_s*)Data;
            break;
        case ADDRESS_OPERAND:
            Object->Address = (Address_s*)Data;
            break;
        case SYMBOL_OPERAND:
            Object->Symbol = (char*)Data;
            break;
        default:
            Error("Operand()", "invalid operand type");
            break;
    };
    Object->Free = FreeOperand_s;
    return Object;
};