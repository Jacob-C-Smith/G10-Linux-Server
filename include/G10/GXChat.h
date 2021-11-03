#include <G10/GXtypedef.h>

enum GXChatChannels {
    CHAT_PARTY          = 0xFF,
    CHAT_TEAM           = 0xF0,
    CHAT_ALL            = 0x0F,
    CHAT_COMMAND        = 0x00
};
typedef enum GXChatChanels chatChannel_t;

int sendChat ( GXClient_t *sender, chatChannel_t channel );