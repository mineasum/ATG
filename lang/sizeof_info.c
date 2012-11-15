#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MAX_LOOP_COUNT 0

int main(){
	time_t t;
	struct tm *tm;
	tm=localtime(&t);
	fprintf(stderr,"%04d-%02d-%02d\n" , tm->tm_year+1900 , tm->tm_mon , tm->tm_mday);
	puts( "----------------------------" );
	printf( "short sizeof=%d\n" , sizeof(short));
	printf( "int sizeof=%d\n" , sizeof(int));
	printf( "long sizeof=%d\n" , sizeof(long));
	puts( "----------------------------" );
	for(int i=0;i<MAX_LOOP_COUNT;i++){
		fprintf( stderr , "%d:\n" , i ) ;
	}
	return(0);
}
