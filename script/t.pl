#!/usr/bin/perl

use IO::Socket::INET;
use strict;

$| = 1;


while(1){
	&Rec("{\"FUN\":1}");
	last;
}

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
		my $line=$_;
		$line =~ s/\s+$//;
		print "Recv:$line\n";
	}
	$socket->close();
}
