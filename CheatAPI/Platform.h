#ifndef CHEAT_PLATFORM_H
#define CHEAT_PLATFORM_H

#include "Types.h"
#include <stdbool.h>
#include <stdint.h>

#define OUTPUT_ELF 0
#define OUTPUT_EXE 1
#define OUTPUT_BIN 2

#define BIN_OP_EQ 0
#define BIN_OP_LE 1
#define BIN_OP_GE 2
#define BIN_OP_NE 3
#define BIN_OP_L 4
#define BIN_OP_G 5

#define CREATED_ENTRY_POINT 0

#define Triple Operand_s* Destination, Operand_s* A, Operand_s* B

typedef struct Architecture_s Architecture_s;

struct Architecture_s
{
    const char* Name;
    int OutputFormat;

    void (*SetBuffer)(Buffer_s* NewBuffer, BufferType Type);
    
    void (*Move)(Operand_s* Destination, Operand_s* Source);
    void (*AddrOf)(RegisterNumber Destination, Operand_s* Source);
    void (*Load)(Operand_s* Destination, Address_s* Source);
    void (*Store)(Operand_s* Destination, Address_s* Source);
    
    void (*Add)(Triple);
    void (*Sub)(Triple);
    void (*Mul)(Triple);
    void (*Div)(Triple);
    void (*Neg)(Operand_s* Destination, Operand_s* Source);
    
    void (*And)(Triple);
    void (*Or)(Triple);
    void (*XOr)(Triple);
    void (*Not)(Operand_s* Destination, Operand_s* Source);
    
    void (*ShiftR)(Triple);
    void (*ShiftL)(Triple);
    void (*AShiftR)(Triple);
    void (*RotateL)(Triple);
    void (*RotateR)(Triple);
    
    void (*Push)(Operand_s* Data);
    void (*Pop)(Operand_s* Destination);
    
    void (*Label)(const char* Name);
    void (*Jump)(const char* Name);
    void (*Call)(const char* Name);
    void (*If)(Operand_s* A, int BinaryOperation, Operand_s* B, bool IsSigned, Address_s* Code);

    void (*Byte)(const char* Name, uint8_t Data, BufferType Type);
    void (*Byte2)(const char* Name, uint16_t Data,BufferType Type;
    void (*Byte4)(const char* Name, uint32_t Data, BufferType Type);
    void (*Byte8)(const char* Name, uint64_t Data, BufferType Type);
    void (*ASCII)(const char* Name, const char* Data, size_t Length, BufferType Type);
    void (*Reserve)(const char* Name, size_t Size, BufferType Type);

    void (*SysCall)(const char* Name);
    void (*Specific)(uint32_t Data, BufferType Type);
};

typedef struct OS_s OS_s;

struct OS_s
{
    const char* Name;
    const struct { const char* Name; int Number; }* SysCalls;
    size_t SysCallCount;
    #if CREATED_ENTRY_POINT == 1
        void (*MakeEntryPoint)(Buffer_s* Code);
    #endif
    void (*GenerateFile)(Buffer_s* Code, const char* Name, bool IsExited); // Name + .exe/.elf
};

#undef Triple

void AddArchitecture(Architecture_s* Architecture);
const Architecture_s* GetArchitecture(const char* Name);
void AddOS(OS_s* OS);
const OS_s* GetOS(const char* Name);

#endif