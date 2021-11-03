#include <G10/GXCommand.h>

char *parsePacket (GXClient_t *client, char *buffer)
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

            gPrintLog("[G10] [Client] %s connected from %s\n", client->name, inet_ntoa(client->clientAddress->sin_addr));

            break;
        }
        case COMMAND_DISPLACE_ROTATE:
        {
            float *x = (float*) &buffer[2],
                  *y = (float*) &buffer[6],
                  *z = (float*) &buffer[10];
            
            gPrintLog("[G10] [Command] User %s is at < %f, %f, %f >\n", client->name ,*x,*y,*z);
        }
            break;
        case COMMAND_CHAT:

            break;
        case COMMAND_DISCONNECT:
        {
            client->connected = false;
            gPrintLog("[G10] [Client] %s disconnected\n", client->name);
            break;
        }
        default:
            #ifndef NDEBUG
                gPrintWarning("[G10] [Command] Unrecognized command short \"0x%x\" from client \"%s\"\n",*(u16*)buffer, client->name);
            #endif
            break;
    }

    return buffer;

    // Error handling
    {
        noBuffer:
        #ifndef NDEBUG
            gPrintError("[G10] [Command] No buffer provided in call to function %s", __PRETTY_FUNCTION__ );
        #endif
        return 0;
    }
}

char *assemblePacket ( GXClient_t *client, char *buffer )
{
    
}

char *NoopPacket()
{

}