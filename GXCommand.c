#include <G10/GXCommand.h>
#define MAX_DATA_PER_PACKET 4096

char *parse_packet (GXClient_t *client, char *buffer)
{
    // Argument check
    {
        #ifndef NDEBUG
        if(buffer == (void*)0)
            goto noBuffer;
        #endif
    }
    
    switch (*(u16*)buffer)
    {
        case COMMAND_NOOP:
            break;
        case COMMAND_CONNECT:
        {
            char   *name = &buffer[2];
            size_t  len  = strlen(name);
            client->name = calloc(1+len, sizeof(u8));
            strncpy(client->name, name, len);

            g_print_log("[G10] [Client] %s connected from %s\n", client->name, inet_ntoa(client->clientAddress->sin_addr));

            break;
        }
        case COMMAND_DISPLACE_ROTATE:
        {
            float *x = (float*) &buffer[2],
                  *y = (float*) &buffer[6],
                  *z = (float*) &buffer[10];
            
            g_print_log("[G10] [Command] User %s is at < %f, %f, %f >\n", client->name ,*x,*y,*z);
        }
            break;
        case COMMAND_CHAT:
        {
            /*
                ff 00 0f 12 00  48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 00
                 ID   ch Len    H  e  l  l  o  ,     w  o  r  l  d  !  \0
            */
            u8  channel = *(u8*)  &buffer[2];
            u16 length  = *(u16*) &buffer[3];

            char *channelName = 0;

            const char *chatChannels[4] = {
                "Command",
                "All",
                "Team",
                "Party"
            };

            if ( channel == CHAT_COMMAND )
                channelName = chatChannels[0];
            else if ( channel == CHAT_ALL )
                channelName = chatChannels[1];
            else if ( channel == CHAT_TEAM )
                channelName = chatChannels[2];
            else if ( channel == CHAT_PARTY )
                channelName = chatChannels[3];

            printf("%s: [%x]: %s\n", client->name, channel, &buffer[5]);
        }
            break;
        case COMMAND_DISCONNECT:
        {
            client->connected = false;
            g_print_log("[G10] [Client] %s disconnected\n", client->name);
            break;
        }
        default:
            #ifndef NDEBUG
                g_print_warning("[G10] [Command] Unrecognized command short \"0x%x\" from client \"%s\"\n",*(u16*)buffer, client->name);
            #endif
            break;
    }

    client->recieved_packets++;

    return buffer;

    // Error handling
    {
        noBuffer:
        #ifndef NDEBUG
            g_print_error("[G10] [Command] No buffer provided in call to function %s", __PRETTY_FUNCTION__ );
        #endif
        return 0;
    }
}


char *no_op_packet()
{

}