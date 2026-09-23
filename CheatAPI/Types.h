#ifndef CHEAT_OPERAND_H
#define CHEAT_OPERAND_H

#include <stdint.h>
#include <stddef.h>

#define MakeREG(Data) (void*)(uintptr_t)Data
#define MakeIMM(Data) (void*)(intptr_t)Data
#define MakeSYM(Data) (void*)Data

typedef struct Buffer_s Buffer_s;

struct Buffer_s
{
    uint8_t* Data;
    size_t Size;
    size_t Capacity;
    void (*Free)(Buffer_s* This);
};

typedef enum
{
    CODE_SECTION,
    DATA_SECTION,
    RDATA_SECTION,
    UDATA_SECTION,
    UNDEFINED_SECTION
}
SectionType;

typedef enum
{
    REGISTER_OPERAND,
    LITERAL_OPERAND,
    ADDRESS_OPERAND,
    SYMBOL_OPERAND
}
OperandType;

typedef enum
{
    R0, R1, R2, R3, R4, R5,
    R6, R7, R8, R9, R10, R11,
    R12, R13, R14, R15, R16, R17,
    R18, R19, R20, R21, R22, R23,
    R24, R25, R26, R27, R28, R29, 
    R30, RSpecial // SP, ZR, PC
}
RegisterNumber;

typedef struct Register_s Register_s;

struct Register_s
{
    bool Is32Bit;
    RegisterNumber Reg
};

typedef enum
{
    SYMBOL_TYPE_NOTYPE, // aka Label or Undefined
    SYMBOL_TYPE_VARIABLE,
    SYMBOL_TYPE_FUNCTION
}
SymbolType;

typedef struct SymbolRef_s SymbolRef_s;

struct SymbolRef_s // -erence
{
    const char* Name;
    SymbolType Type;
    void (*Free)(SymbolRef_s* This);
};

typedef struct Address_s Address_s;

struct Address_s
{
    bool Is32Bit;
    bool IsAbsolute;
    union
    {
        uintptr_t Absolute;
        RegisterNumber Register;
    };
    void (*Free)(Address_s* This);
};

typedef struct Literal_s Literal_s;

struct Literal_s
{
    bool IsASCII;
    uint32_t ASCIILength;
    union
    {
        int64_t Number;
        const char* ASCII;
    };
    void (*Free)(Literal_s* This);
};

typedef struct Operand_s Operand_s;

struct Operand_s
{
    OperandType Type;
    union
    {
        RegisterNumber FRegister;
        Literal_s* FLiteral;
        Address_s* FAddress;
        SymbolRef_s* FSymbol;
    };
    bool Is32Bit;
    void (*Free)(Operand_s* Object);
};

Buffer_s* Buffer(void);
SymbolRef_s* SymbolRef(const char* Name, SymbolType Type);
Address_s* Address(bool Is32Bit, bool IsAbsolute, void* Data, int64_t Offset);
Literal_s* Literal(bool IsASCII, uint32_t ASCIILength, void* Data);
Operand_s* Operand(OperandType Type, bool Is32Bit, void* Data);

#endif