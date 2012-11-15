#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

#define MyGetStr( tptr , sptr , len ) { strncpy( tptr , sptr , len ) ; tptr[len] = '\0'; }
#define MyGetInt( tptr , sptr ) { tptr = atoi( sptr ); }
#define MyGetFloat( tptr , sptr ) { tptr = atof( sptr ); }
#define MyGetChar(tptr,sptr) { *tptr = *sptr }


#define S_HOST_LEN 			60
#define S_USER_LEN 			16
#define S_PASSWORD_LEN 	41

struct MY{
	MYSQL_RES	*res;
	MYSQL_ROW	*row;
	int        size;
};

MYSQL *MyConnect(const char *host,const char *user,const char*password , const char *db){
	MYSQL *my=mysql_init(NULL);
	if( my==NULL ){
		fprintf( stderr , "Error code:%d Error message:%s line:%d\n" , mysql_errno( my ) , mysql_error(my) , __LINE__ );
		exit(0);
	}
	if( mysql_real_connect( my , host , user , password , NULL , 0 , NULL , 0 ) == NULL ){
		fprintf( stderr , "Error code:%d Error message:%s line:%d\n" , mysql_errno( my ) , mysql_error(my) , __LINE__ );
		exit(0);
	}
	if( mysql_select_db( my , db ) != 0 ){
		fprintf( stderr , "Error code:%d Error message:%s line:%d\n" , mysql_errno( my ) , mysql_error(my) , __LINE__ );
		exit(0);
	}

	return my;
}

int MyExecute(MYSQL *con,const char *query){
	if( mysql_query( con ,  query ) != 0 ){
		fprintf( stderr , "Error code:%d Error message:%s line:%d\n" , mysql_errno( con ) , mysql_error(con) , __LINE__ );
		return NULL;
	}
	return 1;
}

MY *MyQuery(MYSQL *con,const char* query){
	MY *my=(MY*)calloc(sizeof(MY),1);
	//Return zero is no error
	//"select current_timestamp() as CurrDate"
	if( mysql_query( con ,  query ) != 0 ){
		fprintf( stderr , "Error code:%d Error message:%s line:%d\n" , mysql_errno( con ) , mysql_error(con) , __LINE__ );
		free(my);
		return NULL;
	}
	//Return NULL is error
	if( ( my->res=mysql_store_result( con ) ) == NULL ){
		mysql_free_result( my->res ) ;
		free(my);
		fprintf( stderr , "Error code:%d Error message:%s line:%d\n" , mysql_errno( con ) , mysql_error(con) , __LINE__ );
		return NULL;
	}
	my->size=mysql_num_rows(my->res);
	my->row = (MYSQL_ROW*)calloc(sizeof(MYSQL_ROW),my->size);
	for(int i=0;i<my->size;i++){
		my->row[i] = mysql_fetch_row(my->res);
		//fprintf( stderr , "addr=%x 0=%s 1=%s\n" , *(my->row[i]) , (*my->row[i])[0] , (*my->row[i])[1] );
	}
	return my;
}

void FreeMyQuery(MY *my){
	mysql_free_result(my->res);
	free(my->row);
	free(my);
	return;
}

void MyClose(MYSQL *my){
	mysql_close( my ) ;	
}


int main(){
	MYSQL *dbh;
	dbh=MyConnect("localhost","root","","mysql");
	MY *qy=MyQuery(dbh,"select Host,User,Password from user limit 10");
	if( qy ) {
		printf( "size=%d\n" , qy->size ) ;
		for(int i=0;i<qy->size;i++){
			MYSQL_ROW row=*(qy->row);
			char buff_host[S_HOST_LEN+1];
			char buff_user[S_USER_LEN+1];
			char buff_password[S_PASSWORD_LEN+1];
			MyGetStr( buff_host , qy->row[i][0] , S_HOST_LEN );
			MyGetStr( buff_user , qy->row[i][1] , S_USER_LEN );
			MyGetStr( buff_password , qy->row[i][2] , S_PASSWORD_LEN );
			fprintf( stderr , "host=%s\t\tuser=%s\t\t\tpassword=%s\n" , buff_host , buff_user , buff_password );
		}
		FreeMyQuery(qy);
	}
	
	
	if(0){
		for( int i=0 ; i < 100000 ; i++ )
			MyExecute( dbh , "insert into t(t) values(1) " );
	}
	
	MyClose( dbh );
	return(0);
}

