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
#include <G10/GXPhysics.h>

#include <G10/GXEntity.h>

#define WAIT_SIZE 16

struct GXServer_s
{
    char               *name,
                       *password;
    bool                running;
	int                 listenSocket,
                        playerCount,
                        maxPlayers;
    time_t              startTime;
    struct sockaddr_in *serverAddress;
    GXClient_t         *clients;
    pthread_t           listeningThread,
                        tickThread;

    size_t              ticks,
                        tick_rate;

    // Actors are dynamic entites
    GXEntity_t        **actors;

    struct GXServer_s  *next;
};

// Allocators
GXServer_t* create_server         ( void );                                                           // ✅ Creates an empty server

// Constructors
GXServer_t* load_server           ( const char* path );                                          // ✅ Loads a server from a path
GXServer_t* load_server_as_json     ( char*       token );                                         // ✅ Loads a server from a JSON token

// Listener thread
int         listen_for_connections ( GXServer_t* server );                                        // ✅ Start in a new thread. Listens for connections to server. Creates, populates, and appends a new GXClient on connection.

// Appenders
int         append_client         ( GXServer_t* server, GXClient_t* client );                    // ✅ Appends a client to a servers client list

// Getters
GXClient_t* get_client            ( GXServer_t* server, const char* name );                      // ✅ Finds a client by name

// Client operations
int         kick_client           ( GXServer_t *server, const char* name, char* message );       // ✅ Kicks a client off with a custom error message

// Removers
int         remove_client         ( GXServer_t* server, const char* name );                      // ✅ Removes a client from the server list

// Destructors
int         destroy_server        ( GXServer_t* server );                                        // ✅ Destroys a server