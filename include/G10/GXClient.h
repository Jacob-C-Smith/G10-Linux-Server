#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXJSON.h>
#include <G10/GXCommand.h>

#define MAX_BUFFER_SIZE 4096

struct GXClient_s
{
    char               *name;
    time_t              joinTime;
    size_t              clientAddressLength;
    struct sockaddr_in *clientAddress;
    pthread_t           clientThread;
    int                 clientSocket;
    bool                connected;
    u8                  inBuffer[MAX_BUFFER_SIZE];
    struct GXClient_s  *next;
};

GXClient_t* createClient  ( );
GXClient_t* acceptClient  ( int         listenSocket );
int         clientThread  ( GXClient_t *client );
int         declineClient ( GXClient_t *client      , const char* message )
int         TCPRecv       ( GXClient_t *client      , char       *buffer , size_t  len );
int         TCPSend       ( GXClient_t *client      , char       *buffer , size_t  len );
int         destroyClient ( GXClient_t *client );