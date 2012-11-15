#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
#include "util.h"
#include "atg_g.h"

#define BLOCKLOG 10
#define PTHREAD_MAX 5000
int serverfd;



struct AtgGV gv;
struct STBL stbl;


void CloseClient(STBL_DATA *s){
			const char *close_str="{\"FUN\":999}";
			if( s->con == CON_UNUSED ) return;
			
			FLog( gv.flog ,  "atg" , "Close id=%d\n" , s->cfd );
			Info ( "Close cfd=%d\n" , s->cfd );
			write( s->cfd , close_str , strlen( close_str ) );
			close(s->cfd);
			s->cfd = -1;
			s->con = CON_UNUSED;
			close(s->tfd);
}

int TCPConnect( const char *clientaddr , int port){
	int sockfd;
	struct sockaddr_in addr;
	if( ( sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1 ){
		perror( "socket() error.");
		return -1;
	}
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr = inet_addr(clientaddr);
	bzero( &(addr.sin_zero) , 8 );
	if( connect( sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) == -1 ){
		perror("connect()");
		return -1;
	}
	return sockfd;
}



void GetIPInfo(int s,char *ipstr,int *port){
	socklen_t len;
	struct sockaddr_storage addr;
	len = sizeof addr;
	getpeername(s, (struct sockaddr*)&addr, &len);
	
	// deal with both IPv4 and IPv6:
	if (addr.ss_family == AF_INET) {
	    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
	    *port = ntohs(s->sin_port);
	    inet_ntop(AF_INET, &s->sin_addr, ipstr, INET6_ADDRSTRLEN);
	} else { // AF_INET6
	    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
	    *port = ntohs(s->sin6_port);
	    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, INET6_ADDRSTRLEN);
	}
	return;
}

int SetupTCP( const char *ip , int port ){
	//Init
	struct sockaddr_in server;
	socklen_t addrlen;
	int serverfd;
	
	//Init socket
	if((serverfd=socket(AF_INET,SOCK_STREAM,0)) == -1 ){
		FLog( gv.flog ,  "atg" ,"socket() error");
		return(-1);
	}
	
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	if( strlen(ip) == 0 )
		server.sin_addr.s_addr=inet_addr(ip);
	else
		server.sin_addr.s_addr=INADDR_ANY;
	
	int socket_reuse_flag=1;
	int socket_buff_size = 16384;
	
	if( (setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&socket_reuse_flag,sizeof(socket_reuse_flag)) )== -1 ){
		perror( "setsockopt() error" );
		exit(0);
	}
	
	if( (setsockopt(serverfd,SOL_SOCKET,SO_SNDBUF,&socket_buff_size,sizeof(socket_buff_size)) )== -1 ){
		perror( "setsockopt() error" );
		exit(0);
	}
	
	if( 0 && (setsockopt(serverfd,SOL_SOCKET,SO_REUSEPORT,&socket_reuse_flag,sizeof(socket_reuse_flag)) )== -1 ){
		perror( "setsockopt() error" );
		exit(0);
	}
	
	FLog( gv.flog ,  "atg" ,"Initial\n");
	FLog( gv.flog ,  "atg" ,"Server Info:%s:%d\n",ip,port );
	bzero(&(server.sin_zero),8);
	if(bind(serverfd,(struct sockaddr *)&server,sizeof(struct sockaddr)) == -1 ){
		FLog( gv.flog ,  "atg" ,"bind() error\n");
		return(-1);
	}
	if(listen(serverfd,BLOCKLOG)==-1){
		FLog( gv.flog ,  "atg" ,"listen() error\n");
		return(-1);
	}
	return serverfd;
}



int GetFreeConn(){
	int i=0;
	for(i=0;i<gv.thread_num;i++){
		if( stbl.d[i].con == CON_UNUSED ) return i;
	}
	if( stbl.d[i-1].con == CON_UNUSED ){
		return i;
	}
	return -1;
}


void StartSevice(){
	struct sockaddr_in server,client;
	socklen_t addrlen;
	pid_t pid;
	
	if( ( serverfd=SetupTCP( gv.address , gv.port ) ) == 0 ){
		perror( "SetupTCP() error" );
		exit(0);	
	} 
	int clientfd;
	while(1){
		addrlen =sizeof(struct sockaddr_in);
		if((clientfd=accept(serverfd,(struct sockaddr *)&client,&addrlen))==-1){
			perror("accept() error\n");
			FLog( gv.flog ,  "atg" ,"accept() error\n");
			exit(0);
		}
		
		int pos=GetFreeConn();
		FLog( gv.flog ,  "atg" , "GetFreeConn=%d\n",pos);
		STBL_DATA *d=(STBL_DATA*)&stbl.d[pos];
		d->tsec=gv.tsec+gv.timeout;
		if( pos == -1 ){
			FLog( gv.flog ,  "atg" , "too many conection\n" );
			close(clientfd);
		}else{
			d->cfd=clientfd;
			d->con=CON_USED;
			FLog( gv.flog ,  "atg" ,"Connect id %d.\n", d->cfd );
			usleep(10);
		}
		//pthread_t pth;
	}
	close(serverfd);
}


void StopSevice(int sig){
	FLog( gv.flog ,  "atg" ,"Close ArgSrv\n");
	Info( "Close service\n" );
	
	for(int i=0;i<gv.thread_num;i++){
		STBL_DATA *p=(STBL_DATA*)&stbl.d[i];
		CloseClient( p ) ;
	}
	
	if( shutdown(serverfd,SHUT_RDWR) == -1 ){
		perror("shotdown() error");
	}
	
	signal(SIGINT,SIG_DFL); 			//SIG_DFL
	usleep(1000);
	exit(0);
}

void *DoDeal(void *arg){
	int pos=*((int*)arg);
	STBL_DATA *d=(STBL_DATA*)&stbl.d[pos];
	d->pid=pthread_self();
	char ipstr[INET6_ADDRSTRLEN];
	int port;
	
	while( 1 ){
		while( d->con == CON_UNUSED ){
			//FLog( gv.flog ,  "atg" ,"pos=%d con=%d cfd=%d\n",pos,d->con,d->cfd);
			usleep(100);
		}
		
		GetIPInfo(d->cfd,ipstr,&port);
		Info( "Incoming %s:%d\n" , ipstr , port );
		//-------------------------------------------------
    pthread_mutex_lock(&stbl.s_mutex);
	  SERIALNUMBER sn=GetSerialNumber();
		pthread_mutex_unlock(&stbl.s_mutex);
		char wrbuff[256];
		sprintf( wrbuff ,"%ld",sn);
		//Info( d->cfd , wrbuff , strlen(wrbuff) );
		char buff[1024];
		//-------------------------------------------------
		int ret=0;
		memset( buff , 0x00 , sizeof( buff ) );
		ret=read( d->cfd , buff , 1024 ) ;
		//-------------------------------------------------
		// Socket or MMAP ?
		//Link to ice trace
		if( d->tfd <= 0 ){
						for( int i=0;i<1;i++){
							usleep(100);
							d->tfd= TCPConnect(gv.r_host,gv.r_port);
						}
						Info ( "Connect %s:%d tfd=%d\n" , gv.r_host,gv.r_port , d->tfd );
		}
		
		//-------------------------------------------------
		
		if( ret < 1 ) {
			CloseClient(d);
		}else{
			int fun;
			RTrim( buff ) ;
		  fun=GetFUN(buff);
		  Info( "Recv:>%s<\n" , buff ) ;
		  Info( "Recv fun:%d\n" , fun ) ;
			
			if( d->tfd <= 0 ){
						for( int i=0;i<1;i++){
							usleep(100);
							d->tfd= TCPConnect(gv.r_host,gv.r_port);
						}
						Info ( "Connect %s:%d tfd=%d\n" , gv.r_host,gv.r_port , d->tfd );
		  }
			
			if( fun > 0 ){
				if( write( d->tfd , buff , strlen(buff) ) < 0 ){
					perror( "wirte() error" );
				}else{

				}
				Info( "Send:%s\n" , buff ) ;
				/*
				if( 0 ) {
						
				}else if( fun == 1 ){
								AtgLogin *lg=ParseAtgLogin(buff);
								Info( "Check LTYPE=%d AC=%s PWD=%s\n" , lg->LTYPE , lg->AC , lg->PWD ) ;
								free( lg ) ;
				}
				*/
			}
		}
	}
}

void *SetTimer(void *) {
	while( 1 ) {
		time(&gv.tsec);
		//Activity
		stbl.activity=0;
		for( int i=0 ; i < gv.thread_num ; i ++ ) {
			STBL_DATA *d=(STBL_DATA*)&stbl.d[i];
			if( d->con == CON_USED ) stbl.activity++;
			if( d->con == CON_USED & gv.timeout > 0 ){
				//Info( "gv.tsec=%d d->tsec=%d\n" , gv.tsec , d->tsec ) ;
				if( gv.tsec >  d->tsec ){
					Info( "Timeout fd=%d\n" , d->cfd );
					CloseClient(d) ;
				}
			}
		}
		//Info( "Activity=%d\n" , stbl.activity );
	  usleep( 1000000 );
	}
}


int BuildThread(){
	FLog( gv.flog ,  "atg" , "Create pthread number %d\n" , gv.thread_num ) ;
	int ret=0;int i;
	//Wait for all pthread?!
	for( i =0;i<gv.thread_num;i++ ) {
		stbl.d[i].id=i;
		pthread_create( &stbl.d[i].pid , NULL , DoDeal , (void *)&stbl.d[i].id );
		//FLog( gv.flog ,  "atg" , "i=%d\n" , stbl.d[i].id );
		ret++;
		//usleep(10);
	}
	return ret;
}


int SetupEnv(){
	char buff[128];
	memset(&gv,0x00,sizeof(AtgGV));
	
	//Get current directory
	getcwd(gv.logpath,32);
	Info(  "log path=%s\n" ,  gv.logpath ) ;
	gv.thread_num = 20;
	stbl.d =(STBL_DATA*) calloc( sizeof(STBL_DATA)  ,gv.thread_num );
	for(int i =0;i<gv.thread_num;i++){
		stbl.d[i].con=CON_UNUSED;
	}
	
	time( &gv.unixtime );
	gethostname( gv.hostname , sizeof( gv.hostname ) ) ;
	
	//Default
	strcpy( gv.address , "localhost");
	gv.port=5555;
	
	gv.playdate=GDate();
	gv.playtime=GTime();
	strcpy( gv.cfgfile , "cfg/srv.cfg" );
	gv.debug=1;
	
	sprintf( buff , "%s/atg_%d.log" , gv.logpath , GDate() );
	
	gv.flog = open ( buff , O_WRONLY | O_APPEND | O_CREAT , 0700 );
	
	if( gv.flog == -1){
		perror( "open() error" );exit(0);
	}
	
	Info(  "open %s\n" , buff );

	gv.timeout = 1000;
}


void GetOpts(int argc,char **argv){
	int ch;
	while( ( ch=getopt(argc,argv,"p:f:n:s:S:P:d"))!= -1 ){
		switch( ch ){
			case 'd':	gv.debug=1;break;	
			case 'f': strcpy( gv.cfgfile , optarg );break;
			case 'n':	gv.thread_num=atoi(optarg);break;
			case 'S':	strcpy( gv.address , optarg );break;
			case 'P':	gv.port=atoi( optarg );break;
			case 's': strcpy( gv.r_host , optarg );break;
			case 'p': gv.r_port=atoi( optarg );break;
			default:  break;
		}
	}
	return;
}


int main( int argc ,char ** argv ){
	pthread_t timerfd;
	int thr_num;
	
	SetupEnv();
	GetOpts(argc,argv);
	Info(  "gv.debug=%d\n" , gv.debug ) ;
	Info(  "gv.cfgfile=%s\n" , gv.cfgfile ) ;
	Info(  "gv.playdate=%d\n" , gv.playdate ) ;
	Info(  "gv.playtime=%d\n" , gv.playtime ) ;
	Info(  "gv.thread_num=%d\n" , gv.thread_num ) ;
	Info(  "gv.address=%s\n" , gv.address );
	Info(  "gv.port=%d\n" , gv.port ) ;
	Info(  "gv.timeout=%d\n" , gv.timeout ) ;
	//BuildPthread();
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,StopSevice);
	
	pthread_mutex_init( &stbl.s_mutex , NULL );
	pthread_mutex_init( &stbl.f_mutex , NULL );
	
	thr_num=BuildThread();
	Info( "Thr num=%d%d\n" , thr_num );
	
	Info( "Init timer thr\n" );
	pthread_create( &timerfd , NULL , SetTimer , NULL );
	
	Info( "Start sevice\n");
	StartSevice();
	
	return(0);
}


