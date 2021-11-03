#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <G10/GXtypedef.h>
#include <G10/GXClient.h>
#include <G10/GXJSON.h>

#define WAIT_SIZE 16

struct GXServer_s
{
    char               *name,
                       *password;
	int                 listenSocket,
                        playerCount,
                        maxPlayers;
    time_t              startTime;
    struct sockaddr_in *serverAddress;
    GXClient_t         *clients;
    pthread_t           listeningThread;

    struct GXServer_s  *next;
};
 
GXServer_t* createServer         ( );                                                           // ✅ Creates an empty server

GXServer_t* loadServer           ( const char* path );                                          // ✅ Loads a server from a path
GXServer_t* loadServerAsJSON     ( char*       token );                                         // ✅ Loads a server from a JSON token

int         listenForConnections ( GXServer_t* server );                                        // ✅ Start in a new thread. Listens for connections to server. Creates, populates, and appends a new GXClient on connection.

int         appendClient         ( GXServer_t* server, GXClient_t* client );                    // ✅ Appends a client to a servers client list

GXClient_t* getClient            ( GXServer_t* server, const char* name );                      // ✅ Finds a client by name

int         kickClient           ( GXServer_t *server, const char* name, char* message );       // ✅ Kicks a client off with a custom error message

int         removeClient         ( GXServer_t* server, const char* name );                      // ✅ Removes a client from the server list

int         destroyServer        ( GXServer_t* server );                                        // ✅ Destroys a server