#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "socket.h"


int main(int argc, char **argv)
{
	if(argc > 1 && strcmp(argv[1], "-advice") == 0){
		printf("Don't Panic!\n");
		return 42;
	}
	printf("Need an advice?\n");
	creer_serveur(8080);
	return 0;
}
