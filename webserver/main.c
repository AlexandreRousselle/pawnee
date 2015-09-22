#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "socket.h"
#include <unistd.h>


int main(void)
{
	fflush(stdout);
	int socket_serveur = creer_serveur(8080);
	int socket_client;

	char * data = malloc(42);

	while(1){
		socket_client = accept(socket_serveur, NULL, NULL);

		if (socket_client == -1){
			perror("accept");
		}
		
		sleep(1);

		const char * msg = "Bonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\n";
		write(socket_client, msg, strlen(msg));
	

		while(1){
			int i = read(socket_client, data, 42);
			if(i != -1)
				write(socket_client, data, i);
			else
				write(socket_client, data, 42);
		}

	}
	return 0;
}
