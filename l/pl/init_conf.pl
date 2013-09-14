#! /usr/bin/perl
# Copyleft (C) KRT, 2012 by kiterunner_t

use strict;
use warnings;

sub plugins($);
sub print_product(\@\@);
sub getplugin(*);
sub getparam(*;$);
sub getline(*);
sub printplugin(*\%\@);
sub printparam(*\%\@);
sub compare_key($\@);
sub print_getparam(\%\%\@);


my $file = "system.conf";
my $plugins = plugins $file;
my @products = (
  [qw(s12500 mr standard)], 
  [qw(s12500 mr advanced)], 
  [qw(s5800 standard)], 
  [qw(s5800 advanced)], 
);

for (my $i = 0; $i <= $#products; $i++) {
  print join('_', @{products[$i]}), "_system.conf\n";
  print_product @{$products[$i]}, @$plugins;
}
print "\n";


sub plugins($) {
  my ($file) = @_;
  
  open HPLUGIN, $file or die "Open $file error.";
  my $num = 0;
  my @plugins;
  while (my $line = getline HPLUGIN) {
    $plugins[$num++] = getplugin HPLUGIN if $line =~ /\{\s*$/;
  }
  close HPLUGIN;
  \@plugins;
}


sub print_product(\@\@) {
  my ($product_keys, $plugin) = @_;
  
  my $file = (join '_', @product_keys) . '_system.conf';
  open HCONF, '>', $file or die "Open $file error.";
  for (my $j = 0; $j < @$plugin; $j++) {
    printplugin \*HCONF, %{$plugin->[$j]}, @$product_keys;
  }
  close HCONF;
}


sub getplugin(*) {
  my ($handle) = @_;
  
  my %plugin;
  my $flag = 0;

  # $plug{name} = (split /\s+/, getline $handle)[0];
  while (my $line = getline $handle) {
    if ($line =~ /^\s*\}\s*$/) {
      $flag = 1;
      last;
    }
    
    my @ins = split /\s+/, $line;
    if ($ins[$#ins] eq "{") {
      $plugin{$ins[0]} = getparam $handle, 0;
    } else {
      $plugin{$ins[0]} = $ins[1];
    }
  }
  
  die "Bad file." if $flag == 0;
  \%plugin;
}


sub getparam(*;$) {
  my ($handle, $level) = @_;
  
  my $flag = 0;
  my %param;
  while (my $line = getline $handle) {
    if ($line =~ /^\s*\}\s*$/) {
      $flag = 1;
      last;
    }
    
    # print $level, $line, "\n";
    my @ins = split /[\s=]+/, $line;
    if ($ins[$#ins] eq '{') {
      $param{$ins[0]} = getparam $handle, $level + 1;
    } else {
      $param{$ins[0]} = $ins[1];
    }
  }
  
  die "Bad file." if $flag == 0;
  \%param;
}


sub getline(*) {
  my ($handle) = @_;
  
  while (my $line = <$handle>) {
    next if $line =~ /^\s*#/;
    next if $line =~ /^\s*$/;
    
    chomp $line;
    $line =~ s/^\s+//;
    $line =~ s/\s+$//;
    return $line;
  }
}


sub printplugin(*\%\@) {
  my ($handle, $plugin, $product_key) = @_;
  
  print $handle "define plugin {\n";
  foreach my $key (sort { lc $a cmp lc $b } keys %$plugin) {
    if ($key ne "load" and $key ne "_load_param") {
      print $handle "    $key $plugin->{$key}\n";
    }
  }
  print $handle "    load $plugin->{load} \\\n";
  printparam $handle, %{$plugin->{_load_param}}, @$product_key;
  print $handle "}\n\n";
}


sub printparam(*\%\@) {
  my ($handle, $param_in, $product_key) = @_;
  
  my %param_out;
  print_getparam %param_out, %$param_in, @$product_key;
  my $num = 0;
  # print join('_', sort { lc $a cmp lc $b } keys %param_out), "\n";
  foreach my $key (sort { lc $a cmp lc $b } keys %$param_out) {
    if (!$param{$key} || $param_out{$key} =~ /^\s+$/) {
      print $handle "        --$key";
    } else {
      print $handle "        --$key=$param_out{$key}";
    }
    
    $num++;
    if ($num == keys %param_out) {
      print $handle "\n";
    } else {
      print $handle " \\\n";
    }
  }
}


sub compare_key($\@) {
  my ($key, $keys) = @_;
  
  foreach (@$keys) {
    return 1 if $_ eq $key;
  }
  0;
}


sub print_getparam(\%\%\@) {
  my ($param, $param_in, $product_key) = @_;
  
  foreach my $key (keys %$param_in) {
    if (ref $param_in->{$key} eq "HASH") {
      if (compare_key $key, @$product_key) {
        print_getparam %$param, %{$param_in->{$key}}, @$product_key;
      }
    } else {
      $param->{$key} = $param_in->{$key};
    }
  }
}

