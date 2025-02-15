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

use Encode;
use YAML::PP::Parser;
use YAML::PP::Common;
use Getopt::Long;
Getopt::Long::Configure('bundling');
GetOptions(
    'help|h' => \my $help,
    'module|M=s' => \my $module,
) or usage(1);

usage(0) if $help;

$module ||= 'YAML::PP';
if ($module eq 'YAML::PP') {
    $module = 'YAML::PP::Parser';
}
elsif ($module eq 'YAML::PP::LibYAML') {
    require YAML::PP::LibYAML::Parser;
    $module = 'YAML::PP::LibYAML::Parser';
}
elsif ($module eq 'YAML::PP::Ref') {
    require YAML::PP::Ref;
    $module = 'YAML::PP::Ref::Parser';
}

my ($file) = @ARGV;

my $parser = $module->new(
    receiver => sub {
      my ($self, undef, $event) = @_;
      print encode_utf8(YAML::PP::Common::event_to_test_suite($event, { flow => 1 })), "\n";
    },
    $file ? (reader => YAML::PP::Reader::File->new) : (),
);
if ($file) {
    $parser->parse_file($file);
}
else {
    my $yaml;
    $yaml = do { local $/; <STDIN> };
    $yaml = decode_utf8($yaml);
    $parser->parse_string($yaml);
}

sub usage {
    my ($rc) = @_;
    print <<"EOM";
Usage:

    $0 [options] < file
    $0 [options] file

Options:
    --module -M  Module to use for parsing. YAML::PP (default),
                 YAML::PP::LibYAML or YAML::PP::Ref
EOM
    exit $rc;
}
__END__
:endofperl
@set "ErrorLevel=" & @goto _undefined_label_ 2>NUL || @"%COMSPEC%" /d/c @exit %ErrorLevel%
