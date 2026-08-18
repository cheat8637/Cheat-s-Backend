#include "Reporter.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void Error(const char* Function, const char* Format, ...)
{
    va_list Arguments;
    
    va_start(Arguments, Format);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, Format, Arguments);
    fprintf(stderr, "\nIn function: \"%s\";", Function);
    va_end(Arguments);
    
    exit(1);
};