
#define __SOCKET_H__

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

int creer_serveur(int port);
void initialiser_signaux(void);
void traitement_signal(int sig);
char * fgets_or_exit ( char * buffer , int size , FILE * stream );

enum http_method {
	HTTP_GET ,
	HTTP_UNSUPPORTED ,
};
typedef struct
{
	enum http_method method ;
	int major_version ;
	int minor_version ;
	char * url ;
} http_request ;

int parse_http_request ( const char * request_line , http_request * request);
void skip_headers(FILE * fichier_client);
void send_status ( FILE * fichier_client , int code , const char * reason_phrase);
void send_response ( FILE * fichier_client , int code , const char * reason_phrase , const char * message_body);


