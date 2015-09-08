#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

int creer_serveur(int port)
{
	int socket_serveur;
	int socket_client;

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;

	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_serveur == -1)
	{
		perror("socket_serveur");
	}

	bind(socket_serveur, *saddr, socklen_t addrlen);
}

