#include <G10/G10.h>

size_t gLoadFile(const char* path, void* buffer)
{
    // Argument checking 
    {
        #ifndef NDEBUG
            if(path==0)
                goto noPath;
        #endif
    }

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, "r");
    
    // Check if file is valid
    if (f == NULL)
        goto invalidFile;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if(buffer)
        ret = fread(buffer, 1, ret, f);

    // We no longer need the file
    fclose(f);
    
    return ret;

    // Error handling
    {
        noPath:
        #ifndef NDEBUG
            gPrintError("[G10] Null path provided to funciton \"%s\\n",__PRETTY_FUNCTION__);
        #endif
        return 0;
        
        invalidFile:
        #ifndef NDEBUG
            gPrintError("[G10] Failed to load file \"%s\"\n",path);
        #endif
        return 0;
    }
}

int gPrintError( const char* const format, ... )
{
    // We use the varadic argument list in vprintf
    va_list aList;
    va_start(aList, format);

    // Uses ANSI terminal escapes to set the color to red, 
    // print the message, and restore the color.
    putchar('\r');
    printf("\033[91m");
    vprintf(format, aList);
    printf("\033[0m");

    va_end(aList);

    return 0;
}

int gPrintWarning(const char* const format, ...)
{
    // We use the varadic argument list in vprintf
    va_list aList;
    va_start(aList, format);

    // Uses ANSI terminal escapes to set the color to yellow, 
    // print the message, and restore the color.
    putchar('\r');
    printf("\033[93m");
    vprintf(format, aList);
    printf("\033[0m");

    va_end(aList);

    return 0;
}

int gPrintGray ( const char* const format, ... )
{
    // We use the varadic argument list in vprintf
    va_list aList;
    va_start(aList, format);

    // Uses ANSI terminal escapes to set the color to light blue, 
    // print the message, and restore the color.
    putchar('\r');
    printf("\033[90m");
    vprintf(format, aList);
    printf("\033[0m");

    va_end(aList);

    return 0;
}

int gPrintLog ( const char* const format, ... )
{
    // We use the varadic argument list in vprintf
    va_list aList;
    va_start(aList, format);

    // Uses ANSI terminal escapes to set the color to light blue, 
    // print the message, and restore the color.
    putchar('\r');
    printf("\033[94m");
    vprintf(format, aList);
    printf("\033[0m");

    va_end(aList);

    return 0;
}
