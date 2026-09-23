#ifndef CHEAT_ARCHITECTURE_CORE_H
#define CHEAT_ARCHITECTURE_CORE_H

#include "../../CheatAPI/Types.h"
#include "../../Emitter/Emitter.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>



typedef struct Context_s Context_s;

// Buffer & Emitter stuff

typedef struct
{
    Buffer_s* Code;
    Buffer_s* Data;
    Buffer_s* RData;
    Buffer_s* UData;
}
BufferTable;
Buffer_s* GetBuffer(SectionType Section, Context_s* ArchContext);


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
    uint32_t Index;
    ReallocType Type;
    int64_t Addend;
    SectionType Section;
}
Realloc_s;
typedef struct
{
    Realloc_s** Data;
    size_t Capacity;
    size_t Size;
}
ReallocTable;
void AddRealloc(uint32_t Index, ReallocType Type, int64_t Addend, SectionType Section, Context_s* ArchContext);   



// Symbol table stuff

typedef struct
{
    char* Name;
    size_t Size;
    uint32_t Offset;
    uint32_t Index;
    SectionType Section;
    bool IsDefined;
    bool IsGlobal;
    SymbolType Type;
}
Symbol_s; 
typedef struct
{
    Symbol_s** Data;
    size_t Capacity;
    size_t Size;
}
SymbolTable;
void AddSymbol(const char* Name, size_t Size, SectionType Section, bool IsDefined, bool IsGlobal, SymbolType Type, Context_s* ArchContext);
Symbol_s* FindSymbol(const char* SymbolName, Context_s* ArchContext);
void DefineSymbol(Symbol_s* TargetSymbol, size_t Size, SectionType Section, bool IsDefined, bool IsGlobal, SymbolType Type, Context_s* ArchContext);


struct Context_s
{
    BufferTable Buffers;
    ReallocTable* Reallocs;
    SymbolTable* Symbols;
};

Context_s* Context(void);



#endif