#include "../../CheatAPI/Types.h"
#include "../../Emitter/Emitter.h"
#include "../../Reporter/Reporter.h"
#include "ArchitectureCore.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>



Buffer_s* GetBuffer(SectionType Section, Context_s* ArchContext)
{
    #define Table ArchContext->Buffers
    switch (Section)
    {
        case CODE_SECTION:
            return Table.Code;
        case DATA_SECTION:
            return Table.Data;
        case RDATA_SECTION:
            return Table.RData;
        case UDATA_SECTION:
            return Table.UData;
        default:
            Error("GetBuffer()", "invalid section type");
    };
    #undef Table
    return NULL;
};

void AddRealloc(uint32_t Index, ReallocType Type, int64_t Addend, SectionType Section, Context_s* ArchContext)
{
    #define Table ArchContext->Reallocs
    Realloc_s* Object = (Realloc_s*)malloc(sizeof(Realloc_s));
    if (!Object) Error("AddRealloc()", "failed to allocate %zu bytes (for Realloc_s structure)", sizeof(Realloc_s));
    Object->CurrentOffset = GetBuffer(Section, ArchContext)->Size;
    Object->Index = Index;
    Object->Type = Type;
    Object->Addend = Addend;
    Object->Section = Section;
    if (Table->Size >= Table->Capacity)
    {
        Table->Capacity = Table->Capacity == 0 ? 5 : Table->Capacity * 2;
        Realloc_s** NewRT = realloc(Table->Data, Table->Capacity * sizeof(Realloc_s*));
        if (!NewRT)
        {
            free(Table->Data);
            Error("AddRealloc()", "failed to reallocate %zu bytes (for Reallocation table)", sizeof(Realloc_s*) * Table->Capacity);
        };
        Table->Data = NewRT;
    };
    Table->Data[Table->Size] = Object;
    Table->Size++;
    #undef Table
};

void AddSymbol(const char* Name, size_t Size, SectionType Section, bool IsDefined, bool IsGlobal, SymbolType Type, Context_s* ArchContext)
{
    #define Table ArchContext->Symbols
    Symbol_s* Object = (Symbol_s*)malloc(sizeof(Symbol_s));
    if (!Object) Error("AddSymbol()", "failed to allocate %zu bytes (for Symbol_s structure)", sizeof(Symbol_s));
    Object->Name = strdup(Name);
    Object->Size = Size;
    Object->Offset = IsDefined ? GetBuffer(Section, ArchContext)->Size : 0;
    Object->Index = Table->Size;
    Object->Section = Section;
    Object->IsDefined = IsDefined;
    Object->IsGlobal = IsGlobal;
    Object->Type = Type;
    if (Table->Size >= Table->Capacity)
    {
        Table->Capacity = Table->Capacity == 0 ? 5 : Table->Capacity * 2;
        Symbol_s** NewST = realloc(Table->Data, Table->Capacity * sizeof(Symbol_s*));
        if (!NewST)
        {
            free(Table->Data);
            Error("AddSymbol()", "failed to reallocate %zu bytes (for Symbol table)", sizeof(Symbol_s*) * Table->Capacity);
        };
        Table->Data = NewST;
    };
    Table->Data[Table->Size] = Object;
    Table->Size++;
    #undef Table
};

Symbol_s* FindSymbol(const char* SymbolName, Context_s* ArchContext)
{
    for (size_t SymbolIndex = 0; SymbolIndex < ArchContext->Symbols->Size; SymbolIndex++)
    {
        if (strcmp(SymbolName, ArchContext->Symbols->Data[SymbolIndex]->Name) == 0)
        {
            return ArchContext->Symbols->Data[SymbolIndex];
        };
    };
    return NULL;
};

void DefineSymbol(Symbol_s* TargetSymbol, size_t Size, SectionType Section, bool IsDefined, bool IsGlobal, SymbolType Type, Context_s* ArchContext)
{
    TargetSymbol->Size = Size;
    TargetSymbol->Offset = GetBuffer(Section, ArchContext)->Size;
    TargetSymbol->Section = Section;
    TargetSymbol->IsDefined = IsDefined;
    TargetSymbol->IsGlobal = IsGlobal;
    TargetSymbol->Type = Type;
};

Context_s* Context(void)
{
    Context_s* Object = (Context_s*)malloc(sizeof(Context_s));
    if (!Object) Error("Context()", "failed to allocate %zu bytes (for Context_s structure)", sizeof(Context_s));
    Object->Reallocs = (ReallocTable*)calloc(1, sizeof(ReallocTable));
    if (!Object->Reallocs) Error("Context()", "failed to allocate %zu bytes (for ReallocTable structure)", sizeof(ReallocTable));
    Object->Symbols = (SymbolTable*)calloc(1, sizeof(SymbolTable));
    if (!Object->Symbols) Error("Context()", "failed to allocate %zu bytes (for SymbolTable structure)", sizeof(SymbolTable));
    return Object;
};