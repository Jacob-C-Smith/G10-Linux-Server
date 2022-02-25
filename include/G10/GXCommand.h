#pragma once

#include <G10/GXtypedef.h>
#include <G10/GXCommand.h>
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXClient.h>
#include <G10/GXChat.h>

enum GXCommands {
    COMMAND_NOOP                    = 0x0000, // 0000 0000 0000 0000
    COMMAND_ACTOR_INIT              = 0x0033, // 0000 0000 0011 0011 
    COMMAND_ACTOR_LOC_ROT           = 0x0055, // 0000 0000 0101 0101
    COMMAND_CONNECT                 = 0x0077, // 0000 0000 0111 0111
    COMMAND_DISPLACEMENT_DERIVATIVE = 0x00AA, // 0000 0000 1010 1010
    COMMAND_D                       = 0x00CC, // 0000 0000 1100 1100
    COMMAND_CHAT                    = 0x00FF, // 0000 0000 1111 1111
    COMMAND_X                       = 0x3300, // 0011 0011 0000 0000
    COMMAND_W                       = 0x5500, // 0101 0101 0000 0000
    COMMAND_DISPLACE_ROTATE         = 0x7700, // 0111 0111 0000 0000
    COMMAND_NET_STAT                = 0xAA00, // 1010 1010 0000 0000
    COMMAND_ACTOR_REMOVE            = 0xCC00, // 1100 1100 0000 0000
    COMMAND_DISCONNECT              = 0xFF00  // 1111 1111 0000 0000
};
/*┌────────────────────────────────┬──────────────────────────────────────────────────────────────────────┬────────┐
 *│ Command name                   │ Command description                                                  │ Code   │
 *├────────────────────────────────┼──────────────────────────────────────────────────────────────────────┼────────┤
 *│ No Operation                   │ Does nothing                                                         │ 0x0000 │
 *│ Initialize actor               │ Tell the client to start keeping track of an actor                   │ 0x0033 │
 *│ Actor location and rotation    │ Update the location and roatation of an actor                        │ 0x0055 │
 *│ Connect                        │ Requires a name. Permits a list of player names in the clients party │ 0x0077 │
 *│ Actor displacement derivatives │ Update displacement derivatives of an actor                          │ 0x00AA │
 *│ Chat                           │ First byte corresponds to chat channel. Everything else it text      │ 0x00FF │
 *│ Displace and rotate            │                                                                      │ 0x7700 │
 *│ Remove actor                   │ Tells a client to stop keeping track of an actor                     │ 0xCC00 │
 *│ Disconnect                     │ Removes the player from the server.                                  │ 0xFF00 │
 *└────────────────────────────────┴──────────────────────────────────────────────────────────────────────┴────────┘
*/

char *parse_packet                         ( GXClient_t *client, char *buffer );
char *assemble_packet                      ( GXClient_t *client, char *buffer );

int   encodeNoOpInstruction               ( char *buffer );
int   encodeConnectInstruction            ( char *buffer, GXClient_t *client );
int   encodeEntityDisplacementInstruction ( char *buffer );
