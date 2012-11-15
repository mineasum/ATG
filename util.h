
#ifndef __UTIL_H__
#define __UTIL_H__
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <stdarg.h>
	#include <string.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/stat.h>
	

	int GDate();
	int GTime();
	int Info(const char *format,...);
	int Mesg(FILE *fh,const char* format,...);
	int FLog(int fd,const char *cate,const char *format,...);
	int SafeWrite(int fd,char *message,int len);
	int SafeWriteLog(int fd,const char *cate,const char *format,...);
	int FileExists( const char *filename );
	int OpenLogFile( );
	void RTrim( char *str );
	void LTrim( char *str ) ;
	char *GetFileContent(const char *filename ) ;
#endif
