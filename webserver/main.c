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
	int booleen = 0;
	char * data = malloc(2048);
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
			FILE *fichier_client = fdopen(socket_client,"w+");	
			write(socket_client, msg, strlen(msg));

			while (1){
				if(fgets(data, 2048, fichier_client) == NULL){
					break;
				}
				if(requetevalide(data)){
					booleen = 1;
					printf("%s\n",data );
				}
				int taille = strlen(data);
				while(!(data[0] == '\n' || (data[0] == '\r' && data[1] == '\n'))){
					if(fgets(data, 2048, fichier_client) == NULL){
						break;
					}
					taille = taille+strlen(data);
				}
				if(booleen){
					write(socket_client, msg, strlen(msg));
					fprintf(fichier_client, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %d\n\n200 OK\r\n", taille);
				}else{
					fprintf(fichier_client, "HTTP/1.1 400 BAD REQUEST\r\nConnection: close\r\nContent-Length: %d\n\n400 Bad request\r\n", taille);
				}
			}
			exit(0);
		}
	}
	return 0;
}
