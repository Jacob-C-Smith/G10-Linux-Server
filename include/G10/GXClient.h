#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <G10/GXtypedef.h>

struct GXClient_s
{
    char               *name;
    time_t              joinTime;
    struct sockaddr_in *clientSocket;
    struct GXClient_s  *next;
};

typedef struct GXClient_s GXClient_t;

GXClient_t* createClient  ( );
int         destroyClient ( GXClient_t* client );