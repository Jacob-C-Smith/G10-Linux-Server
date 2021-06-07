#include <G10/GXClient.h>

GXClient_t* createClient  ( )
{
    // Initialized data
    GXClient_t* ret = malloc(sizeof(GXClient_t));

    // Zero set everything
    ret->name       = (void*)0;
    ret->joinTime   = 0;

    // Initialize the client address
    ret->clientSocket                  = malloc(sizeof(struct sockaddr_in));
    ret->clientSocket->sin_addr.s_addr = 0;
    ret->clientSocket->sin_family      = 0;
    ret->clientSocket->sin_port        = 0;

    ret->next       = 0;

    return ret;
}

// GXClient_t* accept

int destroyClient ( GXClient_t* client )
{
    client->name     = (void*)0;
    client->joinTime = 0;


    client->clientSocket->sin_addr.s_addr = 0;
    client->clientSocket->sin_family      = 0;
    client->clientSocket->sin_port        = 0;

    free(client->clientSocket);

    client->next     = 0;

    free(client);

    return 0;
}