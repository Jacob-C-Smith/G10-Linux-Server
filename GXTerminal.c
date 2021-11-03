#include <G10/GXTerminal.h>

int processCommands ( GXServer_t* server )
{
	char* command = malloc(0xffff);
	printf("Started CLI\n");
	while (1)
	{
		// Get some input from the console
		{
			printf(" > ");
			fgets(command,0xfffe,stdin);

			// Erase the '\n'
			command[strlen(command)-1]='\0';
		}

		// Process the command
		{
			// Commands that require an active server
			if(server)
			{
				// Uptime: prints out how many seconds the server has been alive for
				if ( strncmp(command, "uptime",6) == 0)
    			{
					// Uninitialized data
					printf("%s has been up for %ld seconds\n",server->name,time(NULL) - server->startTime);

	 			}

				// Prints the name of the active server
				else if ( strcmp(command, "name") == 0)
				{
					printf("%s\n",server->name);
				}

				// Prints a list of clients, IP addresses, and play time
				else if ( strcmp(command, "clients") == 0)
    			{
    			    GXClient_t* c = server->clients;

					printf("\nName            IP             Play time (s) \n");

					while (c)
					{
						if(c->connected)
							printf("%-16s%-15s%ld\n", c->name, inet_ntoa(c->clientAddress->sin_addr), time(NULL) - c->joinTime);
						c = c->next;
					}

					putchar('\n');
					putchar('\n');
					

					continue;
    			}

				// Shuts the server down
				else if ( strcmp(command,"shutdown")==0)
				{
					goto cliExitShutdown;
				}

				// Kicks a player
				else if ( strcmp(command,"kick") == 0)
				{
					strtok(command," ");
					char* clientName  = strtok(NULL," ");
					char* kickMessage = strtok(NULL,"");

					if(clientName == (void*)0)
						goto noKickName;

					printf("Kicking %s\n",clientName);
					kickClient(server,clientName, (kickMessage) ? kickMessage : "You have been kicked!");
					continue;

					// No name supplied to kick
					noKickName:
						printf("kick: missing client name\nUsage: kick [clientName] [message]\n");
						continue;
				}

				// Bans a player
				else if ( strcmp(command,"ban") == 0)
				{
					// TODO: Ban a player
				}

				// Unbans a player
				else if ( strcmp(command,"unban")==0)
				{
					// TODO: Unban a player
				}

				// Broadcasts a message to all clients
				else if ( strcmp(command,"broadcast")==0)
				{
					strtok(command, " ");

					char* broadcastMessage = strtok(NULL,"");
					size_t len = strlen(broadcastMessage);
					printf("\nBroadcasting \"%s\"\n",broadcastMessage);
	
					GXClient_t* c = server->clients;

					while (c)
					{
						TCPSend(c,broadcastMessage,len);
						c = c->next;
					}
				}
			}

			if(strncmp(command,"load",4)==0)
			{
				// Initiailzed data
				char* path = 0;

				// Get the path
				strtok(command," ");
				path = strtok(NULL,"");

				// Load the server or not
				(path) ? loadServer(path) : printf("load: missing path\nUsage: --load [path]\n");
			}
		}

	}
	return 0;
	cliExitShutdown:
	return 0;
}