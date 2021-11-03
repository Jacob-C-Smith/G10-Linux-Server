#pragma once

#include <G10/GXtypedef.h>
#include <G10/GXCommand.h>
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXClient.h>

enum GXCommands {
    COMMAND_NOOP            = 0x0000,
    COMMAND_CONNECT         = 0x0077,
    COMMAND_DISPLACE_ROTATE = 0x7700,
    COMMAND_CHAT            = 0x00FF,
    COMMAND_DISCONNECT      = 0xFF00
};

/*┌─────────────────────┬──────────────────────────────────────────────────────────────────────┬────────┐
 *│ Command name        │ Command description                                                  │ Code   │
 *├─────────────────────┼──────────────────────────────────────────────────────────────────────┼────────┤
 *│ No Op               │ No operation, i.e. does nothing                                      │ 0x0000 │
 *│ Connect             │ Requires a name. Permits a list of player names in the clients party │ 0x0077 │
 *│ Displace and Rotate │                                                                      │ 0x7700 │
 *│                     │                                                                      │        │
 *│ Chat                │ First byte corresponds to chat channel. Everything else it text      │ 0x00FF │
 *│ Disconnect          │ Removes the player from the server.                                  │ 0xFF00 │
 *└─────────────────────┴──────────────────────────────────────────────────────────────────────┴────────┘
*/

char *parsePacket    ( GXClient_t *client, char *buffer );
char *assemblePacket ( GXClient_t *client, char *buffer );
char *NoopPacket     ( );