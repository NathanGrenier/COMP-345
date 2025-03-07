package Net::DNS::Packet;

use strict;
use warnings;

our $VERSION = (qw$Id: Packet.pm 1959 2024-01-17 08:55:01Z willem $)[2];


=head1 NAME

Net::DNS::Packet - DNS protocol packet

=head1 SYNOPSIS

    use Net::DNS::Packet;

    $query = Net::DNS::Packet->new( 'example.com', 'MX', 'IN' );

    $reply = $resolver->send( $query );


=head1 DESCRIPTION

A Net::DNS::Packet object represents a DNS protocol packet.

=cut


use integer;
use Carp;

use Net::DNS::Parameters qw(:dsotype);
use constant UDPSZ => 512;

BEGIN {
	require Net::DNS::Header;
	require Net::DNS::Question;
	require Net::DNS::RR;
}


=head1 METHODS

=head2 new

    $packet = Net::DNS::Packet->new( 'example.com' );
    $packet = Net::DNS::Packet->new( 'example.com', 'MX', 'IN' );

    $packet = Net::DNS::Packet->new();

If passed a domain, type, and class, new() creates a Net::DNS::Packet
object which is suitable for making a DNS query for the specified
information.  The type and class may be omitted; they default to A
and IN.

If called with an empty argument list, new() creates an empty packet.

=cut

sub new {
	my ( $class, @arg ) = @_;
	return &decode if ref $arg[0];

	my $self = bless {
		status	   => 0,
		question   => [],
		answer	   => [],
		authority  => [],
		additional => [],
		}, $class;

	$self->{question} = [Net::DNS::Question->new(@arg)] if scalar @arg;

	return $self;
}


#=head2 decode

=pod

    $packet = Net::DNS::Packet->decode( \$data );
    $packet = Net::DNS::Packet->decode( \$data, 1 );	# debug
    $packet = Net::DNS::Packet->new( \$data ... );

If passed a reference to a scalar containing DNS packet data, a new
packet object is created by decoding the data.
The optional second boolean argument enables debugging output.

Returns undef if unable to create a packet object.

Decoding errors, including data corruption and truncation, are
collected in the $@ ($EVAL_ERROR) variable.


    ( $packet, $length ) = Net::DNS::Packet->decode( \$data );

If called in array context, returns a packet object and the number
of octets successfully decoded.

Note that the number of RRs in each section of the packet may differ
from the corresponding header value if the data has been truncated
or corrupted during transmission.

=cut

use constant HEADER_LENGTH => length pack 'n6', (0) x 6;

sub decode {
	my $class = shift;					# uncoverable pod
	my $data  = shift;
	my $debug = shift || 0;

	my $offset = 0;
	my $self;
	eval {
		local $SIG{__DIE__};
		my $length = length $$data;
		die 'corrupt wire-format data' if $length < HEADER_LENGTH;

		# header section
		my ( $id, $status, @count ) = unpack 'n6', $$data;
		my ( $qd, $an, $ns, $ar ) = @count;

		$self = bless {
			id	   => $id,
			status	   => $status,
			count	   => [@count],
			question   => [],
			answer	   => [],
			authority  => [],
			additional => [],
			replysize  => $length
			}, $class;

		# question/zone section
		my $hash = {};
		my $record;
		$offset = HEADER_LENGTH;
		while ( $qd-- ) {
			( $record, $offset ) = Net::DNS::Question->decode( $data, $offset, $hash );
			CORE::push( @{$self->{question}}, $record );
		}

		# RR sections
		while ( $an-- ) {
			( $record, $offset ) = Net::DNS::RR->decode( $data, $offset, $hash );
			CORE::push( @{$self->{answer}}, $record );
		}

		while ( $ns-- ) {
			( $record, $offset ) = Net::DNS::RR->decode( $data, $offset, $hash );
			CORE::push( @{$self->{authority}}, $record );
		}

		while ( $ar-- ) {
			( $record, $offset ) = Net::DNS::RR->decode( $data, $offset, $hash );
			CORE::push( @{$self->{additional}}, $record );
		}

		return unless $offset == HEADER_LENGTH;
		return unless $self->header->opcode eq 'DSO';

		$self->{dso} = [];
		my $limit = $length - 4;
		while ( $offset < $limit ) {
			my ( $t, $l, $v ) = unpack "\@$offset n2a*", $$data;
			CORE::push( @{$self->{dso}}, [$t, substr( $v, 0, $l )] );
			$offset += ( $l + 4 );
		}
	};

	if ($debug) {
		local $@ = $@;
		print $@ if $@;
		eval { $self->print };
	}

	return wantarray ? ( $self, $offset ) : $self;
}


=head2 data

    $data = $packet->data;
    $data = $packet->data( $size );

Returns the packet data in binary format, suitable for sending as a
query or update request to a nameserver.

Truncation may be specified using a non-zero optional size argument.

=cut

sub data {
	return &encode;
}

sub encode {
	my ( $self, $size ) = @_;				# uncoverable pod

	my $edns = $self->edns;					# EDNS support
	my @addl = grep { !$_->isa('Net::DNS::RR::OPT') } @{$self->{additional}};
	$self->{additional} = [$edns, @addl] if $edns->_specified;

	return $self->truncate($size) if $size;

	my @part = qw(question answer authority additional);
	my @size = map { scalar @{$self->{$_}} } @part;
	my $data = pack 'n6', $self->header->id, $self->{status}, @size;
	$self->{count} = [];

	my $hash = {};						# packet body
	foreach my $component ( map { @{$self->{$_}} } @part ) {
		$data .= $component->encode( length $data, $hash, $self );
	}

	return $data;
}


=head2 header

    $header = $packet->header;

Constructor method which returns a Net::DNS::Header object which
represents the header section of the packet.

=cut

sub header {
	my $self = shift;
	return bless \$self, q(Net::DNS::Header);
}


=head2 edns

    $edns    = $packet->edns;
    $version = $edns->version;
    $UDPsize = $edns->size;

Auxiliary function which provides access to the EDNS protocol
extension OPT RR.

=cut

sub edns {
	my $self = shift;
	my $link = \$self->{xedns};
	($$link) = grep { $_->isa(qw(Net::DNS::RR::OPT)) } @{$self->{additional}} unless $$link;
	$$link = Net::DNS::RR->new( type => 'OPT' ) unless $$link;
	return $$link;
}


=head2 reply

    $reply = $query->reply( $UDPmax );

Constructor method which returns a new reply packet.

The optional UDPsize argument is the maximum UDP packet size which
can be reassembled by the local network stack, and is advertised in
response to an EDNS query.

=cut

sub reply {
	my ( $query, @UDPmax ) = @_;
	my $qheadr = $query->header;
	croak 'erroneous qr flag in query packet' if $qheadr->qr;

	my $reply  = Net::DNS::Packet->new();
	my $header = $reply->header;
	$header->qr(1);						# reply with same id, opcode and question
	$header->id( $qheadr->id );
	$header->opcode( $qheadr->opcode );
	my @question = $query->question;
	$reply->{question} = [@question];

	$header->rcode('FORMERR');				# no RCODE considered sinful!

	$header->rd( $qheadr->rd );				# copy these flags into reply
	$header->cd( $qheadr->cd );

	return $reply unless grep { $_->isa('Net::DNS::RR::OPT') } @{$query->{additional}};

	my $edns = $reply->edns();
	CORE::push( @{$reply->{additional}}, $edns );
	$edns->udpsize(@UDPmax);
	return $reply;
}


=head2 question, zone

    @question = $packet->question;

Returns a list of Net::DNS::Question objects representing the
question section of the packet.

In dynamic update packets, this section is known as zone() and
specifies the DNS zone to be updated.

=cut

sub question {
	my @qr = @{shift->{question}};
	return @qr;
}

sub zone { return &question }


=head2 answer, pre, prerequisite

    @answer = $packet->answer;

Returns a list of Net::DNS::RR objects representing the answer
section of the packet.

In dynamic update packets, this section is known as pre() or
prerequisite() and specifies the RRs or RRsets which must or must
not preexist.

=cut

sub answer {
	my @rr = @{shift->{answer}};
	return @rr;
}

sub pre		 { return &answer }
sub prerequisite { return &answer }


=head2 authority, update

    @authority = $packet->authority;

Returns a list of Net::DNS::RR objects representing the authority
section of the packet.

In dynamic update packets, this section is known as update() and
specifies the RRs or RRsets to be added or deleted.

=cut

sub authority {
	my @rr = @{shift->{authority}};
	return @rr;
}

sub update { return &authority }


=head2 additional

    @additional = $packet->additional;

Returns a list of Net::DNS::RR objects representing the additional
section of the packet.

=cut

sub additional {
	my @rr = @{shift->{additional}};
	return @rr;
}


=head2 print

    $packet->print;

Prints the entire packet to the currently selected output filehandle
using the master file format mandated by RFC1035.

=cut

sub print {
	print &string;
	return;
}


=head2 string

    print $packet->string;

Returns a string representation of the packet.

=cut

sub string {
	my $self = shift;

	my $header = $self->header;
	my $opcode = $header->opcode;
	my $packet = $header->qr ? 'Response' : 'Query';
	my $server = $self->{replyfrom};
	my $length = $self->{replysize};
	my $origin = $server ? ";; $packet received from [$server] $length octets\n" : "";
	my @record = ( "$origin;; HEADER SECTION", $header->string );

	if ( $opcode eq 'DSO' ) {
		CORE::push( @record, ";; DSO SECTION" );
		foreach ( @{$self->{dso}} ) {
			my ( $t, $v ) = @$_;
			CORE::push( @record, sprintf( ";;\t%s\t%s", dsotypebyval($t), unpack( 'H*', $v ) ) );
		}
		return join "\n", @record, "\n";
	}

	my $edns = $self->edns;
	CORE::push( @record, $edns->string ) if $edns->_specified;

	my @section  = $opcode eq 'UPDATE' ? qw(ZONE PREREQUISITE UPDATE) : qw(QUESTION ANSWER AUTHORITY);
	my @question = $self->question;
	my $qdcount  = scalar @question;
	my $qds	     = $qdcount != 1 ? 's' : '';
	CORE::push( @record, ";; $section[0] SECTION ($qdcount record$qds)", map { ';; ' . $_->string } @question );

	my @answer  = $self->answer;
	my $ancount = scalar @answer;
	my $ans	    = $ancount != 1 ? 's' : '';
	CORE::push( @record, "\n;; $section[1] SECTION ($ancount record$ans)", map { $_->string } @answer );

	my @authority = $self->authority;
	my $nscount   = scalar @authority;
	my $nss	      = $nscount != 1 ? 's' : '';
	CORE::push( @record, "\n;; $section[2] SECTION ($nscount record$nss)", map { $_->string } @authority );

	my @additional = $self->additional;
	my $arcount    = scalar @additional;
	my $ars	       = $arcount != 1 ? 's' : '';
	my $EDNSmarker = join ' ', qq[;; {\t"EDNS-VERSION":], $edns->version, qq[}];
	CORE::push( @record, "\n;; ADDITIONAL SECTION ($arcount record$ars)" );
	CORE::push( @record, map { ( $_ eq $edns ) ? $EDNSmarker : $_->string } @additional );

	return join "\n", @record, "\n";
}


=head2 from

    print "packet received from ", $packet->from, "\n";

Returns the IP address from which this packet was received.
This method will return undef for user-created packets.

=cut

sub from {
	my ( $self, @argument ) = @_;
	for (@argument) { $self->{replyfrom} = $_ }
	return $self->{replyfrom};
}

sub answerfrom { return &from; }				# uncoverable pod


=head2 size

    print "packet size: ", $packet->size, " octets\n";

Returns the size of the packet in octets as it was received from a
nameserver.  This method will return undef for user-created packets
(use length($packet->data) instead).

=cut

sub size {
	return shift->{replysize};
}

sub answersize { return &size; }				# uncoverable pod


=head2 push

    $ancount = $packet->push( prereq => $rr );
    $nscount = $packet->push( update => $rr );
    $arcount = $packet->push( additional => $rr );

    $nscount = $packet->push( update => $rr1, $rr2, $rr3 );
    $nscount = $packet->push( update => @rr );

Adds RRs to the specified section of the packet.

Returns the number of resource records in the specified section.

Section names may be abbreviated to the first three characters.

=cut

sub push {
	my ( $self, $section, @rr ) = @_;
	my $list = $self->_section($section);
	return CORE::push( @$list, @rr );
}


=head2 unique_push

    $ancount = $packet->unique_push( prereq => $rr );
    $nscount = $packet->unique_push( update => $rr );
    $arcount = $packet->unique_push( additional => $rr );

    $nscount = $packet->unique_push( update => $rr1, $rr2, $rr3 );
    $nscount = $packet->unique_push( update => @rr );

Adds RRs to the specified section of the packet provided that the
RRs are not already present in the same section.

Returns the number of resource records in the specified section.

Section names may be abbreviated to the first three characters.

=cut

sub unique_push {
	my ( $self, $section, @rr ) = @_;
	my $list = $self->_section($section);

	my %unique = map { ( bless( {%$_, ttl => 0}, ref $_ )->canonical => $_ ) } @rr, @$list;
	return scalar( @$list = values %unique );
}


=head2 pop

    my $rr = $packet->pop( 'pre' );
    my $rr = $packet->pop( 'update' );
    my $rr = $packet->pop( 'additional' );

Removes a single RR from the specified section of the packet.

=cut

sub pop {
	my $self = shift;
	my $list = $self->_section(shift);
	return CORE::pop(@$list);
}


my %_section = (			## section name abbreviation table
	'ans' => 'answer',
	'pre' => 'answer',
	'aut' => 'authority',
	'upd' => 'authority',
	'add' => 'additional'
	);

sub _section {				## returns array reference for section
	my $self = shift;
	my $name = shift;
	my $list = $_section{unpack 'a3', $name} || $name;
	return $self->{$list} ||= [];
}


=head2 sign_tsig

    $query = Net::DNS::Packet->new( 'www.example.com', 'A' );

    $query->sign_tsig(
		$keyfile,
		fudge => 60
		);

    $reply = $res->send( $query );

    $reply->verify( $query ) || die $reply->verifyerr;

Attaches a TSIG resource record object, which will be used to sign
the packet (see RFC 2845).

The TSIG record can be customised by optional additional arguments to
sign_tsig() or by calling the appropriate Net::DNS::RR::TSIG methods.

If you wish to create a TSIG record using a non-standard algorithm,
you will have to create it yourself.  In all cases, the TSIG name
must uniquely identify the key shared between the parties, and the
algorithm name must identify the signing function to be used with the
specified key.

    $tsig = Net::DNS::RR->new(
		name		=> 'tsig.example',
		type		=> 'TSIG',
		algorithm	=> 'custom-algorithm',
		key		=> '<base64 key text>',
		sig_function	=> sub {
					  my ($key, $data) = @_;
						...
					}
		);

    $query->sign_tsig( $tsig );


The response to an inbound request is signed by presenting the request
in place of the key parameter.

    $response = $request->reply;
    $response->sign_tsig( $request, @options );


Multi-packet transactions are signed by chaining the sign_tsig()
calls together as follows:

    $opaque  =	$packet1->sign_tsig( 'Kexample.+165+13281.private' );
    $opaque  =	$packet2->sign_tsig( $opaque );
		$packet3->sign_tsig( $opaque );

The opaque intermediate object references returned during multi-packet
signing are not intended to be accessed by the end-user application.
Any such access is expressly forbidden.

Note that a TSIG record is added to every packet; this implementation
does not support the suppressed signature scheme described in RFC2845.

=cut

sub sign_tsig {
	my ( $self, @argument ) = @_;
	return eval {
		local $SIG{__DIE__};
		require Net::DNS::RR::TSIG;
		my $tsig = Net::DNS::RR::TSIG->create(@argument);
		$self->push( 'additional' => $tsig );
		return $tsig;
	} || return croak "$@\nTSIG: unable to sign packet";
}


=head2 verify and verifyerr

    $packet->verify()		|| die $packet->verifyerr;
    $reply->verify( $query )	|| die $reply->verifyerr;

Verify TSIG signature of packet or reply to the corresponding query.


    $opaque  =	$packet1->verify( $query ) || die $packet1->verifyerr;
    $opaque  =	$packet2->verify( $opaque );
    $verifed =	$packet3->verify( $opaque ) || die $packet3->verifyerr;

The opaque intermediate object references returned during multi-packet
verify() will be undefined (Boolean false) if verification fails.
Access to the object itself, if it exists, is expressly forbidden.
Testing at every stage may be omitted, which results in a BADSIG error
on the final packet in the absence of more specific information.

=cut

sub verify {
	my ( $self, @argument ) = @_;
	my $sig = $self->sigrr;
	return $sig ? $sig->verify( $self, @argument ) : shift @argument;
}

sub verifyerr {
	my $sig = shift->sigrr;
	return $sig ? $sig->vrfyerrstr : 'not signed';
}


=head2 sign_sig0

SIG0 support is provided through the Net::DNS::RR::SIG class.
The requisite cryptographic components are not integrated into
Net::DNS but reside in the Net::DNS::SEC distribution available
from CPAN.

    $update = Net::DNS::Update->new('example.com');
    $update->push( update => rr_add('foo.example.com A 10.1.2.3'));
    $update->sign_sig0('Kexample.com+003+25317.private');

Execution will be terminated if Net::DNS::SEC is not available.


=head2 verify SIG0

    $packet->verify( $keyrr )		|| die $packet->verifyerr;
    $packet->verify( [$keyrr, ...] )	|| die $packet->verifyerr;

Verify SIG0 packet signature against one or more specified KEY RRs.

=cut

sub sign_sig0 {
	my $self = shift;
	my $karg = shift;

	return eval {
		local $SIG{__DIE__};

		my $sig0;
		if ( ref($karg) eq 'Net::DNS::RR::SIG' ) {
			$sig0 = $karg;

		} else {
			require Net::DNS::RR::SIG;
			$sig0 = Net::DNS::RR::SIG->create( '', $karg );
		}

		$self->push( 'additional' => $sig0 );
		return $sig0;
	} || return croak "$@\nSIG0: unable to sign packet";
}


=head2 sigrr

    $sigrr = $packet->sigrr() || die 'unsigned packet';

The sigrr method returns the signature RR from a signed packet
or undefined if the signature is absent.

=cut

sub sigrr {
	my $self = shift;

	my ($sig) = reverse $self->additional;
	return unless $sig;
	for ( $sig->type ) {
		return $sig if /TSIG|SIG/;
	}
	return;
}


########################################

=head2 truncate

The truncate method takes a maximum length as argument and then tries
to truncate the packet and set the TC bit according to the rules of
RFC2181 Section 9.

The smallest length limit that is honoured is 512 octets.

=cut

# From RFC2181:
#
# 9. The TC (truncated) header bit
#
#   The TC bit should be set in responses only when an RRSet is required
#   as a part of the response, but could not be included in its entirety.
#   The TC bit should not be set merely because some extra information
#   could have been included, for which there was insufficient room. This
#   includes the results of additional section processing.  In such cases
#   the entire RRSet that will not fit in the response should be omitted,
#   and the reply sent as is, with the TC bit clear.  If the recipient of
#   the reply needs the omitted data, it can construct a query for that
#   data and send that separately.
#
#   Where TC is set, the partial RRSet that would not completely fit may
#   be left in the response.  When a DNS client receives a reply with TC
#   set, it should ignore that response, and query again, using a
#   mechanism, such as a TCP connection, that will permit larger replies.

# Code developed from a contribution by Aaron Crane via rt.cpan.org 33547

sub truncate {
	my $self = shift;
	my $size = shift || UDPSZ;

	my $sigrr = $self->sigrr;
	$size = UDPSZ unless $size > UDPSZ;
	$size -= $sigrr->_size if $sigrr;

	my $data = pack 'x' x HEADER_LENGTH;			# header placeholder
	$self->{count} = [];

	my $tc;
	my $hash = {};
	foreach my $section ( map { $self->{$_} } qw(question answer authority) ) {
		my @list;
		foreach my $item (@$section) {
			my $component = $item->encode( length $data, $hash );
			last if length($data) + length($component) > $size;
			last if $tc;
			$data .= $component;
			CORE::push @list, $item;
		}
		$tc++ if scalar(@list) < scalar(@$section);
		@$section = @list;
	}
	$self->header->tc(1) if $tc;				# only set if truncated here

	my %rrset;
	my @order;
	foreach my $item ( grep { ref($_) ne ref($sigrr) } $self->additional ) {
		my $name  = $item->{owner}->canonical;
		my $class = $item->{class} || 0;
		my $key	  = pack 'nna*', $class, $item->{type}, $name;
		CORE::push @order, $key unless $rrset{$key};
		CORE::push @{$rrset{$key}}, $item;
	}

	my @list;
	foreach my $key (@order) {
		my $component = '';
		my @item      = @{$rrset{$key}};
		foreach my $item (@item) {
			$component .= $item->encode( length $data, $hash );
		}
		last if length($data) + length($component) > $size;
		$data .= $component;
		CORE::push @list, @item;
	}

	if ($sigrr) {
		$data .= $sigrr->encode( length $data, $hash, $self );
		CORE::push @list, $sigrr;
	}
	$self->{'additional'} = \@list;

	my @part = qw(question answer authority additional);
	my @size = map { scalar @{$self->{$_}} } @part;
	return pack 'n6 a*', $self->header->id, $self->{status}, @size, substr( $data, HEADER_LENGTH );
}


########################################

sub dump {				## print internal data structure
	my @data = @_;						# uncoverable pod
	require Data::Dumper;
	local $Data::Dumper::Maxdepth = $Data::Dumper::Maxdepth || 3;
	local $Data::Dumper::Sortkeys = $Data::Dumper::Sortkeys || 1;
	local $Data::Dumper::Useqq    = $Data::Dumper::Useqq	|| 1;
	print Data::Dumper::Dumper(@data);
	return;
}


1;
__END__


=head1 COPYRIGHT

Copyright (c)1997-2000 Michael Fuhr.

Portions Copyright (c)2002-2004 Chris Reinhardt.

Portions Copyright (c)2002-2009 Olaf Kolkman

Portions Copyright (c)2007-2019 Dick Franks

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

L<perl> L<Net::DNS> L<Net::DNS::Update> L<Net::DNS::Header>
L<Net::DNS::Question> L<Net::DNS::RR> L<Net::DNS::RR::TSIG>
L<RFC1035(4.1)|https://tools.ietf.org/html/rfc1035>
L<RFC2136(2)|https://tools.ietf.org/html/rfc2136>
L<RFC8945|https://tools.ietf.org/html/rfc8945>

=cut

