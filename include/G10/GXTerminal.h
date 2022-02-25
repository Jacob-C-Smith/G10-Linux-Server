#pragma once

#include <stdio.h>
#include <string.h>

#include <G10/GXServer.h>
#include <G10/GXClient.h>
#include <G10/GXJSON.h>
#include <G10/GXtypedef.h>
#include <G10/GXBan.h>

// Terminal thread
int process_commands ( GXServer_t* server );
