
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


struct GV{
	int id;
	char message[30];
	pthread_t t;
};

void *Handler(void *gv){
	GV *pgv=(GV*)gv;
	printf( "id %d pid %d gv.t %d\n", pgv->id , getpid() ,pgv->t );
	fflush(stdout);
	while(1){
		sleep(1);	
	}
}

int main(){
	GV gv[100];
	for(int i=0;i<100;i++){
		gv[i].id=i;
		strcpy(gv[i].message,"T");
		pthread_create(&gv[i].t,NULL,Handler,(void*)&gv[i]);
		//pthread_join(gv[i].t,NULL);
	}
	while(1){
		sleep(3);	
	}
	puts("Master");
	return(0);
}
