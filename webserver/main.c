#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#include "socket.h"


int main(void)
{
	initialiser_signaux();
	int socket_serveur = creer_serveur(8080);
	int socket_client;
	char * data = malloc(42);
	const char * msg = "Bonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\n";

	while (1){
		socket_client = accept(socket_serveur, NULL, NULL);

		if (socket_client == -1){
			perror("accept");	/* Gestion d'erreur sur accept */
		}
		sleep(1);

		int pid = fork();

		if (pid != 0){
			close(socket_client);

		}else{
			FILE *fichier = fdopen(socket_client,"w+");	
			write(socket_client, msg, strlen(msg));

			while (1){
				if(fgets(data, 42, fichier) == NULL){
					break;
				}
				fprintf(fichier,"<pawnee>");
				fprintf(fichier,data);
			}
			exit(0);
		}
	}
	return 0;
}
