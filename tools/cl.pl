#! /usr/bin/env perl
# Copyleft (C) KRT, 2014 by kiterunner_t


use strict;
use warnings;

# use Data::Dumper;


my @files;

if (!@ARGV) {
  print "Usage: cl.pl <files-or-dirs> ...\n";
  exit 1;
}

for my $f (@ARGV) {
  if (-f $f && $f=~/\.[ch]$/) {
    push @files, $f;
    next;
  }

  if (-d $f) {
    push @files, `find $f -name "*.[ch]"`;
  }
}

my $total_lines = 0;

foreach my $file (@files) {
  $file =~ s/^\.\///;
  $file =~ s/[\r\n]+//g;

  open my $fd, "<", $file or die "open $file error: $!";
  my $tmp_count = 0;
  while (<$fd>) {
    next if /^\s*$/;
    next if /^\s*\/\//;
    next if /^\s*\/\*.*\*\/\s*$/;

    if (/\s*\/\*.*\*\//) {
      # ignore inline comment ...

    } elsif (/\/\*/) {
      s/\/\*.*$//;
      s/\s+//g;
      $tmp_count++ if length > 0;

      while (<$fd>) {
        if (/\*\//) {
          s/^.*\*\///;
          s/\s+//g;
          $tmp_count++ if length > 0;
          last;
        }
      }
      next;
    }

    $tmp_count++;
  }
  close $fd;

  print "$tmp_count $file\n";
  $total_lines += $tmp_count;
}

print "    total lines: ", $total_lines, "\n";

