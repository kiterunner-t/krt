#! /usr/bin/perl
# Copyleft (C) KRT, 2013 by kiterunner_t

use strict;
use warnings;
#use utf8;
use Encode;
use Math::BigFloat;


sub order_custom($$$;@);
sub utf8_to_gbk($);
sub gbk_to_utf8($);
sub custom_order($$);


# perl order.pl file1 file2 ... > result.csv
die "Usage: perl order.pl <files> ..." if $#ARGV < 0;


my @orders;
foreach my $file (@ARGV) {
  custom_order \@orders, $file;
}

print utf8_to_gbk "买家ID,订单编号,定制类型,花型,尺寸,数量\n";
foreach my $h_order (@orders) {
  my $line = join ",", $h_order->{custom_id}, $h_order->{order_id}, 
                       $h_order->{type}, $h_order->{color}, $h_order->{size},
                       $h_order->{number};

  print utf8_to_gbk $line, "\n";
}


sub custom_order($$) {
  my ($ref_orders, $file) = @_;
  open FH_ORDER, "< $file" or die "open $file error: $!";
  <FH_ORDER>;

  while (<FH_ORDER>) {
    chomp;
    $_ = gbk_to_utf8 $_;

    my ($order_id, $custom_id, $msg, $memo) = (split /,/)[0,1,11,23];
    # print "$order_id, $msg, $memo\n";

    $msg =~ s/ +/ /g;
    $memo =~ s/ +/ /g;

    $order_id = Math::BigFloat->new($order_id);

    if (index($msg, "定制")!=-1 || index($memo, "定制")!=-1) {
      order_custom $ref_orders, $custom_id, $order_id, $msg, $memo;
    }
  }

  close FH_ORDER;
}


sub order_custom($$$;@) {
  my ($ref_orders, $custom_id, $order_id, @ms) = @_;

  foreach my $m (@ms) {
    next if $m =~ /^ *$/;

    while ($m =~ /(打孔|挂钩)定制(?:：|;) ?(.*?) (宽.*?) (.*?)；/g) {
	  my %order;
      $order{custom_id} = $custom_id;
      $order{order_id} = $order_id;
      $order{type} = $1;
      $order{color} = $2;
      $order{size} = $3;
      $order{number} = $4;

      push @$ref_orders, \%order;
    }
  }
}


sub utf8_to_gbk($) {
  my ($str) = @_;

  encode "gbk", decode("utf-8", $str);
}


sub gbk_to_utf8($) {
  my ($str) = @_;

  encode "utf-8", decode("gbk", $_);
}

