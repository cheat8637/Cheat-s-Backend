#include "../Reporter/Reporter.h"
#include "Platform.h"
#include "Types.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static Architecture_s** Architectures = NULL;
static size_t ACapacity = 0;
static size_t ASize = 0;

static OS_s** OSs = NULL;
static size_t OCapacity = 0;
static size_t OSize = 0;

void AddArchitecture(Architecture_s* Architecture)
{
    if (ASize >= ACapacity)
    {
        ACapacity = (ACapacity == 0) ? 2 : ACapacity + 2;
        Architecture_s** NewArch = realloc(Architectures, ACapacity * sizeof(Architecture_s*));
        if (!NewArch)
        {
            free(Architectures);
            Error("AddArchitecture()", "failed to allocate %zu bytes (realloc for Architectures dynamic array).", ACapacity * sizeof(Architecture_s*));
        };
        Architectures = NewArch;
    }
    Architectures[ASize] = Architecture;
    ASize++;
};
const Architecture_s* GetArchitecture(const char* Name)
{
    for (size_t ArchIndex = 0; ArchIndex < ASize; ArchIndex++)
    {
        if (strcmp(Architectures[ArchIndex]->Name, Name) == 0)
        {
            return Architectures[ArchIndex];
        };
    };
    return NULL;
};
void AddOS(OS_s* OS)
{
    if (OSize >= OCapacity)
    {
        OCapacity = (OCapacity == 0) ? 2 : OCapacity + 2;
        OS_s** NewOS = realloc(OSs, OCapacity * sizeof(OS_s*));
        if (!NewOS)
        {
            free(OSs);
            Error("AddOS()", "failed to allocate %zu bytes (for OS_s dynamic array).", OCapacity * sizeof(OS_s*));
        };
        OSs = NewOS;
    };
    OSs[OSize] = OS;
    OSize++;
};
const OS_s* GetOS(const char* Name)
{
    for (size_t OSIndex = 0; OSIndex < OSize; OSIndex++)
    {
        if (strcmp(OSs[OSIndex]->Name, Name) == 0)
        {
            return OSs[OSIndex];
        };
    };
    return NULL;
};