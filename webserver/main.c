#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "socket.h"
#include <unistd.h>



int main(void)
{
	initialiser_signaux();
	int socket_serveur = creer_serveur(8080);
	int socket_client;
	char * data = malloc(42);
	const char * msg = "Bonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\n";

	while (1){
		socket_client = accept(socket_serveur, NULL, NULL);

		int pid = fork();

 		if (pid == 0){

			if (socket_client == -1)
				perror("accept");	
		
			sleep(1);

			write(socket_client, msg, strlen(msg));
	
			while (1){
				int i = read(socket_client, data, 42);
				if (i > 0){
					write(socket_client, data, i);
				} else {
				  exit(0);/*break;*/
				}
			}
			/*exit(0);*/
		}
	}
	return 0;
}
