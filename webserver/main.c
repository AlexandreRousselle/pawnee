#include "socket.h"


int main(int argc, char *argv[]){
	int fd = 0;
	if(argc != 2){
		perror("Dossier non dispo");
		return 0;
	}

	const char * root=argv[1];
	if(!check_path(root)){
		return 0;
	}
	initialiser_signaux();
	int socket_serveur =creer_serveur(8080);
	int socket_client ;
	int resultat=0;
	http_request my_http_request;
	while(1){
		socket_client = accept ( socket_serveur , NULL , NULL );
		if (socket_client == -1)
			{
				perror ( "accept" );
				
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
			else if((fd=check_and_open(my_http_request.url, root)) != -1){
				send_response_fd (fichier_client , 200 , "OK" , fd, getmime(my_http_request.url));
			}
			else{
				send_response ( fichier_client , 404 , "Not Found" , "Not Found\r\n" );
			}
			exit(0);
		}
	}
	return 0;
}
