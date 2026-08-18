#include "CheatTypes.h"
#include "../Reporter/Reporter.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void FreeAddress_s(Address_s* Object)
{
    free(Object);
};
void FreeOperand_s(Operand_s* Object)
{
    if (Object->Address) FreeAddress_s(Object->Address);
    free(Object);
};

Address_s* Address(bool IsAbsolute, void* Data, int64_t Offset)
{
    if (!Data)
    {
        Error("Address()", "null pointer passed.");
    };
    Address_s* Object = (Address_s*)malloc(sizeof(Address_s));
    if (!Object)
    {
        Error("Address()", "failed to allocate %zu bytes (for Address_s structure).", sizeof(Address_s));
    };
    Object->IsAbsolute = IsAbsolute;
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
Operand_s* Operand(OperandType Type, void* Data)
{
    Operand_s* Object = (Operand_s*)malloc(sizeof(Operand_s));
    if (!Object)
    {
        Error("Operand()", "failed to allocate %zu bytes (for Operand_s structure).", sizeof(Operand_s));
    };
    Object->Type = Type;
    switch (Type)
    {
        case REGISTER_OPERAND:
            Object->Register = (RegisterNumber)(uintptr_t)Data;
            break;
        case IMMEDIATE_OPERAND:
            Object->Immediate = (int64_t)(intptr_t)Data;
            break;
        case ADDRESS_OPERAND:
            Object->Address = (Address_s*)Data;
            break;
    };
    Object->Free = FreeOperand_s;
    return Object;
};