#ifndef CHEAT_API_H
#define CHEAT_API_H

#include "../Emitter/Emitter.h"
#include "CheatTypes.h"
#include <stdint.h>

typedef struct CheatAPI CheatAPI;

struct CheatAPI
{
    void (*GenerateFile)(const char* Target, const char* OutputName);

#endif