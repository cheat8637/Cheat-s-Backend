#include "API.h"
#include "Types.h"
#include "Platform.h"
#include "../Emitter/Emitter.h"
#include "../Reporter/Reporter.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static bool IsAlreadyCreated = false;
static const Architecture_s* CurrentArch = NULL;
static const OS_s* CurrentOS = NULL;

static void FreeAPI(CheatAPI* This)
{
    free(This);
};
static void SetOSAPI(const OS_s* NewOS)
{
    if (!NewOS)
    {
        Error("SetOS()", "null pointer passed.");
    };
    CurrentOS = NewOS;
};
static void SetArchAPI(const Architecture_s* NewArch)
{
    if (!NewArch)
    {
        Error("SetArchitecture()", "null pointer passed.");
    };
    CurrentArch = NewArch;
};
static const OS_s* GetOSAPI(void)
{
    return CurrentOS;
};
static const Architecture_s* GetInstrAPI(void)
{
    return CurrentArch;
};
CheatAPI* GetAPI(void)
{
    if (IsAlreadyCreated == true)
    {
        Error("GetAPI()", "you can't have more than 1 API at a time.");
    };
    CheatAPI* Object = (CheatAPI*)malloc(sizeof(CheatAPI));
    if (!Object)
    {
        Error("GetAPI()", "failed to allocate %zu bytes (for CheatAPI structure).", sizeof(CheatAPI));
    };
    Object->Free = FreeAPI;
    Object->SetOS = SetOSAPI;
    Object->SetArchitecture = SetArchAPI;
    Object->GetOS = GetOSAPI;
    Object->GetInstructions = GetInstrAPI;
    IsAlreadyCreated = true;
    return Object;
};