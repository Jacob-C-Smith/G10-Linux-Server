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
#include <G10/GXServer.h>
#include <G10/GXCommand.h>

#define MAX_BUFFER_SIZE 4096

struct GXClient_s
{
    // Is the client connected?
    bool                connected;

    // How many packets have we sent?
    size_t              recieved_packets;
    u8                  inBuffer[MAX_BUFFER_SIZE];
    int                 clientSocket;

    // When did the client join
    time_t              joinTime;
    size_t              clientAddressLength;

    // Name
    char               *name,
                       *chatBuffer;

    // Socket related 
    struct sockaddr_in *clientAddress;
    pthread_t           client_thread;

    // Command queues
    void        **input,
                **output;
    size_t        input_front, 
                  input_rear,
                  output_front,
                  output_rear;

    // next 
    struct GXClient_s  *next;
};

// Allocators
GXClient_t  *create_client    ( void );

// Client connection operations
GXClient_t  *accept_client    ( GXServer_t *server );

// Client thread
int          client_thread    ( GXClient_t *client );

// Queue operations
int          enqueue_command ( GXClient_t *client, void        *command ); 
void        *dequeue_command ( GXClient_t *client );


// Socket operations
int          flush_commands  ( GXClient_t *client ); 
int          recv_commands   ( GXClient_t *client );       

// TCP operations
int          tcp_recv         ( GXClient_t *client, char       *buffer , size_t  len );
int          tcp_send         ( GXClient_t *client, char       *buffer , size_t  len );

// Destructors
int          destroy_client   ( GXClient_t *client );