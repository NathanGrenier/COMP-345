package Net::DNS::RR::KEY;

use strict;
use warnings;
our $VERSION = (qw$Id: KEY.pm 1972 2024-04-21 08:13:19Z willem $)[2];

use base qw(Net::DNS::RR::DNSKEY);


=head1 NAME

Net::DNS::RR::KEY - DNS KEY resource record

=cut


sub _defaults {				## specify RR attribute default values
	my $self = shift;

	$self->algorithm(1);
	$self->flags(0);
	$self->protocol(3);
	return;
}


1;
__END__


=head1 SYNOPSIS

    use Net::DNS;
    $rr = Net::DNS::RR->new('name KEY flags protocol algorithm publickey');

=head1 DESCRIPTION

DNS KEY resource record

This is a clone of the DNSKEY record and inherits all properties of
the Net::DNS::RR::DNSKEY class.

Please see the L<Net::DNS::RR::DNSKEY> documentation for details.

=head1 METHODS

The available methods are those inherited from the base class augmented
by the type-specific methods defined in this package.

Use of undocumented package features or direct access to internal data
structures is discouraged and could result in program termination or
other unpredictable behaviour.



=head1 COPYRIGHT

Copyright (c)2005 Olaf Kolkman, NLnet Labs.

All rights reserved.

Package template (c)2009,2012 O.M.Kolkman and R.W.Franks.


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

L<perl> L<Net::DNS> L<Net::DNS::RR>
L<Net::DNS::RR::DNSKEY>
L<RFC2535(3)|https://iana.org/go/rfc2535#section-3>
L<RFC2936|https://iana.org/go/rfc2936>
L<RFC2931|https://iana.org/go/rfc2931>
L<RFC3110|https://iana.org/go/rfc3110>
L<RFC4034|https://iana.org/go/rfc4034>

=cut
