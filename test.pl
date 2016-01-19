#!/usr/bin/perl -w 
use strict; 
use IO::Socket; 

main:
{
  return -1 if(@ARGV < 2);
    my $host = $ARGV[0]; 
    my $port = $ARGV[1];  
    my $sock = new IO::Socket::INET( PeerAddr => $host, PeerPort => $port, Proto => 'tcp'); 
    $sock or die "no socket :$!"; 
    my $msg;
    while ( 1 ) {
        $sock->send("000000010400000000020000000000050407008000300001090000300400200050100000000806000\r\n");
        $sock->recv($msg, 1024);
        print $msg . "\n";
        sleep(1);
    }
    
    close $sock;
}
