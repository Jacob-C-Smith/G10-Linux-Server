#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include <time.h>


// Load a file
size_t g_load_file     ( const char       *path,   void            *buffer );    // ✅ Loads a file and reads it into buffer. If buffer is null, function will return size of file, else returns bytes written.

// printf with ANSI colors
int    g_print_error   ( const char *const format, ... );                        // ✅ g_print_error is printf, but in red
int    g_print_warning ( const char *const format, ... );                        // ✅ g_print_warning is printf, but in yellow
int    g_print_gray    ( const char *const format, ... );
int    g_print_log     ( const char *const format, ... );                        // ✅ g_print_log is printf, but in blue
