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


int main(void){
	initialiser_signaux();
	int socket_serveur =creer_serveur(8080);
	int socket_client ;
	int resultat=0;
	const char * msg = "Bonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\n";
	while(1){
		socket_client = accept ( socket_serveur , NULL , NULL );
		if (socket_client == -1)
			{
				perror ( "accept" );
				/* traitement d ’ erreur */
			}else{
				printf("Bienvenue \n");
			}		
		int pid=fork();
		if(pid!=0){
			close(socket_client);
		}else{
			FILE *fichier_client= fdopen(socket_client,"w+");			/*FILE *fichier_serveur= fdopen(socket_serveur,"w+");			*/
			char data[2048];
			if(fgets(data,2048,fichier_client)==NULL){
				break;
			}
			printf(data);
			resultat=requetevalide(data);
			int taille = strlen(data);
			while(!(data[0]=='\n' || (data[0]=='\r' && data[1]=='\n'))){
				if(fgets(data,2048,fichier_client)==NULL){
					break;
				}
				printf(data);
				taille=taille+strlen(data);
			}
			if(resultat==200){
				write(socket_client, msg, strlen(msg));
				fprintf(fichier_client, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %d\n\n200 OK\r\n",taille );
				
			}else if(resultat==404){
				fprintf(fichier_client, "HTTP/1.1 404 Not found\r\nConnection: close\r\nContent-Length:%d\n\n404 Not found\r\n",taille  );
			}else{
				fprintf(fichier_client, "HTTP/1.1 400 BAD REQUEST\r\nConnection: close\r\nContent-Length:%d\n\n400 Bad request\r\n",taille  );
			}
			exit(0);
		}
	}
	return 0;
}
