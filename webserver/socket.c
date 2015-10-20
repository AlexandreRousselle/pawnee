#include "socket.h"
#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>




int creer_serveur(int port){
	int socket_serveur;
	
	socket_serveur = socket(AF_INET,SOCK_STREAM, 0);

	if (socket_serveur == -1)
		perror ("socket_serveur");

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET ; /* Socket ipv4 */
	saddr.sin_port = htons(port); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

	int optval = 1;
	if (setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		perror ( " Can not set SO_REUSEADDR option " );	

	if (bind(socket_serveur,(struct sockaddr *)& saddr, sizeof(saddr)) == -1)
		perror("bind socker_serveur");

	if (listen(socket_serveur, 10) == -1)
		perror("listen socket_serveur");

	return socket_serveur;

}

void traitement_signal(int sig){
	printf("Signal : %d", sig);
	if(sig == SIGCHLD){
		waitpid(-1,0,WNOHANG);
	}
}

void initialiser_signaux(void){

	struct sigaction sa;

	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		perror("sigaction(SIGCHLD)");

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		perror("signal");
}

int requetevalide(char data[]){
	int i=0;
	int nbmot=1;
	int j=0;
	while(data[i]!='\0' && i!=8000){
		if(data[i]==' '){
			if(data[i-1]!=' '){
				nbmot++;
				j=i+1;
			}
		}
		i++;
	}
	if(nbmot!=3){
		return 0;
	}
	if(!(data[0]=='G' && data[1]=='E' && data[2]=='T')){
		return 0;
	}
	if(!(data[j]=='H' && data[j+1]=='T' && data[j+2]=='T' && data[j+3]=='P' && data[j+4]=='/')){
		return 0;
	}if(!(data[j+5]>='0' && data[j+5]<='1')){
		return 0;
	}if(!(data[j+6]=='.')){
		return 0;
	}if(!(data[j+7]>='0' && data[j+7]<='9')){
		return 0;
	}
	return 1;
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
