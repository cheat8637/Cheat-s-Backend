#ifndef CHEAT_OPERAND_H
#define CHEAT_OPERAND_H

#include <stdint.h>
#include <stdbool.h>

#define MakeREG(Data) (void*)(uintptr_t)Data
#define MakeIMM(Data) (void*)(intptr_t)Data

typedef enum
{
    R0, R1, R2, R3, R4, R5,
    R6, R7, R8, R9, R10, R11,
    R12, R13, R14, R15, R16, R17,
    R18, R19, R20, R21, R22, R23,
    R24, R25, R26, R27, R28, R29, 
    R30, SPR, RVR,
}
RegisterNumber;

typedef enum
{
    REGISTER_OPERAND,
    IMMEDIATE_OPERAND,
    ADDRESS_OPERAND,
}
OperandType;

typedef struct Address_s Address_s;

struct Address_s
{
    bool IsAbsolute;
    union
    {
        uintptr_t Absolute;
        RegisterNumber Register;
    };
    int64_t Offset;
    void (*Free)(Address_s* Object);
};

typedef struct Operand_s Operand_s;

struct Operand_s
{
    OperandType Type;
    union
    {
        RegisterNumber Register;
        int64_t Immediate;
        Address_s* Address;
    };
    void (*Free)(Operand_s* Object);
};

Address_s* Address(bool IsAbsolute, void* Data, int64_t Offset);
Operand_s* Operand(OperandType Type, void* Data);

#endif