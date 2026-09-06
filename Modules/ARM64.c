#include "../CheatAPI/Platform.h"
#include "../CheatAPI/Types.h"
#include "../Emitter/Emitter.h"
#include "../Reporter/Reporter.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// R - Register, INumber - Immediate (I6, I16, etc.), M - Memory

// Buffer stuff
static Buffer_s* CodeBuffer = NULL;
static Buffer_s* DataBuffer = NULL;
static Buffer_s* RDataBuffer = NULL;
static Buffer_s* UDataBuffer = NULL;
static const Emitter_s* CurrentEmitter = Emitter();
static Buffer_s* GetBuffer(BufferType Type)
{
    switch (Type)
    {
        case CODE_BUFFER:
            return CodeBuffer;
            break;
        case DATA_BUFFER:
            return DataBuffer;
            break;
        case RDATA_BUFFER:
            return RDataBuffer;
            break;
        case UDATA_BUFFER:
            return UDataBuffer
        default:
            Error("GetBuffer()", "invalid buffer type");
            break;
    };
};
static void SetBuffer(Buffer_s* NewBuffer, BufferType Type)
{
    GetBuffer(Type) = NewBuffer;
};



// Reallocation table stuff
typedef enum
{
    RT_ADDR_64,
    RT_ADRP_21,
    RT_ADD_12,
    RT_LDR_19,
    RT_CALL_16,
    RT_JUMP_26,
    RT_GOT_PAGE,
    RT_LDRGOT_12
}
ReallocType;
typedef struct
{
    uint32_t CurrentOffset;
    uint32_t SymbolIndex;
    ReallocType Type;
    int64_t Addend;
}
Realloc_s;
static struct 
{
    Realloc_s** Data;
    size_t Size;
    size_t Capacity;
}
ReallocationTable = {NULL, 0, 0};
static void AddRealloc(uint32_t CurrentOffset, uint32_t SymbolIndex, ReallocType Type, int64_t Addend)
{
    #define RTSize ReallocationTable.Size
    #define RTCap ReallocationTable.Capacity
    
    Realloc_s* Object = (Realloc_s*)malloc(sizeof(Realloc_s));
    if (!Object) Error("AddRealloc()", "failed to allocate %zu bytes (for Realloc_s structure)", sizeof(Realloc_s));
    Object->CurrentOffset = CurrentOffset;
    Object->SymbolIndex = SymbolIndex;
    Object->Type = Type;
    Object->Addend = Addend;
    if (RTSize >= RTCapacity)
    {
        RTCapacity = RTCapacity == 0 ? 5 : RTCapacity * 2;
        Realloc_s** NewRT = realloc(ReallocationTable.Data, RTCapacity * sizeof(Realloc_s*));
        if (!NewRT)
        {
            free(ReallocationTable.defineata);
            Error("AddRealloc()", "failed to reallocate %zu bytes (for Reallocation table)", sizeof(Realloc_s*) * RTCapacity);
        };
        ReallocationTable.Data = NewRT;
    };
    ReallocationTable.Data[RTSize] = Object;
    RTSize++;
    #undef RTSize
    #undef RTCapacity
};




// Symbol stuff
typedef enum
{
    SECTION_CODE
    SECTION_DATA,
    SECTION_RDATA,
    SECTION_UDATA,
    SECTION_UNDEFINED,
}
SectionType;
typedef struct
{
    char* Name;
    size_t Size;
    uint32_t Offset;
    uint32_t Index;
    SectionType Section;
    bool IsDefined;
    bool IsGlobal;
}
Symbol_s;
static struct
{
    Symbol_s** Data;
    size_t Capacity;
    size_t Size;
}
SymbolTable = {NULL, 0, 0};
static void AddSymbol(const char* Name, size_t Size, uint32_t Offset, SectionType Section, bool IsDefined, bool IsGlobal)
{
    Symbol_s* Object = (Symbol_s*)malloc(sizeof(Symbol_s));
    if (!Object) Error("AddSymbol()", "failed to allocate %zu bytes (for Symbol_s structure)", sizeof(Symbol_s));
    Object->Name = Name;
    Object->Size = Size;
    Object->Offset = Offset;
    Object->Index = SymbolTable.Size;
    Object->Section = Section;
    Object->IsDefined = IsDefined;
    Object->IsGlobal = IsGlobal;
    if (SymbolTable.Size >= SymbolTable.Capacity)
    {
        SymbolTable.Capacity = SymbolTable.Capacity == 0 ? 5 : SymbolTable.Capacity * 2;
        Symbol_s** NewST = realloc(SymbolTable.Data, SymbolTable.Capacity * sizeof(Symbol_s*));
        if (!NewRT)
        {
            free(SymbolTable.Data);
            Error("AddSymbol()", "failed to reallocate %zu bytes (for Symbol table)", sizeof(Symbol_s*) * SymbolTable.Capacity);
        };
        SymbolTable = NewST;
    };
    SymbolTable.Data[SymbolTable.Size] = Object;
    SymbolTable.Size++;
};
static Symbol_s* FindSymbol(const char* SymbolName)
{
    for (int SymbolIndex = 0; SymbolIndex < SymbolTable.Size; SymbolIndex++)
    {
        if (strcmp(SymbolName, SymbolTable.Data[SymbolIndex]) == 0)
        {
            return SymbolTable.Data[SymbolIndex];
        };
    };
    return NULL;
};



// Local label stuff
typedef struct
{
    char* Name;
    uint32_t Offset;
    SectionType Section;
}
LocalLabel_s;
static struct
{
    LocalLabel_s** Data;
    size_t Size;
    size_t Capacity;
}
LocalLabels = {NULL, 0, 0A};
static void AddLocalLabel(const char* Name, SectionType Section)
{
    #define LLSize LocalLabels.Size
    #define LLCapacity LocalLabels.Capacity
    LocalLabel_s* Object = (LocalLabel_s*)malloc(sizeof(LocalLabel_s));
    if (!Object) Error("AddLocalLabel()", "failed to allocate %zu bytes (for LocalLabel_s structure)", sizeof(LocalLabel_s));
    Object->Name = Name;
    Object->Offset = GetBuffer(Type)->Size;
    Object->Section = Section;
    if (LLSize >= LLCapacity)
    {
        LLCapacity = LLCapacity == 0 ? LLCapacity = 5 : LLCapacity * 2;
        LocalLabel_s** NewLL = realloc(LocalLabels.Data, sizeof(LLCapacity) * LocalLabel_s*);
        if (!NewLL)
        {
            free(LocalLabels.Data);
            Error("AddLocalLabel()", "failed to allocate %zu bytes (for Local label table)", sizeof(LocalLabel_s*) * LLCapacity);
        };
        LocalLabels.Data = NewLL;
    };
    LocalLabels[LLSize] = Object;
    LLSize++;
    #undef LLSize
    #undef LLCapacity
};
static LocalLabel_s* FindLocalLabel(const char* LabelName)
{
    for (int LabelIndex = 0; LabelIndex < LocalLabels.Size; LabelIndex++)
    {
        if (strcmp(Name, LocalLabels.Data[LabelIndex]) == 0)
        {
            return LocalLabels.Data[LabelIndex];
        };
    };
    return NULL;
};
static int RLiteralCount = 0;


// Byte realisation
static void ARMByte(const char* Name, uint8_t Data, BufferType Type)
{
    AddLocalLabel(Name, (SectionType)Type);
    CurrentEmitter->Byte(Data, GetBuffer(Type));
};



// Byte2 realisation
static void ARMByte2(const char* Name, uint16_t Data, BufferType Type)
{
    AddLocalLabel(Name, (SectionType)Type);
    CurrentEmitter->Byte2(Data, LITTLE_ENDIAN, GetBuffer(Type));
};



// Byte4 realisation
static void ARMByte4(const char* Name, uint32_t Data, BufferType Type)
{
    AddLocalLabel(Name, (SectionType)Type);
    CurrentEmitter->Byte4(Data, LITTLE_ENDIAN, GetBuffer(Type));
};



// Byte8 realisation
static void ARMByte8(const char* Name, uint64_t Data, BufferType Type)
{
    AddLocalLabel(Name, (SectionType)Type);
    CurrentEmitter->Byte8(Data, LITTLE_ENDIAN, GetBuffer(Type));
};



// ASCII realisation
static void ARMASCII(const char* Name, const char* Data, size_t Length, BufferType Type)
{
    AddLocalLabel(Name, (SectionType)Type);
    for (size_t CharIndex = 0; CharIndex < Length; CharIndex++)
    {
        CurrentEmitter->Byte(Data[CharIndex], GetBuffer(Type));
    };
};



// Reserve realisation
static void ARMReserve(const char* Name, size_t Size, BufferType Type)
{
    AddLocalLabel(Name, (SectionType)Type);
    Buffer_s* TargetBuffer = GetBuffer(Type);
    for (size_t Index = 0; Index < Size; Index++)
    {
        CurrentEmitter->Byte(0x00, TargetBuffer);
    };
};


// Move realisation
static void /* R<-I16 */ EmitMovZ(RegisterNumber Rd, int64_t Number, uint8_t Shift /*0 - 3 for 64 bits, 0 - 1 for 32 bits*/, bool Is32Bit)
{
    // [31:23] Instruction, [22:21] Shift, [20:5] Immediate16, [4:0] Rd
    uint32_t Opcode = (Is32Bit ? 0b010100101 : 0b110100101) << 23; // + 23 zero bits  
    Opcode |= (Shift << 21);
    Opcode |= ((Number & 0xFFFF) << 5);
    Opcode |= Rd & 0x1F;
    CurrentEmitter->Byte4(Opcode, LITTLE_ENDIAN, CodeBuffer);
};
static void /* R<-I16 */ EmitMovK(RegisterNumber Rd,  int64_t Number, uint8_t Shift, bool Is32Bit)
{
    // Same as MovZ
    uint32_t Opcode = (Is32Bit ? 0b011100101 : 0b111100101) << 23;
    Opcode |= (Shift << 21);
    Opcode |= ((Number & 0xFFFF) << 5);
    Opcode |= (Rd & 0x1F << 0);
    CurrentEmitter->Byte4(Opcode, LITTLE_ENDIAN, CodeBuffer);
};
static void /* R<-R */ EmitORR(RegisterNumber Rd, RegisterNumber Rn, RegisterNumber Rm, uint8_t ShiftType /*0 - 3*/, uint8_t Shift, bool Is32Bit)
{
    // [31:24] Instruction, [23:22] Shift type, [21:21] N bit, [20:16] Rm, [15:10] Immediate6 (aka Shift), [9:5] Rn, [4:0] Rd    
    uint32_t Opcode = (Is32Bit ? 0b00101010 : 0b10101010) << 24;
    Opcode |= (ShiftType << 22);
    Opcode |= (0 << 21);
    Opcode |= (Rm & 0x1F << 16);
    Opcode |= ((Shift & 0b001111) << 10);
    Opcode |= (Rn & 0x1F << 5);
    Opcode |= (Rd & 0x1F);
    CurrentEmitter->Byte4(Opcode, LITTLE_ENDIAN, CodeBuffer);
};
static void ARMMove(Operand_s* Destination, Operand_s* Source)
{
    if (Destination->Type == ADDRESS_OPERAND || Source->Type == ADDRESS_OPERAND)
    {
        Error("ARMMove()", "destination address not supported, use Load or AddrOf instead");
    }
    else if (Destination->Type == LITERAL_OPERAND)
    {
        Error("ARMMove()", "destination can't be literal");
    }
    if (Destination->Type == REGISTER_OPERAND)
    {
        if (Source->Type == LITERAL_OPERAND)
        {
            uint64_t Value = (uint64_t)Source->FLiteral->Number;
            EmitMovZ(Destination->FRegister, Value & 0xFFFF, 0, Destination->Is32Bit);
            Value >>= 16;
            EmitMovK(Destination->FRegister, Value & 0xFFFF, 1, Destination->Is32Bit);
            Value >>= 16;
            if (!Is32Bit)
            {
                EmitMovK(Destination->FRegister, Value & 0xFFFF, 2, Destination->Is32Bit);
                Value >>= 16;
                EmitMovK(Destination->FRegister, Value & 0xFFFF, 3, Destination->Is32Bit);
            };
            return;
        }
        else if (Source->Type == REGISTER_OPERAND)
        {
            EmitORR(Destination->FRegister, RSpecial, Source->FRegister, 0, 0, Destination->Is32Bit);
            return;
        };
    }
};



// AddrOf realisation
static void EmitADRP(RegisterNumber Rd, char* SymbolName, int64_t Addend)
{
    // [31:31] N bit, [30:29] Low Immediate (Patch), [28:24] Instruction, [23:5] High Immediate (Patch), [4:0] Rd   
    Symbol_s* TargetSymbol = FindSymbol(SymbolName);
    if (!TargetSymbol)
    {
        AddSymbol(SymbolName, 0, 0, SECTION_UNDEFINED, false, true);
        TargetSymbol = FindSymbol(SymbolName);
    };
    
    uint32_t Opcode = 0b10010000 << 24;
    Opcode |= Rd & 0x1F;
    AddRealloc(BinBuffer->Size, TargetSymbol->Index, RT_ADRP_21, Addend);
    CurrentEmitter->Byte4(Opcode, LITTLE_ENDIAN, CodeBuffer);
};
static void ARMAddrOf(RegisterNumber Destination, Operand_s* Source)
{
    if (Source->FLiteral->IsASCII == false)
    {
        Error("ARMAddrOf()", "source can't be number literal");
    }
    else if (Destination->Type == REGISTER_OPERAND)
    {
        Error("ARMAddrOf()", "can't take address from register");
    };
    switch (Source->Type)
    {
        case LITERAL_OPERAND:
        {
            char TempASCII[5 + sizeof(uint32_t)];
            snprintf(TempASCII, sizeof(TempASCII), "TEMP_RDATA_LITERAL%d\0", RLiteralCount);
            ARMASCII(TempASCII, Source->FLiteral->ASCII, Source->FLiteral->ASCIILength, (BufferType)SECTION_RDATA);
            RLiteralCount++;
            EmitADRP(Destination, TempASCII, 0);
            break;
        }
        case SYMBOL_OPERAND:
        {
            EmitADRP();
            break;   
        }
    };
};

static void ARMSpecific(uint32_t Data, BufferType Type)
{
    CurrentEmitter->Byte4(Data, LITTLE_ENDIAN, GetBuffer(Type));
};