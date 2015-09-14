#include "socket.h"
#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


int creer_serveur(int port){
	int socket_serveur;	
	int socket_client;
	
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET ; /* Socket ipv4 */
	saddr.sin_port = htons(port); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

	char data[1024];

	socket_serveur = socket(AF_INET,SOCK_STREAM, 0);

	if (socket_serveur == -1){
		perror ("socket_serveur");
		return -1;
	}

	if (bind(socket_serveur,(struct sockaddr *)& saddr, sizeof(saddr)) == -1){
		perror("bind socker_serveur");
		return -1;
	}

	if (listen(socket_serveur, 10) == -1){
		perror("listen socket_serveur");
		return -1;
	}

	socket_client = accept(socket_serveur, NULL, NULL);

	if (socket_client == -1){
		perror("accept");
		return -1;
	}
	
	sleep(1);

	const char * msg = "Bonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\nBonjour,\nBienvenue sur mon serveur\n";
	write(socket_client, msg, strlen(msg));

	while(1){
		if (read(socket_client, data, sizeof(data)) == -1){
			perror("listen socket_serveur");
			return -1;
		}
		write(socket_client, data, strlen(data));
	}

	return socket_client;

}

/* Socket () 
Puis on fait le bind()
listen () 
Rien de bloquant dans ces trois la. 
Sauf si port déjà occupé. 

accept() = > Bloquant, on ne sort pas le temps que personne a demandé une connexion. 
Algorythme général d'un serveur 
Socket()
bind()
listen()

while(accept()){
	fork();
}

Algo géné d'un client : 
Socket()
Accept() */
