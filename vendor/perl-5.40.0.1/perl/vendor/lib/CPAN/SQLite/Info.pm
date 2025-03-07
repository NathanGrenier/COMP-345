# $Id: Info.pm 85 2022-10-29 05:44:36Z stro $

package CPAN::SQLite::Info;
use strict;
use warnings;

our $VERSION = '0.220';

use English qw/-no_match_vars/;

use CPAN::DistnameInfo;
use File::Spec::Functions qw(catfile);
use Compress::Zlib;
use File::Basename;
use Safe;
use CPAN::SQLite::Util qw(vcmp print_debug);

my $ext = qr/\.(tar\.gz|tar\.Z|tgz|zip)$/;

sub new {
  my ($class, %args) = @_;
  my $self = { dists => {}, auths => {}, mods => {}, info => {}, %args };
  return bless $self, $class;
}

sub fetch_info {
  my $self = shift;
  $self->mailrc()         or return;
  $self->dists_and_mods() or return;
  return 1;
}

sub dists_and_mods {
  my $self = shift;
  my ($packages, $cpan_files) = $self->packages();

  my ($dists, $mods);
  my $ignore = $self->{ignore};
  my $pat;
  if ($ignore and ref($ignore) eq 'ARRAY') {
    $pat = join '|', @$ignore;
  }
  foreach my $cpan_file (keys %$cpan_files) {
    if ($pat and ($cpan_file =~ /^($pat)/)) {
      delete $cpan_files->{$cpan_file};
      print_debug("Ignoring $cpan_file\n");
      next;
    }
    my $d = CPAN::DistnameInfo->new($cpan_file);
    next unless ($d->maturity eq 'released');
    my $dist_name = $d->dist;
    my $dist_vers = $d->version;
    my $cpanid    = $d->cpanid;
    my $dist_file = $d->filename;
    unless ($dist_name and $dist_vers and $cpanid) {
      print_debug("No dist_name/version/cpanid for $cpan_file: skipping\n");
      delete $cpan_files->{$cpan_file};
      next;
    }

    # ignore specified dists
    if ($pat and ($dist_name =~ /^($pat)$/)) {
      delete $cpan_files->{$cpan_file};
      print_debug("Ignoring $dist_name\n");
      next;
    }
    if (not $dists->{$dist_name} or vcmp($dist_vers, $dists->{$dist_name}->{dist_vers}) > 0) {
      $dists->{$dist_name}->{dist_vers} = $dist_vers;
      $dists->{$dist_name}->{dist_file} = $dist_file;
      $dists->{$dist_name}->{cpanid}    = $cpanid;
    }
  }

  my $wanted;
  foreach my $dist_name (keys %$dists) {
    $wanted->{ basename($dists->{$dist_name}->{dist_file}) } = $dist_name;
  }
  foreach my $mod_name (keys %$packages) {
    my $file      = basename($packages->{$mod_name}->{dist_file});
    my $dist_name = $wanted->{$file};
    unless ($dist_name and $dists->{$dist_name}) {
      delete $packages->{$mod_name};
      next;
    }
    $mods->{$mod_name}->{dist_name} = $dist_name;
    $dists->{$dist_name}->{modules}->{$mod_name}++;
    $mods->{$mod_name}->{mod_vers} = $packages->{$mod_name}->{mod_vers};
  }
  $self->{dists} = $dists;
  return $self->{mods} = $mods;
}

sub modlist {
  my $self = shift;
  warn 'Modlist does not contain any useful info anymore';
  return;
}

sub packages {
  my $self  = shift;
  my $index = 'modules/02packages.details.txt.gz';
  my $packages =
    $self->{keep_source_where}
    ? CPAN::FTP->localize($index, catfile($self->{keep_source_where}, $index))
    : catfile($self->{CPAN}, $index);
  return unless check_file('modules/02packages.details.txt.gz', $packages);
  print_debug("Reading information from $packages\n");
  my $lines = zcat($packages);
  while (@$lines) {
    my $shift = shift(@$lines);
    last if $shift =~ /^\s*$/;
  }
  my ($mods, $cpan_files);
  foreach (@$lines) {
    my ($mod_name, $mod_vers, $dist_file) = split(" ", $_, 4);
    $mod_vers = undef if $mod_vers eq 'undef';
    $mods->{$mod_name} = { mod_vers => $mod_vers, dist_file => $dist_file };
    $cpan_files->{$dist_file}++;
  }
  return ($mods, $cpan_files);
}

sub mailrc {
  my $self  = shift;
  my $index = 'authors/01mailrc.txt.gz';
  my $mailrc =
    $self->{keep_source_where}
    ? CPAN::FTP->localize($index, catfile($self->{keep_source_where}, $index))
    : catfile($self->{CPAN}, $index);
  return unless check_file('authors/01mailrc.txt.gz', $mailrc);
  print_debug("Reading information from $mailrc\n");
  my $lines = zcat($mailrc);
  my $auths;
  foreach (@$lines) {

    #my($cpanid,$fullname,$email) =
    #m/alias\s+(\S+)\s+\"([^\"\<]+)\s+\<([^\>]+)\>\"/;
    my ($cpanid, $authinfo) = m/alias\s+(\S+)\s+\"([^\"]+)\"/;
    next unless $cpanid;
    my ($fullname, $email);
    if ($authinfo =~ m/([^<]+)\<(.*)\>/) {
      $fullname = $1;
      $email    = $2;
    } else {
      $fullname = '';
      $email    = lc($cpanid) . '@cpan.org';
    }
    $auths->{$cpanid} = {
      fullname => trim($fullname),
      email    => trim($email) };
  }
  return $self->{auths} = $auths;
}

sub check_file {
  my ($index, $file) = @_;
  unless ($file) {
    warn qq{index file '$index' not defined};
    return;
  }
  unless (-f $file) {
    warn qq{index file '$file' not found};
    return;
  }
  return 1;
}

sub zcat {
  my $file = shift;
  my ($buffer, $lines);
  my $gz = gzopen($file, 'rb')
    or die "Cannot open $file: $gzerrno";
  while ($gz->gzreadline($buffer) > 0) {
    push @$lines, $buffer;
  }
  die "Error reading from $file: $gzerrno" . ($gzerrno + 0)
    if $gzerrno != Z_STREAM_END;
  $gz->gzclose();
  return $lines;
}

sub trim {
  my $string = shift;
  return '' unless $string;
  $string =~ s/^\s+//;
  $string =~ s/\s+$//;
  $string =~ s/\s+/ /g;
  return $string;
}

1;

=head1 NAME

CPAN::SQLite::Info - extract information from CPAN indices

=head1 VERSION

version 0.220

=head1 DESCRIPTION

This module extracts information from the CPAN indices
F<$CPAN/modules/02packages.details.txt.gz> and
F<$CPAN/authors/01mailrc.txt.gz>.

A C<CPAN::SQLite::Info> object is created with

    my $info = CPAN::SQLite::Info->new(CPAN => $cpan);

where C<$cpan> specifies the top-level CPAN directory
underneath which the index files are found. Calling

    $info->fetch_info();

will result in the object being populated with 3 hash references:

=over 3

=item * C<$info-E<gt>{dists}>

This contains information on distributions. Keys of this hash
reference are the distribution names, with the associated value being a
hash reference with keys of

=over 3

=item C<dist_vers> - the version of the CPAN file

=item C<dist_file> - the CPAN filename

=item C<cpanid> - the CPAN author id

=item C<dist_abs> - a description, if available

=item C<modules> - specifies the modules present in the distribution:

  for my $module (keys %{$info->{$distname}->{modules}}) {
    print "Module: $module\n";
  }

=back

=item * C<$info-E<gt>{mods}>

This contains information on modules. Keys of this hash
reference are the module names, with the associated values being a
hash reference with keys of

=over 3

=item C<dist_name> - the distribution name containing the module

=item C<mod_vers> - the version

=item C<mod_abs> - a description, if available

=back

=item * C<$info-E<gt>{auths}>

This contains information on CPAN authors. Keys of this hash
reference are the CPAN ids, with the associated value being a
hash reference with keys of

=over 3

=item C<fullname> - the author's full name

=item C<email> - the author's email address

=back

=back

=head1 SEE ALSO

L<CPAN::SQLite::Index>

=cut
