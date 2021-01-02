#!/usr/bin/perl
use strict;
use warnings;

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

open (NODES, "<".$ARGV[0]);
my @nodes_raw = <NODES>;
close (NODES);
#print("loaded node file\n");

open (NODES_CLEAN, ">nodes_clean.csv");
my @node_dex;
foreach my $row (@nodes_raw) {
	my @fields = split('\|', $row);
	#push(@node_dex, $fields[1]);
	print NODES_CLEAN (join('|', $fields[1],$fields[2],$fields[9],$fields[10]));
	
}
close (NODES_CLEAN);
print("nodes cleared\n");

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

open (WAYS, "<".$ARGV[1]);
my @ways_raw = <WAYS>;
close (WAYS);

open (EDGES_CLEAN, ">ways_clean.csv");
foreach my $row (@ways_raw) {
	my @fields = split('\|', $row);
	print EDGES_CLEAN (join('|', @fields[1,7,9..$#fields]));
}
close (EDGES_CLEAN);
print("edges cleared\n");

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~