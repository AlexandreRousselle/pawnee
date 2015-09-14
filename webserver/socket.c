#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "socket.h"

int creer_serveur(int port)
{
	int socket_serveur;
	int socket_client;
	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;

	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	socket_client = accept(socket_serveur, NULL, NULL);

	if(socket_serveur == -1)
	{
		perror("socket_serveur");
		return -1;
	}

	if(bind(socket_serveur,(struct sockaddr *)&saddr, sizeof(saddr)) == -1)
	{
		perror("bind socket_serveur");
		return -1;
	}

	if(socket_client == -1)
	{
		perror("accept");
		return -1;
	}

	write(socket_client, message_bienvenue, strlen(message_bienvenue);

	if(listen(socket_serveur, 10) == -1)
	{
		perror("liste socket_serveur");
		return -1;
	}

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
Accept()