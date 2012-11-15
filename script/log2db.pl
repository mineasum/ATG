#!/usr/bin/perl


use DBI;
use strict;


&main();

sub main{
	my $dbh = &ConnectDB( "localhost" , "root" , "" , "atg" );
	if(!$dbh){ print "Connect db is failed\n"; return}
	
	&MainLoop($dbh);
	return;
}


# dbi:DriverName:database=database_name;host=hostname;port=port
# $dbh = DBI->connect($data_source, $user, $pass, {
#               PrintError => 0,
#               AutoCommit => 0
#         });

sub ConnectDB{
	my ($host,$user,$password,$db) = @_;
	my $dbh =DBI->connect( "dbi:mysql:database=atg;host=$host;port=3306" , $user , $password ,
				{ PrintError => 1 , AutoCommit => 1 } );
}


sub MainLoop{
	my ( $dbh ) = @_;

	while(1){last;}
	my $query = "insert into ATGLOG_201211( fun , txt ) values( '0' , '...' )";
	#$dbh->execute( $query ) ;

}

