#include "socket.h"

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

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
	request->url=rewrite_url(url);

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

void send_response_fd(FILE * fichier_client, int code, const char * reason_phrase, int fd, char mime[]){
	send_status(fichier_client, code, reason_phrase);
	fprintf(fichier_client, "Connection: close\r\nContent-Length: %d\n\rContent-type:%s\n\r\n",get_file_size(fd),mime);
	fflush(fichier_client);
}

int check_path(const char * url){
	if(access(url, F_OK) == -1){
		perror("Unexistant file");
		return 0;
	}
	if(access(url, X_OK) == -1){
		perror("Unreachable file");
		return 0;
	}
	struct stat statdata;
	stat(url, &statdata);
	if(S_ISDIR(statdata.st_mode)){
		perror("this isnt a directory");
		return 0;
	}
	return 1;
}

int check_and_open(const char * url, const char * document_root){
	char * path = malloc(strlen(url)+strlen(document_root)+1);
	strcpy(path, document_root);
	strcat(path, url);

	struct stat statdata; 
	stat(path, &statdata);

	if (!S_ISREG(statdata.st_mode)){
		perror("this isnt a regular file");
		return -1;
	}
	int fd_path = open(path,O_RDONLY);
	return fd_path;
}

char * rewrite_url (char * url){
	int unsigned i = 0;
	if(strcmp(url,"/")==0){
		return "/index.html";
	}
	char * res = malloc(strlen(url));
	while(url[1] != '\0'){
		if (url[i] == '?'){
			res[i]='\0';
			break;
		}
		res[i] = url[i];
		i++;
	}
	return res;
}

int check_url(char * url){
	
	int unsigned i = 0;
	int j = 0;
	for(i = 1; i<(strlen(url)); i++){
		if(url[i]=='/' && url[i-1]!='.' && url[i-1]!='/'){
			j++;
		}if(url[i]=='.' && url[i-1] =='.' ){
			j--;
		}
		if(j<0){
			return 1;
		}
	}
	return 0;
}

int copy(int in, int out){
	
	char * buff = malloc(get_file_size(in));
	if(read(in,buff,get_file_size(in))==-1){
		perror("Error in file reading");
		return 0;
	}
	if(write(out,buff,get_file_size(in))==-1){
		perror("Error in socket's writing");
		return 0;
	}
	
	
	return 1;
}

int get_file_size(int fd){
	struct stat statbuf;
	fstat(fd,&statbuf);
	return statbuf.st_size;
}
char * getmime(char  nom[]){
	int k=strlen(nom);
	int i=0;
	
	while(i<k && nom[i]!='.'){
		i++;
	}
	if(nom[i]=='.'){
		i++;
	}
	int j=0;
	char ext[strlen(nom)-i];
	while(i+j<=k  ){
		ext[j]=nom[i+j];
		j++;
	}
	printf("%s-%d-%d",nom,i,k);
	printf("%s",ext);
	fflush(stdout);
	if(strcmp(ext,"jpg")==0){
		return "image/jpeg";
	}
	if(strcmp(ext,"jpeg")==0){
		return "image/jpeg";
	}
	if(strcmp(ext,"html")==0){
		return "text/html";
	}
	if(strcmp(ext,"htm")==0){
		return "text/html";
	}
	return "text/plain";
}
