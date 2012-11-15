#!/usr/bin/perl

use IO::Socket::INET;
use strict;

$| = 1;

my $n=100;
while($n--){
	fork() ;
	&Rec("{\"FUN\":$n}");
}

#while(1){sleep(100);}

sub Rec{
	my $input_str=shift;
	my ($socket,$client_socket);
	$socket = new IO::Socket::INET (
		PeerHost => '127.0.0.1',
		PeerPort => '5555',
		Proto => 'tcp'
	) or die "ERROR in Socket Creation : $!\n";
	print "TCP Connection Success.\n";
	print $socket $input_str;
	print "Send:$input_str\n";
	while( <$socket> ){
	#	my $line=$_;
	#	sleep(10);
	}
	return $socket;
	#$socket->close();
}
