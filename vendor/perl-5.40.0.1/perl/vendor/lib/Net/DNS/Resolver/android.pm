package Net::DNS::Resolver::android;

use strict;
use warnings;
our $VERSION = (qw$Id: android.pm 1972 2024-04-21 08:13:19Z willem $)[2];


=head1 NAME

Net::DNS::Resolver::android - Android resolver class

=cut


use base qw(Net::DNS::Resolver::Base);


my $config_file = 'resolv.conf';
my @config_path = ( $ENV{ANDROID_ROOT} || '/system' );
my @config_file = grep { -f $_ && -r _ } map {"$_/etc/$config_file"} @config_path;

my $homedir = $ENV{HOME};
my $dotfile = '.resolv.conf';
my @dotfile = grep { -f $_ && -o _ } map {"$_/$dotfile"} grep {$_} $homedir, '.';


sub _init {
	my $defaults = shift->_defaults;

	my @nameserver;
	for ( 1 .. 4 ) {
		my $ret = `getprop net.dns$_` || next;
		chomp $ret;
		push @nameserver, $ret || next;
	}

	$defaults->nameserver(@nameserver) if @nameserver;


	$defaults->_read_config_file($_) foreach @config_file;

	%$defaults = Net::DNS::Resolver::Base::_untaint(%$defaults);

	$defaults->_read_config_file($_) foreach @dotfile;

	$defaults->_read_env;
	return;
}


1;
__END__


=head1 SYNOPSIS

    use Net::DNS::Resolver;

=head1 DESCRIPTION

This class implements the OS specific portions of C<Net::DNS::Resolver>.

No user serviceable parts inside, see L<Net::DNS::Resolver>
for all your resolving needs.

=head1 COPYRIGHT

Copyright (c)2014 Dick Franks.

All rights reserved.

=head1 LICENSE

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted, provided
that the original copyright notices appear in all copies and that both
copyright notice and this permission notice appear in supporting
documentation, and that the name of the author not be used in advertising
or publicity pertaining to distribution of the software without specific
prior written permission.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

=head1 SEE ALSO

L<perl>, L<Net::DNS>, L<Net::DNS::Resolver>

=cut

