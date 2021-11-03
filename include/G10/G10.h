#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

size_t gLoadFile     ( const char       *path,   void            *buffer );    // ✅ Loads a file and reads it into buffer. If buffer is null, function will return size of file, else returns bytes written.

int    gPrintError   ( const char *const format, ... );                        // ✅ gPrintError is printf, but in red
int    gPrintWarning ( const char *const format, ... );                        // ✅ gPrintWarning is printf, but in yellow
int    gPrintGray    ( const char *const format, ... );
int    gPrintLog     ( const char *const format, ... );                        // ✅ gPrintLog is printf, but in blue
