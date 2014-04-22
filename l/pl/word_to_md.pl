#! /usr/bin/env perl
# Copyleft (C) KRT, 2014 by kiterunner_t
# ref: http://www.kiterunner.me/?p=427

use warnings;
use strict;
use Getopt::Long;
use Encode;
use Win32::OLE qw(in);
use Win32::OLE::Const 'Microsoft Word';
use Data::Dumper;


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

my %style = (
  BlockQuotation => $doc->Styles(wdStyleBlockQuotation)->NameLocal,
  BodyText => $doc->Styles(wdStyleBodyText)->NameLocal,
  BodyText2 => $doc->Styles(wdStyleBodyText2)->NameLocal,
  BodyText3 => $doc->Styles(wdStyleBodyText3)->NameLocal,
  BodyTextFirstIndent => $doc->Styles(wdStyleBodyTextFirstIndent)->NameLocal,
  BodyTextFirstIndent2 => $doc->Styles(wdStyleBodyTextFirstIndent2)->NameLocal,
  BodyTextIndent => $doc->Styles(wdStyleBodyTextIndent)->NameLocal,
  BodyTextIndent2 => $doc->Styles(wdStyleBodyTextIndent2)->NameLocal,
  BodyTextIndent3 => $doc->Styles(wdStyleBodyTextIndent3)->NameLocal,
  BookTitle => $doc->Styles(wdStyleBookTitle)->NameLocal,
  Caption => $doc->Styles(wdStyleCaption)->NameLocal,
  Closing => $doc->Styles(wdStyleClosing)->NameLocal,
  CommentReference => $doc->Styles(wdStyleCommentReference)->NameLocal,
  CommentText => $doc->Styles(wdStyleCommentText)->NameLocal,
  Date => $doc->Styles(wdStyleDate)->NameLocal,
  DefaultParagraphFont => $doc->Styles(wdStyleDefaultParagraphFont)->NameLocal,
  Emphasis => $doc->Styles(wdStyleEmphasis)->NameLocal,
  EndnoteReference => $doc->Styles(wdStyleEndnoteReference)->NameLocal,
  EndnoteText => $doc->Styles(wdStyleEndnoteText)->NameLocal,
  EnvelopeAddress => $doc->Styles(wdStyleEnvelopeAddress)->NameLocal,
  EnvelopeReturn => $doc->Styles(wdStyleEnvelopeReturn)->NameLocal,
  Footer => $doc->Styles(wdStyleFooter)->NameLocal,
  FootnoteReference => $doc->Styles(wdStyleFootnoteReference)->NameLocal,
  FootnoteText => $doc->Styles(wdStyleFootnoteText)->NameLocal,
  Header => $doc->Styles(wdStyleHeader)->NameLocal,
  Heading1 => $doc->Styles(wdStyleHeading1)->NameLocal,
  Heading2 => $doc->Styles(wdStyleHeading2)->NameLocal,
  Heading3 => $doc->Styles(wdStyleHeading3)->NameLocal,
  Heading4 => $doc->Styles(wdStyleHeading4)->NameLocal,
  Heading5 => $doc->Styles(wdStyleHeading5)->NameLocal,
  Heading6 => $doc->Styles(wdStyleHeading6)->NameLocal,
  Heading7 => $doc->Styles(wdStyleHeading7)->NameLocal,
  Heading8 => $doc->Styles(wdStyleHeading8)->NameLocal,
  Heading9 => $doc->Styles(wdStyleHeading9)->NameLocal,
  HtmlAcronym => $doc->Styles(wdStyleHtmlAcronym)->NameLocal,
  HtmlAddress => $doc->Styles(wdStyleHtmlAddress)->NameLocal,
  HtmlCite => $doc->Styles(wdStyleHtmlCite)->NameLocal,
  HtmlCode => $doc->Styles(wdStyleHtmlCode)->NameLocal,
  HtmlDfn => $doc->Styles(wdStyleHtmlDfn)->NameLocal,
  HtmlKbd => $doc->Styles(wdStyleHtmlKbd)->NameLocal,
  HtmlNormal => $doc->Styles(wdStyleHtmlNormal)->NameLocal,
  HtmlPre => $doc->Styles(wdStyleHtmlPre)->NameLocal,
  HtmlSamp => $doc->Styles(wdStyleHtmlSamp)->NameLocal,
  HtmlTt => $doc->Styles(wdStyleHtmlTt)->NameLocal,
  HtmlVar => $doc->Styles(wdStyleHtmlVar)->NameLocal,
  Hyperlink => $doc->Styles(wdStyleHyperlink)->NameLocal,
  HyperlinkFollowed => $doc->Styles(wdStyleHyperlinkFollowed)->NameLocal,
  Index1 => $doc->Styles(wdStyleIndex1)->NameLocal,
  Index2 => $doc->Styles(wdStyleIndex2)->NameLocal,
  Index3 => $doc->Styles(wdStyleIndex3)->NameLocal,
  Index4 => $doc->Styles(wdStyleIndex4)->NameLocal,
  Index5 => $doc->Styles(wdStyleIndex5)->NameLocal,
  Index6 => $doc->Styles(wdStyleIndex6)->NameLocal,
  Index7 => $doc->Styles(wdStyleIndex7)->NameLocal,
  Index8 => $doc->Styles(wdStyleIndex8)->NameLocal,
  Index9 => $doc->Styles(wdStyleIndex9)->NameLocal,
  IndexHeading => $doc->Styles(wdStyleIndexHeading)->NameLocal,
  IntenseEmphasis => $doc->Styles(wdStyleIntenseEmphasis)->NameLocal,
  IntenseQuote => $doc->Styles(wdStyleIntenseQuote)->NameLocal,
  IntenseReference => $doc->Styles(wdStyleIntenseReference)->NameLocal,
  LineNumber => $doc->Styles(wdStyleLineNumber)->NameLocal,
  List => $doc->Styles(wdStyleList)->NameLocal,
  List2 => $doc->Styles(wdStyleList2)->NameLocal,
  List3 => $doc->Styles(wdStyleList3)->NameLocal,
  List4 => $doc->Styles(wdStyleList4)->NameLocal,
  List5 => $doc->Styles(wdStyleList5)->NameLocal,
  ListBullet => $doc->Styles(wdStyleListBullet)->NameLocal,
  ListBullet2 => $doc->Styles(wdStyleListBullet2)->NameLocal,
  ListBullet3 => $doc->Styles(wdStyleListBullet3)->NameLocal,
  ListBullet4 => $doc->Styles(wdStyleListBullet4)->NameLocal,
  ListBullet5 => $doc->Styles(wdStyleListBullet5)->NameLocal,
  ListContinue => $doc->Styles(wdStyleListContinue)->NameLocal,
  ListContinue2 => $doc->Styles(wdStyleListContinue2)->NameLocal,
  ListContinue3 => $doc->Styles(wdStyleListContinue3)->NameLocal,
  ListContinue4 => $doc->Styles(wdStyleListContinue4)->NameLocal,
  ListContinue5 => $doc->Styles(wdStyleListContinue5)->NameLocal,
  ListNumber => $doc->Styles(wdStyleListNumber)->NameLocal,
  ListNumber2 => $doc->Styles(wdStyleListNumber2)->NameLocal,
  ListNumber3 => $doc->Styles(wdStyleListNumber3)->NameLocal,
  ListNumber4 => $doc->Styles(wdStyleListNumber4)->NameLocal,
  ListNumber5 => $doc->Styles(wdStyleListNumber5)->NameLocal,
  ListParagraph => $doc->Styles(wdStyleListParagraph)->NameLocal,
  MacroText => $doc->Styles(wdStyleMacroText)->NameLocal,
  MessageHeader => $doc->Styles(wdStyleMessageHeader)->NameLocal,
  NavPane => $doc->Styles(wdStyleNavPane)->NameLocal,
  Normal => $doc->Styles(wdStyleNormal)->NameLocal,
  NormalIndent => $doc->Styles(wdStyleNormalIndent)->NameLocal,
  NormalObject => $doc->Styles(wdStyleNormalObject)->NameLocal,
  NormalTable => $doc->Styles(wdStyleNormalTable)->NameLocal,
  NoteHeading => $doc->Styles(wdStyleNoteHeading)->NameLocal,
  PageNumber => $doc->Styles(wdStylePageNumber)->NameLocal,
  PlainText => $doc->Styles(wdStylePlainText)->NameLocal,
  Quote => $doc->Styles(wdStyleQuote)->NameLocal,
  Salutation => $doc->Styles(wdStyleSalutation)->NameLocal,
  Signature => $doc->Styles(wdStyleSignature)->NameLocal,
  Strong => $doc->Styles(wdStyleStrong)->NameLocal,
  Subtitle => $doc->Styles(wdStyleSubtitle)->NameLocal,
  SubtleEmphasis => $doc->Styles(wdStyleSubtleEmphasis)->NameLocal,
  SubtleReference => $doc->Styles(wdStyleSubtleReference)->NameLocal,
  TableColorfulGrid => $doc->Styles(wdStyleTableColorfulGrid)->NameLocal,
  TableColorfulList => $doc->Styles(wdStyleTableColorfulList)->NameLocal,
  TableColorfulShading => $doc->Styles(wdStyleTableColorfulShading)->NameLocal,
  TableDarkList => $doc->Styles(wdStyleTableDarkList)->NameLocal,
  TableLightGrid => $doc->Styles(wdStyleTableLightGrid)->NameLocal,
  TableLightGridAccent1 => $doc->Styles(wdStyleTableLightGridAccent1)->NameLocal,
  TableLightList => $doc->Styles(wdStyleTableLightList)->NameLocal,
  TableLightListAccent1 => $doc->Styles(wdStyleTableLightListAccent1)->NameLocal,
  TableLightShading => $doc->Styles(wdStyleTableLightShading)->NameLocal,
  TableLightShadingAccent1 => $doc->Styles(wdStyleTableLightShadingAccent1)->NameLocal,
  TableMediumGrid1 => $doc->Styles(wdStyleTableMediumGrid1)->NameLocal,
  TableMediumGrid2 => $doc->Styles(wdStyleTableMediumGrid2)->NameLocal,
  TableMediumGrid3 => $doc->Styles(wdStyleTableMediumGrid3)->NameLocal,
  TableMediumList1 => $doc->Styles(wdStyleTableMediumList1)->NameLocal,
  TableMediumList1Accent1 => $doc->Styles(wdStyleTableMediumList1Accent1)->NameLocal,
  TableMediumList2 => $doc->Styles(wdStyleTableMediumList2)->NameLocal,
  TableMediumShading1 => $doc->Styles(wdStyleTableMediumShading1)->NameLocal,
  TableMediumShading1Accent1 => $doc->Styles(wdStyleTableMediumShading1Accent1)->NameLocal,
  TableMediumShading2 => $doc->Styles(wdStyleTableMediumShading2)->NameLocal,
  TableMediumShading2Accent1 => $doc->Styles(wdStyleTableMediumShading2Accent1)->NameLocal,
  TableOfAuthorities => $doc->Styles(wdStyleTableOfAuthorities)->NameLocal,
  TableOfFigures => $doc->Styles(wdStyleTableOfFigures)->NameLocal,
  Title => $doc->Styles(wdStyleTitle)->NameLocal,
  TOAHeading => $doc->Styles(wdStyleTOAHeading)->NameLocal,
  TOC1 => $doc->Styles(wdStyleTOC1)->NameLocal,
  TOC2 => $doc->Styles(wdStyleTOC2)->NameLocal,
  TOC3 => $doc->Styles(wdStyleTOC3)->NameLocal,
  TOC4 => $doc->Styles(wdStyleTOC4)->NameLocal,
  TOC5 => $doc->Styles(wdStyleTOC5)->NameLocal,
  TOC6 => $doc->Styles(wdStyleTOC6)->NameLocal,
  TOC7 => $doc->Styles(wdStyleTOC7)->NameLocal,
  TOC8 => $doc->Styles(wdStyleTOC8)->NameLocal,
  TOC9 => $doc->Styles(wdStyleTOC9)->NameLocal,
);

if ($debug) {
  foreach my $s (sort keys %style) {
    print $fd "$s ---> $style{$s}\n";
  }
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


sub utf8_to_gbk($) {
  my ($str) = @_;

  encode "gbk", decode("utf-8", $str);
}


sub gbk_to_utf8($) {
  my ($str) = @_;

  encode "utf-8", decode("gbk", $str);
}


my $last_end = 0;
my $ref_num = 0;
my $begin_indent = 0;
my $last_indent = 0;
my $indent;
my @refs;
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

  if ($debug) {
    print $fd "[$start -> $end] ";
    print $fd "style --> " . $paragraph->Format->Style->Type . "-----";

    print $fd $style . "\n";
    print $fd "[debug] " . $text . "\n";
  }

  if ($style eq $style{Title}) {
    # ignore

  } elsif ($style eq $style{Heading1}) {
    print $fd "# $text";

  } elsif ($style eq $style{Heading2}) {
    print $fd "## $text";

  } elsif ($style eq $style{Heading3}) {
    print $fd "### $text";

  } elsif ($style eq $style{ListParagraph}) {
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
    print $fd " " x $indent . "* $text";

  } elsif ($style eq $style{NormalObject}) {
    print $fd "    $text";

  } elsif ($style eq $style{Emphasis}) {
    print $fd "**$text**";

  } elsif ($style eq $style{Caption}) {
    my $table_cn = utf8_to_gbk "表格";
    my $picture_cn = utf8_to_gbk "图";

    if ($text =~ /^\s*(?:$table_cn|$picture_cn)\s*\d+\s+(.*)$/) {
      my $name = $1;
      $name =~ s/[\r\n]//g;
      $ref_num++;
      push @refs, $name;
      print $fd "\n![$name][$ref_num]\n";
    }

  } else {
    print $fd $text;
  }
}

my $num = 0;
$out_path =~ s/\/*$/\// if ! $out_path eq "";
print $fd "\n";
foreach my $ref (@refs) {
  $num++;
  print $fd "[$num]: $out_path" . "$ref.$picture_format \"$ref\"\n";
}

close $fd;
if ($close_word) {
  $doc->Close;
}

