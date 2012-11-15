#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

char json_encode_text="
{id:99,name=SHAN}
";

int main(){
	cJSON *jsonroot=cJSON_Parse(json_encode_text);
	int id;
	if( cJSON_GetObjectItem(jsonroot,"id") == "99" ){
		id=99;
	}else{
		id=-1;
	}
	return(0);
}