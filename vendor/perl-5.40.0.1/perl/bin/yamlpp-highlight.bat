@rem = '--*-Perl-*--
@set "ErrorLevel="
@if "%OS%" == "Windows_NT" @goto WinNT
@perl -x -S "%0" %1 %2 %3 %4 %5 %6 %7 %8 %9
@set ErrorLevel=%ErrorLevel%
@goto endofperl
:WinNT
@perl -x -S %0 %*
@set ErrorLevel=%ErrorLevel%
@if NOT "%COMSPEC%" == "%SystemRoot%\system32\cmd.exe" @goto endofperl
@if %ErrorLevel% == 9009 @echo You do not have Perl in your PATH.
@goto endofperl
@rem ';
#!/usr/bin/perl
#line 16
use strict;
use warnings;

use YAML::PP::Highlight;
use Encode;
use Getopt::Long;

GetOptions(
    'help|h' => \my $help,
    'expand-tabs|et!' => \my $expand_tabs,
) or usage(1);

$expand_tabs = 1 unless defined $expand_tabs;

usage(0) if $help;

my ($file) = @ARGV;
my $yaml;

unless ($file) {
    $yaml = do { local $/; <STDIN> };
    $yaml = decode_utf8($yaml);
}

my $error;
my $tokens;
if (defined $file) {
    ($error, $tokens) = YAML::PP::Parser->yaml_to_tokens( file => $file );
}
else {
    ($error, $tokens) = YAML::PP::Parser->yaml_to_tokens( string => $yaml );
}
my $highlighted = YAML::PP::Highlight->ansicolored($tokens, expand_tabs => $expand_tabs);
print encode_utf8 $highlighted;
if ($error) {
    die $error;
}

sub usage {
    my ($rc) = @_;
    print <<"EOM";
Usage:

    $0 [options] < file
    $0 [options] file

Options:
    --expand-tabs    --et         Expand tabs to 8 spaces (default true)
    --no-expand-tabs --no-et      Don't expand tabs
EOM
    exit $rc;
}

__END__
:endofperl
@set "ErrorLevel=" & @goto _undefined_label_ 2>NUL || @"%COMSPEC%" /d/c @exit %ErrorLevel%
