#include "util.h"

int GDate(){
	time_t t;
	struct tm *tm;
	time(&t);
	tm=localtime(&t);
	return ( (tm->tm_year+1900) * 10000 + tm->tm_mon * 100 + tm->tm_mday );
}

int GTime(){
	time_t t;
	struct tm *tm;
	time(&t);
	tm=localtime(&t);
	return ( tm->tm_hour * 10000 + tm->tm_min * 100 + tm->tm_sec ) ;
}

int FLog(int fd,const char *cate,const char *format,...){
	char buff[8192];
	char head[30];
	va_list args;
	int count=0;
	
	if( fd <= 0 )  return 0;
	
	va_start(args,format);
	count=vsnprintf(buff,sizeof(buff)/sizeof(char),format,args);
	va_end(args);
	sprintf(head,"[%d %d] %s - " , GDate() , GTime() , cate );
	write( fd , head , strlen(head));
	write( fd , buff , strlen(buff));
	return count;
}


int Info(const char* format,...){
	char buff[8192];
	va_list args;
	int count=0;
	va_start(args,format);
	count=vsnprintf(buff,sizeof(buff)/sizeof(char),format,args);
	va_end(args);
	fprintf( stdout , "[%d %d] - %s" , GDate() , GTime() , buff );
	return count;
}

int Mesg(FILE *fh,const char* format,...){
	char buff[8192];
	va_list args;
	int count=0;
	va_start(args,format);
	count=vsnprintf(buff,sizeof(buff)/sizeof(char),format,args);
	va_end(args);
	fprintf( fh , buff ) ;
	return count;
}

//------------------------------------------------------------------
//Bug
int SafeWrite(int fd,char *message,int len){
	//ioctl
	//sync
	struct flock f;
	f.l_type=F_WRLCK;
	f.l_whence=SEEK_SET;
	f.l_start=0;
	f.l_len=0;
	f.l_pid=getpid();
	fcntl(fd,F_SETLKW,&f);
	write(fd,message,len);
	f.l_type=F_UNLCK; 
	fcntl(fd,F_SETLK,&f);
	return(len);
}

int SafeWriteLog(int fd,const char *cate,const char *format,...){
	char buff[8192];
	va_list args;
	int count=0;
	va_start(args,format);
	//Add timestamp size=18
	sprintf(buff,"[%d %d] ",GDate(),GTime());
	//Info( "Buff=%s\n", buff ) ;
	count=vsnprintf(buff+18,8192-18,format,args);
	va_end(args);
	SafeWrite( fd , buff , strlen(buff));
	return count;
}


int FileExists( const char *filename ){
	struct stat st;
	if( stat(filename,&st) == -1  ){ //|| errno == ENOENT
		return 0;	
	}	
	return 1;
}

int OpenLogFile( ) {
	const char fmt[] = "logs/atgd.%d.log";
	char buff[64];
	sprintf( buff , fmt , GDate() ) ;
	return open( buff , O_WRONLY | O_NONBLOCK | O_CREAT , 0700 ) ;
}

void RTrim( char *str ){
	char* original = str + strlen(str);
  do{
  	--original;
  }while(*original == '\r' || *original == '\n' || *original == ' ') ;
  
  *(original + 1) = '\0';
  return;
}

void LTrim( char *str ){
	char* original = str;
  char *p = original;
  int trimmed = 0;
  do
  {
  	if (*original != '\n' || trimmed)
    	{
      	trimmed = 1;
        *p++ = *original;
      }
  }
  while (*original++ != '\0');
  return;
}


char *GetFileContent( const char* filename ) {
	int 	fd=0;
	char	*content=NULL;
	if( fd = open ( filename , O_RDONLY , 0700 ) == -1 ) {
		return NULL;
	}
	content=(char*)calloc( sizeof(char) , 1024 * 1024 * 10  );
	while( read( fd , content , 1024*1024*10 ) ){
		break;
	}
	close(fd);
	return content;	
}



int GetCfgString( const char *str ,const char *thekey,char *p , int size){
	char key[32];
	char value[64];
	char count=0;
	memset( p , 0x00 , size );
	while( *str != '\n' && *str != '\r' && *str != '\0' ){
		if( *str == '=' ) {
			strncpy( key , str-count , ( (count+1) < 32 ) ? (count+1) : 32 );
			count=0;
		}
		count++;
		str++;
	}
	if( strcmp(key,thekey) == 0 && count > 0 ){
		strncpy( value , str - count , count );
	}
	return strlen(value);
}

int GetCfgInt(){}

int GetCfgFloat(){} 
