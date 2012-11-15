
#include "atg_g.h"

#define SafeGetStringItem(json,fld,ptr,len) if( cJSON_GetObjectItem(json,fld) != NULL ) {strncpy( ptr , cJSON_GetObjectItem(json,fld)->valuestring , len );} 
#define SafeGetIntItem(json,fld,ptr) if(  cJSON_GetObjectItem(json,fld) != NULL  ) { *ptr=cJSON_GetObjectItem(jsonroot,fld)->valueint; }	

#define MAX(a,b) ( a >b ) ? a : b
#define MIN(a,b) ( a >b ) ? b : a

//==================================================================
// Parse AtgLogin struct
// return:
// <error>:0
// <success>:struct AtgLogin *
//
//int		FUN;
//int		LTYPE;
//char 	BID[G_BID_LEN+1];	//4
//char 	UID[G_UID_LEN+1]; //
//char 	AC[G_AC_LEN+1];
//char 	PWD[G_PWD_LEN+1];
//int  	CN;
//int  	TIME;
//int  	PID;
//int  	OS;
//int 	OSVER;
//char 	MAC[G_MAC_LEN];
//char 	IP[G_IP_LEN];


AtgLogin *ParseAtgLogin(const char *json_str){
	if(!strlen(json_str))return NULL;
	AtgLogin *lg=(AtgLogin*)calloc(sizeof(AtgLogin),1);
	cJSON *jsonroot=cJSON_Parse(json_str);
	if( !jsonroot ) {
		puts( cJSON_GetErrorPtr() ) ;
		return NULL;
	}
	//Assgin vars
  SafeGetIntItem( jsonroot , "FUN" , &lg->FUN ) ;
  SafeGetStringItem( jsonroot , "BID" , lg->BID , G_BID_LEN );
  SafeGetStringItem( jsonroot , "UID" , lg->UID , G_UID_LEN );
  SafeGetStringItem( jsonroot , "AC" , lg->AC , G_AC_LEN );
  SafeGetStringItem( jsonroot , "PWD" , lg->PWD , G_PWD_LEN );
  SafeGetIntItem( jsonroot , "LTYPE" , &lg->LTYPE ) ;
  SafeGetIntItem( jsonroot , "CN" , &lg->CN ) ;
  SafeGetIntItem( jsonroot , "TIME" , &lg->TIME ) ;
  SafeGetStringItem( jsonroot , "MAC" , lg->MAC , G_MAC_LEN );
  SafeGetStringItem( jsonroot , "IP" , lg->IP , G_IP_LEN );
  cJSON_Delete(jsonroot);
  return lg;
}

//----------
// cJSON_AddNullToObject
// cJSON_AddTrueToObject
// cJSON_AddFalseToObject
// cJSON_AddNumberToObject
// cJSON_AddStringToObject


char *getAtgLoginRes(AtgLoginRes *res){
	cJSON *jsonroot,*fmt;
	char  *out;
	jsonroot=cJSON_CreateObject();	
	cJSON_AddNumberToObject(jsonroot, "FUN", res->FUN );
	cJSON_AddNumberToObject(jsonroot, "SNO", res->SNO );
	cJSON_AddNumberToObject(jsonroot, "RET", res->RET );
	cJSON_AddStringToObject(jsonroot, "ERRMSG", res->ERRMSG );
	cJSON_AddStringToObject(jsonroot, "UID", res->UID );
	cJSON_AddStringToObject(jsonroot, "NAME", res->NAME );
	cJSON_AddStringToObject(jsonroot, "BID", res->BID );
	cJSON_AddStringToObject(jsonroot, "AC", res->AC );
	cJSON_AddStringToObject(jsonroot, "ACNAME", res->ACNAME );
	cJSON_AddNumberToObject(jsonroot, "ACSTATUS", res->ACSTATUS );
	cJSON_AddNumberToObject(jsonroot, "AGREE01", res->AGREE01 );
	cJSON_AddNumberToObject(jsonroot, "AGREE02", res->AGREE02 );
	cJSON_AddNumberToObject(jsonroot, "ORDER", res->ORDER );
	cJSON_AddNumberToObject(jsonroot, "QUERY", res->QUERY );
	cJSON_AddStringToObject(jsonroot, "MARKET", res->MARKET );
	cJSON_AddStringToObject(jsonroot, "SNAME", res->SNAME );
	cJSON_AddStringToObject(jsonroot, "STEL", res->STEL );
	out=cJSON_PrintUnformatted(jsonroot);	
	cJSON_Delete(jsonroot);
	return out;
}

//==================================================
// ParseAtgChangePWD
//
//
//int		FUN;
//char	BID[G_BID_LEN+1];
//char	AC[G_AC_LEN+1];
//char	PWD[G_PWD_LEN+1];
//char	NEWPWD[G_PWD_LEN+1];
//int		TIME;
//int		PID;
//int		OS;
//char	OSVER[G_OSVER_LEN+1];
//int		MAC;
//char	IP[G_IP_LEN+1]	;

AtgChangePWD *ParseAtgChangePWD(const char *json_str){
	if(!strlen(json_str))return NULL;
	AtgChangePWD *cpwd=(AtgChangePWD*)calloc(sizeof(AtgChangePWD),1);
	cJSON *jsonroot=cJSON_Parse(json_str);
	if( !jsonroot ) {
		puts( cJSON_GetErrorPtr() ) ;
		return NULL;	
	}
	SafeGetIntItem( jsonroot , "FUN" , &cpwd->FUN ) ;
	SafeGetStringItem( jsonroot , "BID" , cpwd->BID , G_BID_LEN );
  SafeGetStringItem( jsonroot , "AC" , cpwd->AC , G_AC_LEN );
  SafeGetStringItem( jsonroot , "PWD" , cpwd->PWD , G_PWD_LEN );
  SafeGetStringItem( jsonroot , "NEWPWD" , cpwd->NEWPWD , G_PWD_LEN );
  SafeGetIntItem( jsonroot , "TIME" , &cpwd->TIME ) ;
  SafeGetIntItem( jsonroot , "PID" , &cpwd->PID ) ;
  SafeGetIntItem( jsonroot , "OS" , &cpwd->OS ) ;
  SafeGetStringItem( jsonroot , "OSVER" , cpwd->OSVER , G_OSVER_LEN );
  SafeGetIntItem( jsonroot , "MAC" , &cpwd->MAC ) ;
  SafeGetStringItem( jsonroot , "IP" , cpwd->IP , G_IP_LEN );
  cJSON_Delete(jsonroot);
  return cpwd;
}

int GetFUN(const char *json_str){
	int FUN;
	if(!strlen(json_str))return -1;
	AtgChangePWD *cpwd=(AtgChangePWD*)calloc(sizeof(AtgChangePWD),1);
	cJSON *jsonroot=cJSON_Parse(json_str);
	if( !jsonroot ) {
		//puts( cJSON_GetErrorPtr() ) ;
		return -1;
	}
	SafeGetIntItem( jsonroot , "FUN" , &FUN ) ;
	cJSON_Delete(jsonroot);
	return FUN;
}

void GInitial(AtgGV *gv){
	time_t t;
	time(&t);
	gv->unixtime=(int)t;
	gv->playdate=GDate();
	gv->playtime=GTime();
	gv->serverid=0;
	
}

void ToUpper( char *str){
	while( *str != 0x00 ){
		if( *str >= 0x61 && *str <= 0x7a ) *str -= 0x20;
		str++; 
	}
}

void ToLower(char *str){
	while( *str != 0x00 ){
		if( *str >= 0x41 && *str <= 0x5a ) *str += 0x20;
		str++; 
	}
}

SERIALNUMBER GetSerialNumber(){
	struct timeval tv;
	SERIALNUMBER ret;
	if( gettimeofday(&tv,NULL) == -1 ) return -1;
	ret = tv.tv_sec*1000000+tv.tv_usec;
	return ret;
}

	
