#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "util.h"


#define SRV_PORT 3333
#define SRV_ADDR 127.0.0.1
#define BLOCKLOG 100


void Client(){
	 int sockfd, num;
	 struct hostent *he; 
	 struct sockaddr_in server;
	 fd_set rfds; //
	 int maxfd = -1;
	 if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("socket() error");
		exit(1);
	 }
}

int main( int argc ,char ** argv ){
	Info("Program start\n");
	Info("Parend pid=%d\n",getpid());
	Client();
	return(0);
}


