#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define THR_MAX 1000


typedef unsigned long long SERIALNUMBER;


struct GV{
	int count;
	pthread_mutex_t serial_mutex;
	SERIALNUMBER sn;
	int	g_lock;
};

struct GV gv;


void *MyThr(void*){
	struct timeval tv;
	int i;
	while( gv.g_lock ) { usleep(500); }
	for(i=0;i<3;i++){
		pthread_mutex_lock(&gv.serial_mutex);
		if( gettimeofday(&tv,NULL) == -1){
			perror("gettimeofdate() error");
		}
		gv.sn=tv.tv_sec*1000000+tv.tv_usec;
		printf("%ld\n",(SERIALNUMBER)gv.sn );
		pthread_mutex_unlock(&gv.serial_mutex);
		usleep(100);
	}
	pthread_exit(0);
}



int main(){
	int i;
	gv.g_lock=1;
	pthread_t t[THR_MAX];
	for(i=0;i<THR_MAX;i++){
		pthread_create( &t[i] , NULL , MyThr , NULL);
	}
	while(1){sleep(3);gv.g_lock=0;}
	return(0);
}

