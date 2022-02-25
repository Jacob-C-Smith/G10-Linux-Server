#include <G10/GXClient.h>

GXClient_t  *create_client  ( )
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
    ret->client_thread                  = 0;

    // Set next to nullptr
    ret->next                          = 0;

    return ret;
}

GXClient_t  *accept_client ( GXServer_t *server )
{
    // Uninitialized data
    size_t       len;
    size_t       tokenCount;
    JSONValue_t* tokens;   

    // Initiailzed data
    GXClient_t* ret    = create_client();
    char*       buffer = calloc(MAX_BUFFER_SIZE,sizeof(char));

    // Accept a connection and create a new socket
    ret->clientSocket = accept((int)server->listenSocket, ret->clientAddress, &ret->clientAddressLength );

    // Check the socket
    if(ret->clientSocket < 0)
        goto noSocket;

    g_print_log("[G10] [Client] Connection from %s\n", inet_ntoa(ret->clientAddress->sin_addr));
    
    // Set the join time
    ret->joinTime = time(NULL);

    ret->connected = true;

    // Create a new thread for listening
    ret->client_thread = pthread_create(&ret->client_thread,NULL,&client_thread,ret);
    
    return ret;
    noSocket:
        g_print_warning("[G10] [Client] Couldn't create a socket for client\n");
        return 0;
}

int          client_thread (GXClient_t *client)
{

    // Run while the client is connected
    while ( client->connected )
    {
        // Grab the commands from the socket
        recv_commands(client);
        
        // Parse the commands
        parse_packet(client, client->inBuffer);

        // Flush the command buffer to the socket
        flush_commands(client);

        // Null out the buffer
        memset(client->inBuffer, 0, MAX_BUFFER_SIZE);
    }

    // Log the client's disconnection
    g_print_log("[G10] [Client] Client \"%s\" disconnected\n",(client->name) ? client->name : "[No name]");

    return 0;
}

int          flush_commands            ( GXClient_t *client )
{
    
    return 0;
} 

int          recv_commands             ( GXClient_t *client )
{
    recv(client->clientSocket, client->inBuffer, MAX_BUFFER_SIZE, 0);
    return 0;
}   

int          tcp_recv ( GXClient_t* client, char* buffer, size_t len ) {
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
        
int          tcp_send ( GXClient_t* client, char* buffer, size_t len )
{
    return send(client->clientSocket, buffer,len,0);			
}
        
int          destroy_client ( GXClient_t* client )
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