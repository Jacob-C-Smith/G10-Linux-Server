#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <G10/GXtypedef.h>
#include <G10/GXClient.h>

struct GXServer_s
{
    char               *name;
    time_t              upTime;
    GXClient_t         *clients;
};

typedef struct GXServer_s GXServer_t;

GXServer_t* createServer     ( );
GXServer_t* loadServer       ( const char path[] ) ;
GXServer_t* loadServerAsJSON ( char* token ) ;
int         appendClient     ( GXServer_t* server, GXClient_t* client );
int         destroyServer    ( GXServer_t* client );