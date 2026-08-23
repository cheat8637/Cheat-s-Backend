#ifndef CHEAT_API_H
#define CHEAT_API_H

#include "../Emitter/Emitter.h"
#include "Platform.h"
#include "Types.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct CheatAPI CheatAPI;

struct CheatAPI
{
    void (*Free)(CheatAPI* This);
    void (*SetOS)(const OS_s* NewOS);
    void (*SetArchitecture)(const Architecture_s* NewArch);
    const OS_s* (*GetOS)(void);
    const Architecture_s* (*GetInstructions)(void);
};

CheatAPI* GetAPI(void);

#endif