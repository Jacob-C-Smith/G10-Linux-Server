#include <G10/GXTerminal.h>

int process_commands ( GXServer_t* server )
{
	// Argument check
	{
		if(server == (void*)0)
			return 0;// TODO
	}

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

			if ( strlen(command) == 0)
				continue;

			// Uptime: prints out how many seconds the server has been alive for
			if     ( strcmp(command, "uptime")    == 0 || strcmp(command, "u") == 0 )
    		{
				// Uninitialized data
				printf("%s has been up for %ld seconds\n",server->name,time(NULL) - server->startTime);

	 		}

			// Prints the name of the active server
			else if ( strcmp(command, "name")     == 0 || strcmp(command, "n") == 0 )
			{
				printf("%s\n",server->name);
			}

			// Prints a list of clients, IP addresses, and play time
			else if ( strcmp(command, "clients")  == 0 || strcmp(command, "c") == 0 )
    		{
    		    GXClient_t* c = server->clients;

				printf("\nName            IP             Play time (s)  Commands recieved\n");

				while (c)
				{
					if(c->connected)
						printf("%-16s%-15s%ld - %ld\n", c->name, inet_ntoa(c->clientAddress->sin_addr), time(NULL) - c->joinTime, c->recieved_packets);
					c = c->next;
				}

				putchar('\n');
				putchar('\n');
				

    		}

			// Shuts the server down
			else if ( strcmp(command,"shutdown")  == 0 || strcmp(command, "q") == 0 )
			{
				goto cliExitShutdown;
			}

			// Kicks a player
			else if ( strncmp(command,"kick",4)   == 0 || strcmp(command, "k") == 0 )
			{
				strtok(command," ");
				char* clientName  = strtok(NULL," ");
				char* kickMessage = strtok(NULL,"");

				if(clientName == (void*)0)
					goto noKickName;

				printf("Kicking %s\n",clientName);
				kick_client(server,clientName, (kickMessage) ? kickMessage : "You have been kicked!");
				continue;

				// No name supplied to kick
				noKickName:
					printf("kick: missing client name\nUsage: kick [clientName] [message]\n");
					continue;
			}

			// Bans a player
			else if ( strncmp(command,"ban",3)    == 0 || strcmp(command, "b") == 0 )
				{
					// TODO: Ban a player
					char *playerName = command+3;

					// Trim leading whitespaces
					while(isspace((u8)*playerName)) playerName++;

					// If there is no player specified, we do nothing
					if (*playerName == 0)
						goto noPlayerSpecifiedForBan;

					// Otherwise, we ban the specified client
					//ban_client(playerName);
					
					return 0;

					noPlayerSpecifiedForBan:
						g_print_error("[G10] [Ban] No name specified for ban\n");
						return 0;
				}

			// Unbans a player
			else if ( strncmp(command,"unban",5)  == 0 || strcmp(command, "r") == 0 )
			{
				// TODO: Unban a player
			}

			// Broadcasts a message to all clients
			else if ( strcmp(command,"broadcast") == 0 || strcmp(command, "B") == 0 )
				{
					strtok(command, " ");

					char* broadcastMessage = strtok(NULL,"");
					size_t len = strlen(broadcastMessage);
					printf("\nBroadcasting \"%s\"\n",broadcastMessage);
	
					GXClient_t* c = server->clients;

					while (c)
					{
						if(c->connected)
							tcp_send(c,broadcastMessage,len);
						
						c = c->next;
					}
				}

			// Displays commands and contexts
			else if ( strcmp(command, "help")     == 0 || strcmp(command, "h") == 0 )
			{
				printf("The G10 Linux server terminal\n");


			}
			
			// Load a server
			else if ( strncmp(command,"load",4)   == 0 || strcmp(command, "q") == 0 )
			{
				// Initiailzed data
				char* path = 0;

				// Get the path
				strtok(command," ");
				path = strtok(NULL,"");

				// Load the server or not
				(path) ? load_server(path) : 0 ;
			}
			
			// List actors
			else if ( strncmp(command,"actors",6) == 0 || strcmp(command, "a") == 0  )
			{
				// TODO: 
			}

			// Print ticks
			else if ( strcmp(command, "tick") == 0)
			{
				printf("%d\n",server->ticks);
			}

			// Unknown command
			else
			{
				g_print_warning("[G10] [Terminal] Unknown command \"%s\"\n", command);

			}
		}

	}
	return 0;

	cliExitShutdown:
		server->running = false;
		return 0;
}