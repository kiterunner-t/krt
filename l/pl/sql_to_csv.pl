#! /usr/bin/env perl
# Copyleft (C) KRT, 2014 by kiterunner_t

use strict;
use warnings;
use Getopt::Long;
use Encode qw/encode decode/;
#use Data::Dumper;

sub utf8_to_gbk($);


my $file_out;
my $verbose;

GetOptions(
    "verbose|v+" => \$verbose,
    "output|o=s" => \$file_out,
);

if ($verbose) {
  print "Usage: \n";
  print "    perl sql_to_csv.pl [-v] [-o out_file.csv] xxx.sql\n";
  exit;
}

die "Usage: perl sql_to_csv.pl [-o out_file.csv] xxx.sql" unless @ARGV == 1;
my $file_in = $ARGV[0];

open my $fh, "<", $file_in or die "open error: $!";

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

  if (/^\s*\)\s* (?: segment.* | tablespace.* | ;)*\s*$/ix) {
    $table_part = 0;
    next;
  }

  if (/\s*COMMENT \s+ ON \s+ TABLE \s+ "\w+?"\s*\.\s*"(\w+?)" \s+ IS 
      \s+ '(.*)'\s*;\s*$/ix
  ) {
    $table{$1}{name_ch} = $2;
    next;
  }

  if (/\s*COMMENT \s+ ON \s+ COLUMN \s+ 
      "\w+?" \s* \. \s* "(\w+?)" \s* \. \s* "(\w+?)" \s+ IS 
      \s+ '(.*)'\s*;\s*$/ix
  ) {
    $table{$1}{field}{$2}{comment} = $3;
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
        (?:  \( (.+?) \) )*        # field_length [optional]
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

my $fh_out = *STDOUT;
if ($file_out) {
  open $fh_out, ">", $file_out or die "open error: $!";
}

print $fh_out 
  utf8_to_gbk "表,表名,字段,字段中文名,字段类型,字段长度,是否为NULL,是否为主键\n";

foreach my $ts (sort keys %table) {
  my $t = $table{$ts}{field};

  foreach my $f (keys $t) {
    $t->{$f}{length} =~ s/,/./g;
    $t->{$f}{comment} =~ s/,/./g;

    print $fh_out "$ts," . utf8_to_gbk $table{$ts}{name_ch} . ",$t->{$f}{name},";
    print $fh_out utf8_to_gbk $t->{$f}{comment}
        . ",$t->{$f}{type},"
        . "$t->{$f}{length},$t->{$f}{not_null},$t->{$f}{primary_key}\n";
  }
}

if ($file_out) {
  close $fh_out;
}


sub utf8_to_gbk($) {
  my ($str) = @_;

  encode "GBK", decode "UTF-8", $str;
}


__END__

=COMMENT
This script is tested by the input file which is produced by the following 
sql script in Oracle 11g. 

  begin
    declare str varchar2(1000);
      str_comments varchar2(1000);

    begin
      -- exec dbms_metadata.set_transform_param(
      --   dbms_metadata.session_transform, 'SQLTERMINATOR', TRUE);

      str_comments := 
        'select dbms_metadata.get_dependent_ddl(''''COMMENT'', table_name)' ||
        'from (' ||
          'select distinct a.table_name table_name ' ||
          'from user_col_comments a, user_objects b ' ||
          'where a.table_name=b.object_name and b.object_type=''TABLE'' ' ||
            'and a.comments is not null)';

      str := 
        'select dbms_metadata.get_ddl(''TABLE'', table_name, ''BILL'') ' ||
        'from user_tables order by table_name';

        unload('UNLOAD_DIR', 'hd.sql', str);
        unload('UNLOAD_DIR', 'hd_comment.sql', str_comments);
    end;
  end;
  /

=cut

