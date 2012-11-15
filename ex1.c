#include <stdio.h>
#include "atg_g.h"
#include "util.h"
#include <fcntl.h>

char json_str[]="{\"FUN\":1,\"BID\":\"World!\",\"LTYPE\":9999,\"TIME\":20121212}";

void ExampleWriteJSON(){
	cJSON *jsonroot,*fmt;
	char  *out;
	jsonroot=cJSON_CreateObject();	
	cJSON_AddItemToObject(jsonroot, "name", cJSON_CreateString("Jack (Bee) Nimble"));
	cJSON_AddItemToObject(jsonroot, "format", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"type",		"rect");
	cJSON_AddNumberToObject(fmt,"width",		1920);
	cJSON_AddNumberToObject(fmt,"height",		1080);
	cJSON_AddFalseToObject (fmt,"interlace");
	cJSON_AddNumberToObject(fmt,"frame rate",	24);
	out=cJSON_PrintUnformatted(jsonroot);	
	Info("%s\n",out);	
	free(out);	
	cJSON_Delete(jsonroot);
}

void ExampleWriteJSONArray(){
	cJSON *jsonroot;
	char *out;
	const char *strings[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	jsonroot=cJSON_CreateStringArray(strings,7);
	out=cJSON_PrintUnformatted(jsonroot);	
	cJSON_Delete(jsonroot);	
	Info("%s\n",out);	
	free(out);
}

void ExampleWriteAtgLoginRes(){
	AtgLoginRes res;
	memset(&res,0x00,sizeof(AtgLoginRes));
	res.FUN=2;
	res.ACSTATUS=1;
	strcpy( res.BID , "ExampleWriteAtgLoginRes" );
	strcpy( res.NAME , "Apex" );
	char *output=getAtgLoginRes(&res);
	Info( "%s\n" , output ) ;
	free(output);
	return;
}

int main(){
	int count=10000;
	
	AtgGV gv;
	GInitial(&gv);

	strcpy( gv.logfile , "~/atg.log" );
	Info( "log file=%s\n" , gv.logfile );
	Info( "Unixtime %d\n" , gv.unixtime ) ;
	
	ExampleWriteJSON();
	ExampleWriteJSONArray();
	ExampleWriteAtgLoginRes();
	//------------------------------------------
	//
	//
	int fd;
	fd=open("/tmp/atg.log",O_APPEND|O_WRONLY|O_CREAT,0700);
	if( fd == -1 ){
		perror("open() error");	
	}else{
		for(int i=0;i<10;i++){
			time_t t;
			time(&t);
			SafeWriteLog(fd,"Test","%d\n",t);
		}
	}
	
	char message[]="Check it.";
	SafeWriteLog(fd,"Test","Currdate = %d\n" , GDate() );
	
	//
	int FUN=GetFUN( json_str ) ;
	SafeWriteLog( fd , "Get_FUN" , "FUN=%d\n" , FUN ) ;
	
	//
	close(fd);
	
	Info( "Call ParseAtgLogin count=%d\n" , count ) ;
	for(int i=0;i<count;i++){
		AtgLogin *lg=ParseAtgLogin(json_str);
		if(lg==NULL){
			puts("ParseAtgLogin() error");
			return(1);
		}
		free(lg);
	}
	//----------
	//Bug
	//char *content=GetFileContent("/etc/passwd");
	return(0);
}
