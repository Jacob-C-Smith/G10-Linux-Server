#include <G10/GXServer.h>

GXServer_t* createServer  ( )
{
    GXServer_t* ret = malloc(sizeof(GXServer_t));

    ret->name       = (void*)0;
    ret->clients    = (void*)0;

    return ret;
}

GXServer_t* loadServer ( const char path[] ) 
{
    // Initialized data
    FILE* f;
    size_t i;
    char* data;

    // Load the file
    {
        // Check if file is valid
        if (f == NULL)
        {
            printf("Failed to load file %s\n", path);
            return (void*)0;
        }

        // Find file size and prep for read
        fseek(f, 0, SEEK_END);
        i = ftell(f);
        fseek(f, 0, SEEK_SET);

        // Allocate data and read file into memory
        data = malloc(i+1);
        if (data == 0)
            return (void*)0;
        fread(data, 1, i, f);

        // We no longer need the file
        fclose(f);

        // For reasons beyond me, the null terminator isn't included.
        data[i] = '\0';
    }

    return loadServerAsJSON(data);
}

GXServer_t* loadServerAsJSON ( char* token )
{
    GXServer_t* ret = createServer();
    
    return ret;
}


int destroyServer ( GXServer_t* server )
{
    free(server->name);

    GXClient_t* i = server->clients;
    while(i)
    {
        GXClient_t* j = i;
        i = i->next;
        destroyClient(j);
    }

    free(server);

    return 0;    
}