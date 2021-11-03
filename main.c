// standard library
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

// POSIX sockets, symbolic constants, and internet operations
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// G10 includes
#include <G10/GXtypedef.h>
#include <G10/GXServer.h>
#include <G10/GXClient.h>
#include <G10/GXJSON.h>
#include <G10/GXTerminal.h>

int main(int argc, const char* argv[]) {

	// Uninitialized data
	GXServer_t* server;
	pthread_t   terminal;

	// Initialized data
	char*       serverPath = 0;

	// Parse command line arguments
    {
		// Iterate through the argument vector
        for (size_t i = 1; i < argc; i++)
        {
			// Help
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            {
				printf("G10 Linux Server, a host for G10 Clients.\n");
				printf("Usage: server [OPTION]\n\n");
				printf("Startup: \n");
				printf("\t -h, --help   prints this menu\n");
				printf("\t -l, --load   loads a server file from a JSON file\n");
                return 0;
            }

			// Load
            if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--load") == 0)
            {
				serverPath = (char*)(argv[i+1]) ? (char*)argv[i+1] : 0;
				
                continue;
            }
        }
    }

	printf("G10 Server\n");

	// Notify the user that either we are loading their file, or tell them how to load a file
	if(serverPath)
		gPrintLog("Loading server from %s\n",serverPath);
	else
		gPrintError("No server path was provided. You can load a server with \"load [PATH]\"\n");

	// Load the server, or don't
	if(serverPath)
		server = loadServer(serverPath);
	else
		return 0;
	// TODO: Spawn a thread that listens for connections. When a client connects, check their IP against the 
	//       blacklist or the whitelist. Depending on configurations, either spawn a new thread for them, or
	//       return a (custom) blacklist notificaiton. 

	// Create a new thread for the terminal to process commands
	pthread_create(&terminal,NULL, (void*)&processCommands, server);
	pthread_join(terminal,0);

	destroyServer(server);

	return 0;
}