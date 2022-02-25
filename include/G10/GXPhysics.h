#pragma once
#include <stdio.h>

#include <unistd.h>
#include <time.h>

#include <G10/GXtypedef.h>
#include <G10/GXServer.h>

// This thread counts ticks at a specified time interval 
void tick_thread ( GXServer_t *server, size_t ticks_per_sec );
