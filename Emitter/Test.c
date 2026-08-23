#include "Emitter.h"
#include "../CheatAPI/Types.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    Emitter_s* Test = Emitter();
    Buffer_s* TestB = Buffer();
    Test->Byte(0x67, TestB);
    uint8_t Data[] = {0x6A, 0x6C, 0xF0, 0x71};
    Test->Bytes(Data, 4, TestB);
    for (size_t i = 0; i < TestB->Size; i++)
    {
        printf("%02X ", TestB->Data[i]);
    };
    printf("\n");
    TestB->Free(TestB);
    Test->Free();
    return 0;
};