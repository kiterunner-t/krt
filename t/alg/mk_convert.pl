#! /usr/bin/env perl

use strict;
use warnings;

my $mk_file = $ARGV[0];
my $out_file = $mk_file;
$out_file =~ s/__$//;

open my $fd, "<", $mk_file or die $!;
open my $out_fd, ">", $out_file or die $!;

while (<$fd>) {
  if (/^(\w+)\.o\s*:\s*([a-zA-Z\/_]+)\/\1.c\s+(.*)$/) {
    print $out_fd "objs/$2/$1.o: $2/$1.c $3\n";
  } else {
    print $out_fd $_;
  }
}

close $fd;
close $out_fd;

