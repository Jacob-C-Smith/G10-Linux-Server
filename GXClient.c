#include <G10/GXClient.h>

GXClient_t* createClient  ( )
{
    // Initialized data
    GXClient_t* ret                    = calloc(1, sizeof(GXClient_t));

    // Zero set everything
    ret->name                          = (void*)0;
    ret->joinTime                      = 0;

    // Initialize the client address
    ret->clientAddress                  = calloc(1, sizeof(struct sockaddr_in));
    ret->clientAddress->sin_addr.s_addr = 0;
    ret->clientAddress->sin_family      = 0;
    ret->clientAddress->sin_port        = 0;

    // Initialize the client thread
    ret->clientThread                  = 0;

    // Set next to nullptr
    ret->next                          = 0;

    return ret;
}

GXClient_t* acceptClient ( GXServer_t *server )
{
    // Uninitialized data
    size_t       len;
    size_t       tokenCount;
    JSONValue_t* tokens;   

    // Initiailzed data
    GXClient_t* ret    = createClient();
    char*       buffer = calloc(MAX_BUFFER_SIZE,sizeof(char));

    // Accept a connection and create a new socket
    ret->clientSocket = accept(server->listenSocket, ret->clientAddress, &ret->clientAddressLength );

    // Check the socket
    if(ret->clientSocket < 0)
        goto noSocket;

    gPrintLog("[G10] [Client] Connection from %s\n", inet_ntoa(ret->clientAddress->sin_addr));
    
    // Set the join time
    ret->joinTime = time(NULL);

    // Get the clients message
    while(ret->name == 0)
    {
        recv(ret->clientSocket, buffer, MAX_BUFFER_SIZE,0);

        parsePacket(ret, buffer);
    }

    ret->connected = true;

    // Create a new thread for listening
    ret->clientThread = pthread_create(&ret->clientThread,NULL,&clientThread,ret);
    
    return ret;
    noSocket:
        gPrintWarning("[G10] [Client] Couldn't create a socket for client\n");
        return 0;
}

int clientThread (GXClient_t *client)
{
    while(client->connected)
    {
        recv(client->clientSocket, client->inBuffer, MAX_BUFFER_SIZE, 0);

        parsePacket(client, client->inBuffer);
        memset(client->inBuffer, 0, MAX_BUFFER_SIZE);
    }
    return 0;
}

int declineClient ( GXClient_t *client, const char* message )
{
   tcp
}

int TCPRecv ( GXClient_t* client, char* buffer, size_t len ) {
    int ret;
    while ((ret = recv(client->clientSocket, buffer, len, 0)) > 0)
    {
        #ifndef NDEBUG
            printf ("\033[34mRecieved a %ld byte long message from client %s:\033[m\n",strlen(buffer),inet_ntoa(client->clientAddress->sin_addr));
            for (size_t i = 0; i < ret; i++)
	    	{
    			if(i%16==0)
		        	putchar('\n');
		    	int a = (int) buffer[i];
	    		printf("\033[34m%.02X \033[m", a & 0xFF);

    		}
		    printf("\n\n");
        #endif
    }
    return ret;
}

int TCPSend ( GXClient_t* client, char* buffer, size_t len )
{
    return send(client->clientSocket, buffer,len,0);			
}


int destroyClient ( GXClient_t* client )
{
    // Free the name
    free(client->name);

    // Zero set everything
    client->joinTime                       = 0;

    // Zero set the address
    client->clientAddressLength            = 0;

    client->clientAddress->sin_addr.s_addr = 0;
    client->clientAddress->sin_family      = 0;
    client->clientAddress->sin_port        = 0;

    // Close the socket
    shutdown(client->clientSocket, SHUT_RDWR);

    // Free the address
    free(client->clientAddress);

    client->next     = 0;

    // Free the client
    free(client);

    return 0;
}