#! /usr/bin/env perl
# Copyleft (C) KRT, 2014 by kiterunner_t

use strict;
use warnings;
use Encode qw/encode decode/;
#use Data::Dumper;

sub utf8_to_gbk($);


open my $fh, "<", "test.sql" or die "open error: $!";

my $table_part;
my %table;
my $current_table;

while (<$fh>) {
  chomp;

  if (/^\s*create table\s+"(\w+?)"\."(\w+?)"/i) {
    $table_part = 1;
    $current_table = $2;
    $table{$current_table}{schema} = $1;
    $table{$current_table}{name} = $2;
    $table{$current_table}{name_ch} = "";
    next;
  }

  if (/^\s*\)\s+segment\s+/i) {
    $table_part = 0;
    next;
  }

  if (/\s*COMMENT \s+ ON \s+ TABLE \s+ "\w+?"\s*\.\s*"(\w+?)" \s+ IS 
      \s+ '(.*)'\s*;\s*$/ix
  ) {
    $table{$1}{name_ch} = $2;
    print $2 . "\n";
    next;
  }

  if (/\s*COMMENT \s+ ON \s+ COLUMN \s+ 
      "\w+?" \s* \. \s* "(\w+?)" \s* \. \s* "(\w+?)" \s+ IS 
      \s+ '(.*)'\s*;\s*$/ix
  ) {
    $table{$1}{field}{$2}{comment} = $3;
    print $3 . "\n";
    next;
  }

  if ($table_part) {
    if (/primary \s+ key\s+\("(\w+?)"\)/ix) {
      $table{$current_table}{field}{$1}{primary_key} = "Y";
      next;
    }

    my $not_null = "";

    if (/^\s*\(*\s*
        "(\w+)"                    # field_name
        \s+(\w+)                   # field_type
        (?:  \( ([0-9,]+?) \) )*   # field_length [optional]
        \s*(not \s null(?{ $not_null = "NOT NULL" }))*.*,*$/ix
    ) {
      my $field_name = $1;

      # init fields
      $table{$current_table}{field}{$field_name}{primary_key} = "";
      $table{$current_table}{field}{$field_name}{length} = "";
      $table{$current_table}{field}{$field_name}{comment} = "";

      $table{$current_table}{field}{$field_name}{name} = $field_name;
      $table{$current_table}{field}{$field_name}{type} = $2;
      $table{$current_table}{field}{$field_name}{length} = $3 if $3;
      $table{$current_table}{field}{$field_name}{not_null} = $not_null;
    }
  }
}

close $fh;


open my $fh_out, ">", "test.csv" or die "open error: $!";

print $fh_out 
  utf8_to_gbk "表,表名,字段,字段中文名,字段类型,字段长度,是否为NULL,是否为主键\n";

foreach my $ts (sort keys %table) {
  my $t = $table{$ts}{field};

  foreach my $f (keys $t) {
    print $fh_out "$ts," . utf8_to_gbk $table{$ts}{name_ch} . ",$t->{$f}{name},";
    print $fh_out utf8_to_gbk $t->{$f}{comment}
        . ",$t->{$f}{type},"
        . "$t->{$f}{length},$t->{$f}{not_null},$t->{$f}{primary_key}\n";
  }
}

close $fh_out;


sub utf8_to_gbk($) {
  my ($str) = @_;

  encode "GBK", decode "UTF-8", $str;
}

