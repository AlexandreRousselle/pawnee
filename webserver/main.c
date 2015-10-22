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
#include <sys/wait.h>

#include "socket.h"


int main(void){
	initialiser_signaux();
	int socket_serveur =creer_serveur(8080);
	int socket_client ;
	int resultat=0;
	http_request my_http_request;
	const char * msg = "Welcome to my awesome web socket server\n";
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
			fgets_or_exit(data, 2048, fichier_client);
			printf(data);
			resultat=parse_http_request(data, &my_http_request);
			skip_headers(fichier_client);
			if(!resultat){
				send_response(fichier_client,400,"Bad Request","Bad request\r\n");
			}
			else if(my_http_request.method==HTTP_UNSUPPORTED){
				send_response(fichier_client , 405 , "Method Not Allowed" , "Method Not Allowed\r\n" );
			}
			else if(strcmp(my_http_request.url,"/")==0){
				send_response ( fichier_client , 200 , "OK" , msg );
			}
			else{
				send_response ( fichier_client , 404 , "Not Found" , "Not Found\r\n" );
			}
			exit(0);
		}
	}
	return 0;
}
