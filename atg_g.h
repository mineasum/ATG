#ifndef __ATG_H__
#define __ATG_H__
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "cJSON.h"
	#include <time.h>
	#include "util.h"
	#include <pthread.h>
	
	//--------------------------------------------------
	//
	//
	#define CON_USED 1
	#define CON_UNUSED 0
	
	struct AtgGV{
		int 		serverid;
		char 		mode;
		int 		playdate;
		int 		playtime;
		time_t 	unixtime;
		char 		logfile[60];
		int 		debug;
		int			pthread_num;
		char		logpath[64];
		char		hostname[64];
		char		cfgfile[64];
		int	  	thread_num;
		static 	unsigned long serial;
		char  	address[16];
		int	  	port;
		int   	flog;
		
		char    r_host[16];
		int     r_port;
		
		int     timeout;
		time_t	tsec;
		
	};
	
	struct STBL_DATA{
		int id;
		int cfd;
		int con;
		pthread_t pid;
		int tfd;
		int tsec;
		
	};
	
	struct STBL{
		struct STBL_DATA* d;
		int activity;
		pthread_mutex_t s_mutex;	//
		pthread_mutex_t f_mutex;	  //log
	};
	
	typedef unsigned long long SERIALNUMBER;
	
	extern AtgGV gv;
	extern STBL stbl;
	
	void GInitial(AtgGV *gv);
	
  #define G_BID_LEN	16
  #define G_UID_LEN 16
  #define G_AC_LEN		16
  #define G_PWD_LEN 16
  #define G_MAC_LEN 16
  #define G_IP_LEN 	16
  #define G_OSVER_LEN 16
  #define G_IP_LEN 16
  #define G_ERRMSG_LEN 255
  #define G_NAME_LEN 16
  #define G_ACNAME_LEN 16
  #define G_SNAME_LEN 16
  #define G_STEL_LEN 16
  #define G_MARKET_LEN 1
  
  struct AtgLogin{
		int		FUN;
		int		LTYPE;
		char 	BID[G_BID_LEN+1];	//4
		char 	UID[G_UID_LEN+1]; //
		char 	AC[G_AC_LEN+1];
		char 	PWD[G_PWD_LEN+1];
		int  	CN;
		int  	TIME;
		int  	PID;
		int  	OS;
		int 	OSVER;
		char 	MAC[G_MAC_LEN];
		char 	IP[G_IP_LEN];
	};
	
	struct AtgLoginRes{
		int FUN;
		int SNO;
		int RET;
		char ERRMSG[G_ERRMSG_LEN+1];
		char UID[G_UID_LEN+1];
		char NAME[G_NAME_LEN+1];
		char BID[G_BID_LEN+1];
		char AC[G_AC_LEN+1];
		char ACNAME[G_ACNAME_LEN+1];
		int  ACSTATUS;
		int  AGREE01;
		int  AGREE02;
		int  ORDER;
		int  QUERY;
		char MARKET[G_MARKET_LEN+1];
		char SNAME[G_SNAME_LEN+1];
		char STEL[G_STEL_LEN+1];
	};
	
	struct AtgChangePWD{
		int		FUN;
		char	BID[G_BID_LEN+1];
		char	AC[G_AC_LEN+1];
		char	PWD[G_PWD_LEN+1];
		char	NEWPWD[G_PWD_LEN+1];
		int		TIME;
		int		PID;
		int		OS;
		char	OSVER[G_OSVER_LEN+1];
		int		MAC;
		char	IP[G_IP_LEN+1]	;
	};
	
	AtgLogin *ParseAtgLogin(const char *);
	char *getAtgLoginRes(AtgLoginRes *res);
	AtgChangePWD *ParseAtgChangePWD(const char *);
	int GetFUN(const char*);
	SERIALNUMBER GetSerialNumber();
	
	
	
#endif

