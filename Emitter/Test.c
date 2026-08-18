#include "Emitter.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    Emitter_s* Test = Emitter();
    Test->Byte(0x67);
    uint8_t Data[] = {0x6A, 0x6C, 0xF0, 0x71};
    Test->Bytes(Data, 4);
    for (size_t i = 0; i < Test->Size; i++)
    {
        printf("%02X ", Test->Buffer[i]);
    };
    printf("\n");
    Test->Free();
};