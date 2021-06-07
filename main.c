#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <G10/GXClient.h>

int main(int argc, char *argv[]) {

	// Uninitialized data
	int                listen_sock,
	                   sock;
	struct sockaddr_in *server_address,
	                   *client_address;

	// Initialized data
	int                SERVER_PORT        = 8877,
	                   client_address_len = 0,
	                   wait_size          = 16,
					   retries            = 0;
		
	server_address                  = malloc(sizeof(struct sockaddr_in));
	server_address->sin_family      = AF_INET;
	server_address->sin_port        = htons(SERVER_PORT);
	server_address->sin_addr.s_addr = htonl(INADDR_ANY);
	
	printf("G10 Server\n");

	// Initialize the server
	{
		// Create a socket
		if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
			goto noSocket;

		retryBind:
		// Bind the socket
		if ((bind(listen_sock,(const struct sockaddr *) server_address, sizeof(struct sockaddr_in))) < 0)
			goto noBind;

		// Start listening on the socket
		if (listen(listen_sock, wait_size) < 0)
			goto noListening;
	}

	// 
	while (true) {
		// Initialized data
		int n       = 0,
		    len     = 0,
		    maxlen  = 0xffff;
		char buffer[maxlen],
		     *pbuffer = buffer;

		// Try to accept a TCP connection
		if ((sock = accept(listen_sock, (struct sockaddr *)client_address, &client_address_len)) < 0) {
			printf("could not open a socket to accept data\n");
			return 1;
		}

		// Log the IP Address that connects
		printf("Client connected with IP address: %s\n", inet_ntoa(client_address->sin_addr));

		while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
			
			if(strncmp("GET / HTTP/1.1", buffer,strlen("GET / HTTP/1.1"))==0)
			{
				printf("HTTP Request made\n");
				send(sock, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nConnection: Keep-Alive\nContent-Type: text/html; charset=utf-8\nKeep-Alive: timeout=500,max=999\nServer: G10\nContent-Length:72 \n\n<!DOCTYPE HTML5><html><body><p>Hello from Jakes server</p></body></html>\0", strlen("HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nConnection: Keep-Alive\nContent-Type: text/html; charset=utf-8\nKeep-Alive: timeout=500,max=999\nServer: G10\nContent-Length:72 \n\n<!DOCTYPE HTML5><html><body><p>Hello from Jakes server</p></body></html>\0"),0);			
			}

			for (size_t i = 0; i < n; i++)
			{
				int a = (int) buffer[i];
				printf("%X ", a & 0xFF);
				if(n%16==0)
					putchar('\n');
			}
			putchar('\n');
			
			//printf("received: \"%s\"\n", buffer);
			
			// printf("Quit?\n");
			int a = getchar();
			if ((char)a == 'q')
				goto end;
		}
		continue;
		end:
		goto closeSockets;
	}
	end2:
	close(listen_sock);
	return 0;
    
    // Error debugging
    noSocket:
    	#ifndef NDEBUG
    		printf("could not create listen socket\n");
    	#endif
    	goto closeSockets;
    noBind:
    	#ifndef NDEBUG
    		printf("could not bind socket: Try #%d\r",++retries);
    	#endif
		shutdown(sock, SHUT_RDWR);
		shutdown(listen_sock, SHUT_RDWR);
		goto retryBind;
    noListening:
    	#ifndef NDEBUG
    		printf("Could not open socket for listening\n");
    	#endif
		goto closeSockets;
	closeSockets:
		shutdown(sock, SHUT_RDWR);
		shutdown(listen_sock, SHUT_RDWR);
		close(sock);
		close(listen_sock);
    	return 0;
}