#!/usr/bin/perl

use IO::Socket::INET;
$| = 1;


for(my $i=0;$i<1000;$i++){
	&Rec() if( fork()== 0 ) ;
}

sub Rec{
	my ($socket,$client_socket);
	$socket = new IO::Socket::INET (
		PeerHost => '127.0.0.1',
		PeerPort => '3333',
		Proto => 'tcp'
	) or die "ERROR in Socket Creation : $!\n";
	while(<$socket>){
		#loop...
	}
=comment
	print "TCP Connection Success.\n";
	print "====Rec====\n";
	while( <$socket> ){
		$line=$_;
		print $line;
	}
	print "====Rec====\n";
=cut
	
	$socket->close();
}
