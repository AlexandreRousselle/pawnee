#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "socket.h"



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
/*
int requetevalide(char data[]){
	int i=0;
	int nbrmot=1;
	int troisieme_mot=0;
	int deuxieme_mot=0;
	while(data[i]!='\0' && i!=2048){
		if(data[i]==' '){
			if(data[i-1]!=' '){
				nbrmot++;
				if(deuxieme_mot==0){
					deuxieme_mot=i+1;
				}
				troisieme_mot=i+1;
			}
		}
		i++;
	}
	if(nbrmot!=3){
		return 400;
	}
	if(!(data[0]=='G' && data[1]=='E' && data[2]=='T')){
		return 400;
	}
	if(!(data[troisieme_mot]=='H' && data[troisieme_mot+1]=='T' && data[troisieme_mot+2]=='T' && data[troisieme_mot+3]=='P' && data[troisieme_mot+4]=='/')){
		return 400;
	}if(!(data[troisieme_mot+5]>='0' && data[troisieme_mot+5]<='1')){
		return 400;
	}if(!(data[troisieme_mot+6]=='.')){
		return 400;
	}if(!(data[troisieme_mot+7]>='0' && data[troisieme_mot+7]<='9')){
		return 400;
	}if(!(data[deuxieme_mot]=='/' && data[deuxieme_mot+1]==' ')){
		return 404;
	}
	return 200;
}*/

	char * fgets_or_exit ( char * buffer , int size , FILE * stream ){
		if(fgets(buffer,size,stream)==NULL){
			printf("%s",buffer);
			exit(1);
		}
		return buffer;
	}
	int parse_http_request ( const char * request_line , http_request * request ){
		int i=0;
		int nbmot=1;
		int troisieme_mot=0;
		int deuxieme_mot=0;
		while(request_line[i]!='\0' && i!=2048){
			if(request_line[i]==' '){
				nbmot++;
				if(deuxieme_mot==0){
					deuxieme_mot=i+1;
				}
				troisieme_mot=i+1;

			}
			i++;
		}

		if(nbmot!=3){
			return 0;
		}
		char * url = malloc(troisieme_mot-deuxieme_mot-1);
		char * buf =url;
		while(deuxieme_mot<troisieme_mot-1){
			*buf=request_line[deuxieme_mot];
			buf++;
			deuxieme_mot++;
		}
		request->url=url;

		if(!(request_line[0]=='G' && request_line[1]=='E' && request_line[2]=='T')){
			return 0;
		}else{
			request->method=HTTP_GET;
		}
		if(!(request_line[troisieme_mot]=='H' && request_line[troisieme_mot+1]=='T' && request_line[troisieme_mot+2]=='T' && request_line[troisieme_mot+3]=='P' && request_line[troisieme_mot+4]=='/')){
			return 0;
		}if(!(request_line[troisieme_mot+5]=='1')){
			return 0;
		}if(!(request_line[troisieme_mot+6]=='.')){
			return 0;
		}if(!(request_line[troisieme_mot+7]>='0' && request_line[troisieme_mot+7]<='1')){
			return 0;
		}

		return 1;
	}
	void skip_headers(FILE * fichier_client){
		char data[2048];
		fgets_or_exit(data,2048,fichier_client);
		while(!(data[0]=='\n' || (data[0]=='\r' && data[1]=='\n'))){
			fgets_or_exit(data,2048,fichier_client);
			printf(data);
		}
	}


	void send_status ( FILE * fichier_client , int code , const char * reason_phrase ){
		fprintf(fichier_client, "HTTP/1.1 %d %s\r\n",code,reason_phrase);
	}
	void send_response ( FILE * fichier_client , int code , const char * reason_phrase , const char * message_body ){
		send_status(fichier_client,code,reason_phrase);
		fprintf(fichier_client,"Connection: close\r\nContent-Length: %d\n\r\n",(int) strlen(message_body));
		fprintf(fichier_client,"%s",message_body);
	}