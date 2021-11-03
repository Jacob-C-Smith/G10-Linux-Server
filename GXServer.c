#include <G10/GXServer.h>

GXServer_t* createServer  ( )
{
    GXServer_t* ret = calloc(1, sizeof(GXServer_t));

    return ret;
}

GXServer_t* loadServer ( const char path[] ) 
{
    // Initialized data
    FILE*  f = fopen(path,"rb");
    size_t i;
    char*  data;

    // Load the file
    {
        // Check if file is valid
        if (f == NULL)
            goto invalidFile;

        // Find file size and prep for read
        fseek(f, 0, SEEK_END);
        i = ftell(f);
        fseek(f, 0, SEEK_SET);

        // Allocate data and read file into memory
        data = malloc(i+1);
        if (data == 0)
            return (void*)0;
        fread(data, 1, i, f);

        // We no longer need the file
        fclose(f);

        // For reasons beyond me, the null terminator isn't included.
        data[i] = '\0';
    }

    return loadServerAsJSON(data);
    invalidFile:
    
        printf("\033[31m[G10] [Server] Failed to load file \"%s\"\033[m\n", path);
      
    return 0;
}

GXServer_t* loadServerAsJSON ( char* token )
{
    // Initialized data
    GXServer_t*  ret        = createServer();
    size_t       len        = strlen(token);
    size_t       tokenCount = GXParseJSON(token, len, 0, (void*)0);
    JSONValue_t* tokens     = calloc(tokenCount, sizeof(JSONValue_t));

    // Parse the camera object
    GXParseJSON(token, len, tokenCount, tokens);

    // Set default values
    ret->serverAddress                  = malloc(sizeof(struct sockaddr_in));
	ret->serverAddress->sin_family      = AF_INET;
	ret->serverAddress->sin_port        = htons(0);
	ret->serverAddress->sin_addr.s_addr = htonl(INADDR_ANY);
	
    /* JSON Example 
    {
        "name"       : "localhost",
        "host"       : "127.0.0.1",
        "port"       : 8877,
        "maxPlayers" : 16,
        "blacklist"  : [
            "11.20.22.39",
            "50.69.131.133",
            "168.183.212.227"
        ],
        "whitelist"  : [
            "184.189.190.201",
            "0.69.75.82",
            "89.107.115.143"
        ]
    }
    */

    // Search through values and pull out relevent information
    for (size_t j = 0; j < tokenCount; j++)
    {
        // Set name
        if (strcmp("name", tokens[j].name) == 0)
        {
            // Initialized data
            size_t len = strlen(tokens[j].content.nWhere)+1;

            // Allocate space for the name
            ret->name = malloc(len+1);
            
            // Copy the name
            strncpy(ret->name, tokens[j].content.nWhere, len);
            ret->name[len] = '\0';

            continue;
        }

        // Set host
        else if (strcmp("host", tokens[j].name) == 0)
        {
            /*
            // Uninitialized data
            u8  a,
                b,
                c,
                d;
            
            // Initialized data
            u32 ip = 0;

            // Copy out the bytes of the IP address
            sscanf(tokens[j].name,"%u.%u.%u.%u",&a,&b,&c,&d);

            // Reconstruct the IP as a 32 bit 
            ip =  d;
            ip |= c << 8;
            ip |= b << 16;
            ip |= a << 24;

            // Set the IP address in the socket
            ret->serverAddress->sin_addr.s_addr = htonl(ip);
            */
            continue;
        }

        // Set port
        else if (strcmp("port", tokens[j].name) == 0)
        {
            ret->serverAddress->sin_port = htons(atoi(tokens[j].content.nWhere));
            continue;
        }

        // Set maxPlayers
        else if (strcmp("max players", tokens[j].name) == 0)
        {
            ret->maxPlayers = atoi(tokens[j].content.nWhere);
            continue;
        }

        // Set password
        else if (strcmp("password", tokens[j].name)==0)
        {
            size_t len    = strlen(tokens[j].content.nWhere;
            ret->password = calloc(len+1, sizeof(u8));
            strncpy(ret->password, tokens[j].content.nWhere, len);
        }

        // Set blacklist
        else if (strcmp("bans", tokens[j].name) == 0)
        {
            // TODO: Process bans as an array of objects
            
            continue;
        }
    }

    // TODO: Make sure we got everything we needed from the JSON

    // Initialize the server
	{
		// Create a listen socket
		if ((ret->listenSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
			goto noSocket;

		retryBind:
		// Bind the socket
		if ((bind(ret->listenSocket,(const struct sockaddr *) ret->serverAddress, sizeof(struct sockaddr_in))) < 0)
			goto noBind;

		// Start listening on the socket
		if (listen(ret->listenSocket, WAIT_SIZE) < 0)
			goto noListening;

        // Set the start time
        time(&ret->startTime);

        // Start a new listening thread
        pthread_create(&ret->listeningThread,NULL, (void*)&listenForConnections,ret);

        printf("\033[34m[G10] [Server] Started a listening thread\n\033[m");

	}
    
    return ret;

    // Couldn't make a socket
    noSocket:
        gPrintError("[G10] [Server] A listen socket could not be created\n");
    return 0;

    // Couldn't bind the socket
    noBind:
        gPrintError("[G10] [Server] The listening socket was unable to bind\n");
    return 0;

    // Couldn't start the socket
    noListening:
        gPrintError("[G10] [Server] The listening socket could not be started\n");
    return 0;
}

int listenForConnections ( GXServer_t* server )
{
    gPrintLog("[G10] [Server] Listening for new TCP connections on port %d\n",htons(server->serverAddress->sin_port));
    while(1)
    {
        GXClient_t *client  = server->clients;
        
        GXClient_t *pClient = acceptClient( server );

        if( server->playerCount > server->maxPlayers)
            declineClient(pClient, "The server is currently full\n");
            

        appendClient(server, );

        while(client)
        {
            if(client->connected == false)
                removeClient(server, client->name);
            client = client->next;   
        }
    }
}

int appendClient ( GXServer_t* server, GXClient_t* client )
{
    // Set the pointer to the head of the linked list
    GXClient_t* i = server->clients;

    // Check if the head is null. If so, set the head to the client
    if (i == 0)
    {
        server->clients = client;
        return 0;
    }

    // Search for the end of the linked list, and check every entity 
    while (i->next)
    {
        // Error checking
        #ifndef NDEBUG
        if (strcmp(i->name, client->name) == 0)
            goto duplicateName;
        #endif	
        i = i->next;

    }

    // Assign next as client
    i->next = client;

    // And increment the player count
    server->playerCount++;

    return 0;
    
    duplicateName:
    #ifndef NDEBUG
        printf("\033[31mClient \"%s\" can not be appended to \"%s\" because an entity with that name already exists\033[m\n", client->name, server->name);
    #endif    
    return 0;
}

GXClient_t* getClient ( GXServer_t *server, const char* name )
{
    // Create a pointer to the head of the list
    GXClient_t* i = server->clients;

    // Sanity check
    if (i == 0)
        goto noClients;
    
    // Iterate through list until we hit the entity we want, or zero
    while (i)
    {
        if (strcmp(name, i->name) == 0)
            return i; // If able to locate the entity in question, return a pointer
        i = i->next;
    }
    
    // Unable to locate entity
    goto noMatch;
    noClients:
    #ifndef NDEBUG
        printf("\033[31mThere are no clients\033[m.\n");
    #endif
    return 0;	

    noMatch:
    #ifndef NDEBUG
        printf("\033[31mThere is no client named \"%s\".\033[m\n", name);
    #endif
    return 0;
}

int kickClient ( GXServer_t *server, const char* name, char* message )
{
    // Get a pointer to the GXClient struct
    GXClient_t* client = getClient(server,name);

    // Notify the user that they have been banned
    TCPSend(client,message,strlen(message)+1);

    // Remove the client from the server list 
    removeClient(server, name);

    // Destroy the client
    destroyClient(client);
    
    // Return
    return 0;
}

int removeClient ( GXServer_t *server, const char* name )
{
    // Create a pointer to the head of the list
    GXClient_t* i = server->clients;
    
    // Quick sanity check
    if (i == 0)
        goto noClients;
    
    // Check the head
    if (strcmp(name, i->name) == 0)
    {
        server->clients = 0;
        return 0;
    }

    // Destroy the named client
    while (i->next)
    {
        if (strcmp(name, i->next->name) == 0) 
        {
            // Make a copy of the client that is 2 positions ahead of the current client
            GXClient_t* j = i->next->next;
            
            // Verbose logging
            #ifndef NDEBUG
                gPrintLog("Removed Client \"%s\" from server \"%s\"\n", name, server->name);
            #endif
            
            // Stitch up the linked list 
            i->next = j;

            // And decrement the player count
            server->playerCount--;

            return 0;
        }
        i = i->next;
    }

    goto noMatch;

    // There are no clients on the server at all
    noClients:
    #ifndef NDEBUG
        gPrintLog("There are no clients in \"%s\".\n", server->name);
    #endif
    return 0;

    // There is no matching client
    noMatch:
    #ifndef NDEBUG
        gPrintWarning("There is no client on \"%s\" named \"%s\".\n", server->name, name);
    #endif
    return 0;
}

int destroyServer ( GXServer_t* server )
{
    // Free the name
    free(server->name);

    // Close the listening socket
    shutdown(server->listenSocket, SHUT_RDWR);

    // Zero set all
    server->playerCount                    = 0;
    server->maxPlayers                     = 0;
    server->startTime                      = 0;

    // Free the address
    server->serverAddress->sin_addr.s_addr = 0;
    server->serverAddress->sin_family      = 0;
    server->serverAddress->sin_port        = 0;

    free(server->serverAddress);

    // Remove all the clients from the server
    GXClient_t* i = server->clients;
    while(i)
    {
        GXClient_t* j = i;
        i = i->next;
        destroyClient(j);
    }

    server->clients                        = 0;

    // Free the server
    free(server);

    return 0;    
}