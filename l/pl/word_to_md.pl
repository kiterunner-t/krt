# Copyleft (C) KRT, 2014 by kiterunner_t
# ref: http://www.kiterunner.me/?p=427

use warnings;
use strict;
use Getopt::Long;
use Encode;
use Win32::OLE qw(in);
# use Data::Dumper;


sub gbk_to_utf8($);
sub utf8_to_gbk($);
sub is_shape($);
sub is_table($);
sub print_paragraph_text($);
sub chomp_text($);


use constant {
  PICTURE_CN => "图",
  TABLE_CN   => "表格",
};


if ($^O ne "MSWin32") {
  print "THE SCRIPT CAN BE RUNNING IN WINDOWS SYSTEM\n";
  exit 2;
}

my $debug;
my $verbose;
my $out_file;
my $out_path = "";
my $picture_format = "png";

GetOptions(
    "verbose|v+"         => \$verbose,
    "debug|d+"           => \$debug,
    "output|o=s"         => \$out_file,
    "path|p=s"           => \$out_path,
    "picture-format|f=s" => \$picture_format,
);

die <<"_EOF_"
Usage: perl word_to_md.pl [options] xx.docx
    -d                 <debug option>
    -o out-file.md
    -p picture-path
    -f picture-format
_EOF_
  unless @ARGV == 1;

my $file = $ARGV[0];
my $file_withpath = Win32::GetCwd() . "/$file" if $file !~ /^(\w:)?[\/\\]/;
die "file $file does not exist" unless -f $file_withpath;

my $fd;
if ($out_file) {
  open $fd, ">", $out_file or die "open file error: $!";
} else {
  $fd = *STDOUT;
}

my $word = Win32::OLE->GetActiveObject("Word.Application");
unless (defined $word) {
  $word = Win32::OLE->new('Word.Application', 'Quit') or die "Couldn't run Word";
}

my $close_word;
my $doc;
foreach my $d (in $word->Documents) {
  if ($d->Name eq $file) {
    $doc = $d;
    last;
  }
}

unless (defined $doc) {
  $close_word = 1;
  $doc = $word->Documents->Open($file_withpath);
}

my %styles = (
  Title         => "标题",
  Heading1      => "标题 1",
  Heading2      => "标题 2",
  Heading3      => "标题 3",
  Caption       => "题注",
  Emphasis      => "",
  ListParagraph => "列出段落",
  NormalObject  => "无间隔,program",
  text          => "正文",
);

foreach my $k (keys %styles) {
  $styles{$k} = utf8_to_gbk $styles{$k};
}

my @links;
foreach my $link (in $doc->Hyperlinks) {
  my $l = {
    start => $link->Range->Start,
    end   => $link->Range->End,
    text  => $link->Range->Text,
    url   => $link->Address,
  };

  push @links, $l;
}


my $last_end = 0;
my $ref_num = 0;
my $begin_indent = 0;
my $last_indent = 0;
my $indent;
my @refs;

my @urls;

my $head1 = 0;
my $head2 = 0;
my $head3 = 0;

my $program = 0;

my $table_cn = utf8_to_gbk TABLE_CN;
my $picture_cn = utf8_to_gbk PICTURE_CN;

foreach my $paragraph (in $doc->Paragraphs) {
  my $start = $paragraph->Range->start;
  my $end = $paragraph->Range->end;

  next if $start < $last_end;

  if ($last_end = is_table($start)) {
    next;
  } elsif ($last_end = is_shape($start)) {
    next;
  }

  my $style = $paragraph->Format->Style->NameLocal;
  my $text = $paragraph->Range->Text;
  my $left_indent = int($paragraph->Format->LeftIndent);

  $text =~ s/[\r\n]*$/\n/;

  if ($debug) {
    print $fd "[debug] style --> " . $paragraph->Format->Style->Type;
    print $fd "[$start -> $end]-----$style--->>>$text\n";
  }

  $program = 0 if $style ne $styles{NormalObject};
  $last_indent = 0 if $style ne $styles{ListParagraph};

  if ($style eq $styles{Title}) {
    # ignore

  } elsif ($style eq $styles{Heading1}) {
    $head1++;
    $head2 = 0;
    $head3 = 0;
    print $fd "# $head1 $text";

  } elsif ($style eq $styles{Heading2}) {
    $head2++;
    $head3 = 0;
    print $fd "## $head1.$head2 $text";

  } elsif ($style eq $styles{Heading3}) {
    $head3++;
    print $fd "### $head1.$head2.$head3 $text";

  } elsif ($style eq $styles{ListParagraph}) {
    if ($last_indent == 0) {
      print $fd "\n";
      $begin_indent = $left_indent;
    } elsif ($last_indent < $left_indent) {
      print $fd "\n";
    } elsif ($last_indent > $left_indent) {
      print $fd "\n";
    }

    $indent = int(($left_indent - $begin_indent) / 28) * 4;
    $last_indent = $left_indent;
    print $fd " " x $indent . "* ";
    print_paragraph_text $paragraph;

  } elsif ($style eq $styles{NormalObject}) {
    $program++;
    print $fd "\n" if $program == 1;
    print $fd "    ";
    print_paragraph_text $paragraph;

  } elsif ($style eq $styles{Emphasis}) {
    print $fd "**";
    print_paragraph_text $paragraph;
    print $fd "**";

  } elsif ($style eq $styles{Caption}) {
    if ($text =~ /^\s*(?:$table_cn|$picture_cn)\s*\d+\s+(.*)$/) {
      my $name = $1;
      $name =~ s/[\r\n]//g;
      $ref_num++;
      push @refs, $name;
      print $fd "\n![$name][$ref_num]\n";
    }

  } else {
    print_paragraph_text $paragraph;
  }
}

my $num = 0;
$out_path =~ s/\/*$/\// if ! $out_path eq "";
print $fd "\n";
foreach my $ref (@refs) {
  $num++;
  print $fd "[$num]: $out_path" . "$ref.$picture_format \"$ref\"\n";
}

foreach my $url (@urls) {
  $num++;
  print $fd "[$num]: $url\n";
}

close $fd;
if ($close_word) {
  $doc->Close;
}


sub utf8_to_gbk($) {
  my ($str) = @_;

  encode "gbk", decode("utf-8", $str);
}


sub gbk_to_utf8($) {
  my ($str) = @_;

  encode "utf-8", decode("gbk", $str);
}


sub is_table($) {
  my ($start) = @_;

  foreach my $table (in $doc->Tables) {
    if ($table->range->start == $start) {
      return $table->range->end;
    }
  }

  0;
}


sub is_shape($) {
  my ($start) = @_;

  foreach my $shape (in $doc->InlineShapes) {
    if ($shape->range->start == $start) {
      return $shape->range->end;
    }
  }

  0;
}


sub chomp_text($) {
  my ($text) = @_;
  $text =~ s/[\r\n]+$/\n/;
  $text;
}


sub print_paragraph_text($) {
  my ($paragram) = @_;

  if ($debug) {
    print $fd "[debug] 1 ", $paragram->Range->Start, " ", $paragram->Range->End, "\n";
  }

  # If Range is [n, n+1], the method Range->Start will be fail
  if ($paragram->Range->End - $paragram->Range->Start == 1) {
    print $fd chomp_text($paragram->Range->Text);
    return ;
  }

  my $text_range = $paragram->Range;
 
  foreach my $l (@links) {
    if ($l->{start} >= $text_range->Start && $l->{start} <= $text_range->End) {
      my $pre_link = $doc->Range($text_range->Start, $l->{start});

      if ($debug) {
        print $fd "[debug] 00 ", $pre_link->Start, " ", $pre_link->End, "\n";
      }

      $ref_num++;
      push @urls, $l->{url};
      print $fd chomp_text($pre_link->Text), "[", $l->{text}, "][$ref_num]";
      last if $l->{end} == $text_range->End;
      $text_range = $doc->Range($l->{end}, $text_range->End);
    }
  }

  print $fd chomp_text($text_range->Text);
}


__END__

# Change Log (MD)

* 2014/05/04 add the new features and fix bugs as follows

    * support (and test) office word 2013
    * support hyperlink
    * enhance head1-head3 for section numbers
    * fix listparagraph "first line" bug

* 2014/04/22 write the first version

    * support (and only test) office word 2007
    * support the features, suach as head1-head3, codes, listparagraph, pictures and tables

