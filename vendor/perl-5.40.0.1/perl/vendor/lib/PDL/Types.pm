#line 382 "Basic/Core/Types.pm.PL"

### Generated from Types.pm.PL automatically - do not modify! ###

package PDL::Types;
use strict;
use warnings;
require Exporter;
use Carp;


our @EXPORT = qw( $PDL_SB $PDL_B $PDL_S $PDL_US $PDL_L $PDL_UL $PDL_IND $PDL_ULL $PDL_LL $PDL_F $PDL_D $PDL_LD $PDL_CF $PDL_CD $PDL_CLD
	       @pack %typehash );
#line 400 "Basic/Core/Types.pm.PL"

our @EXPORT_OK = (@EXPORT,
  qw/types typesrtkeys mapfld typefld
    ppdefs ppdefs_complex ppdefs_all
  /
);
our %EXPORT_TAGS = (
	All=>[@EXPORT,@EXPORT_OK],
);

our @ISA    = qw( Exporter );

#line 416 "Basic/Core/Types.pm.PL"


# Data types/sizes (bytes) [must be in order of complexity]
# Enum
our ( $PDL_SB, $PDL_B, $PDL_S, $PDL_US, $PDL_L, $PDL_UL, $PDL_IND, $PDL_ULL, $PDL_LL, $PDL_F, $PDL_D, $PDL_LD, $PDL_CF, $PDL_CD, $PDL_CLD ) = (0..14);
# Corresponding pack types
our @pack= qw/c* C* s* S* l* L* q* Q* q* f* d* D* (ff)* (dd)* (DD)*/;
our @names= qw/PDL_SB PDL_B PDL_S PDL_US PDL_L PDL_UL PDL_IND PDL_ULL PDL_LL PDL_F PDL_D PDL_LD PDL_CF PDL_CD PDL_CLD/;

our %typehash = (
	PDL_SB =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'sbyte',
		  'ctype' => 'PDL_SByte',
		  'defbval' => 'SCHAR_MIN',
		  'floatsuffix' => undef,
		  'identifier' => 'SB',
		  'integer' => 1,
		  'ioname' => 'sbyte',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 0,
		  'ppforcetype' => 'sbyte',
		  'ppsym' => 'A',
		  'real' => 1,
		  'realctype' => 'signed char',
		  'realversion' => 'A',
		  'shortctype' => 'SByte',
		  'sym' => 'PDL_SB',
		  'unsigned' => 0,
		  'usenan' => 0
		}
		,
	PDL_B =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'byte',
		  'ctype' => 'PDL_Byte',
		  'defbval' => 'UCHAR_MAX',
		  'floatsuffix' => undef,
		  'identifier' => 'B',
		  'integer' => 1,
		  'ioname' => 'byte',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 1,
		  'ppforcetype' => 'byte',
		  'ppsym' => 'B',
		  'real' => 1,
		  'realctype' => 'unsigned char',
		  'realversion' => 'B',
		  'shortctype' => 'Byte',
		  'sym' => 'PDL_B',
		  'unsigned' => 1,
		  'usenan' => 0
		}
		,
	PDL_S =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'short',
		  'ctype' => 'PDL_Short',
		  'defbval' => 'SHRT_MIN',
		  'floatsuffix' => undef,
		  'identifier' => 'S',
		  'integer' => 1,
		  'ioname' => 'short',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 2,
		  'ppforcetype' => 'short',
		  'ppsym' => 'S',
		  'real' => 1,
		  'realctype' => 'short',
		  'realversion' => 'S',
		  'shortctype' => 'Short',
		  'sym' => 'PDL_S',
		  'unsigned' => 0,
		  'usenan' => 0
		}
		,
	PDL_US =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'ushort',
		  'ctype' => 'PDL_Ushort',
		  'defbval' => 'USHRT_MAX',
		  'floatsuffix' => undef,
		  'identifier' => 'US',
		  'integer' => 1,
		  'ioname' => 'ushort',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 3,
		  'ppforcetype' => 'ushort',
		  'ppsym' => 'U',
		  'real' => 1,
		  'realctype' => 'unsigned short',
		  'realversion' => 'U',
		  'shortctype' => 'Ushort',
		  'sym' => 'PDL_US',
		  'unsigned' => 1,
		  'usenan' => 0
		}
		,
	PDL_L =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'long',
		  'ctype' => 'PDL_Long',
		  'defbval' => 'INT32_MIN',
		  'floatsuffix' => undef,
		  'identifier' => 'L',
		  'integer' => 1,
		  'ioname' => 'long',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 4,
		  'ppforcetype' => 'int',
		  'ppsym' => 'L',
		  'real' => 1,
		  'realctype' => 'int32_t',
		  'realversion' => 'L',
		  'shortctype' => 'Long',
		  'sym' => 'PDL_L',
		  'unsigned' => 0,
		  'usenan' => 0
		}
		,
	PDL_UL =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'ulong',
		  'ctype' => 'PDL_ULong',
		  'defbval' => 'UINT32_MAX',
		  'floatsuffix' => undef,
		  'identifier' => 'UL',
		  'integer' => 1,
		  'ioname' => 'ulong',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 5,
		  'ppforcetype' => 'uint',
		  'ppsym' => 'K',
		  'real' => 1,
		  'realctype' => 'uint32_t',
		  'realversion' => 'K',
		  'shortctype' => 'ULong',
		  'sym' => 'PDL_UL',
		  'unsigned' => 1,
		  'usenan' => 0
		}
		,
	PDL_IND =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'indx',
		  'ctype' => 'PDL_Indx',
		  'defbval' => 'PTRDIFF_MIN',
		  'floatsuffix' => undef,
		  'identifier' => 'IND',
		  'integer' => 1,
		  'ioname' => 'indx',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 6,
		  'ppforcetype' => 'indx',
		  'ppsym' => 'N',
		  'real' => 1,
		  'realctype' => 'ptrdiff_t',
		  'realversion' => 'N',
		  'shortctype' => 'Indx',
		  'sym' => 'PDL_IND',
		  'unsigned' => 0,
		  'usenan' => 0
		}
		,
	PDL_ULL =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'ulonglong',
		  'ctype' => 'PDL_ULongLong',
		  'defbval' => 'UINT64_MAX',
		  'floatsuffix' => undef,
		  'identifier' => 'ULL',
		  'integer' => 1,
		  'ioname' => 'ulonglong',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 7,
		  'ppforcetype' => 'ulonglong',
		  'ppsym' => 'P',
		  'real' => 1,
		  'realctype' => 'uint64_t',
		  'realversion' => 'P',
		  'shortctype' => 'ULongLong',
		  'sym' => 'PDL_ULL',
		  'unsigned' => 1,
		  'usenan' => 0
		}
		,
	PDL_LL =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'longlong',
		  'ctype' => 'PDL_LongLong',
		  'defbval' => 'INT64_MIN',
		  'floatsuffix' => undef,
		  'identifier' => 'LL',
		  'integer' => 1,
		  'ioname' => 'longlong',
		  'isfinite' => undef,
		  'isnan' => undef,
		  'numval' => 8,
		  'ppforcetype' => 'longlong',
		  'ppsym' => 'Q',
		  'real' => 1,
		  'realctype' => 'int64_t',
		  'realversion' => 'Q',
		  'shortctype' => 'LongLong',
		  'sym' => 'PDL_LL',
		  'unsigned' => 0,
		  'usenan' => 0
		}
		,
	PDL_F =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'float',
		  'ctype' => 'PDL_Float',
		  'defbval' => '-FLT_MAX',
		  'floatsuffix' => 'f',
		  'identifier' => 'F',
		  'integer' => 0,
		  'ioname' => 'float',
		  'isfinite' => 'isfinite(%1$s)',
		  'isnan' => 'isnan(%1$s)',
		  'numval' => 9,
		  'ppforcetype' => 'float',
		  'ppsym' => 'F',
		  'real' => 1,
		  'realctype' => 'float',
		  'realversion' => 'F',
		  'shortctype' => 'Float',
		  'sym' => 'PDL_F',
		  'unsigned' => 0,
		  'usenan' => 1
		}
		,
	PDL_D =>
		{
		  'complexversion' => 'C',
		  'convertfunc' => 'double',
		  'ctype' => 'PDL_Double',
		  'defbval' => '-DBL_MAX',
		  'floatsuffix' => '',
		  'identifier' => 'D',
		  'integer' => 0,
		  'ioname' => 'double',
		  'isfinite' => 'isfinite(%1$s)',
		  'isnan' => 'isnan(%1$s)',
		  'numval' => 10,
		  'ppforcetype' => 'double',
		  'ppsym' => 'D',
		  'real' => 1,
		  'realctype' => 'double',
		  'realversion' => 'D',
		  'shortctype' => 'Double',
		  'sym' => 'PDL_D',
		  'unsigned' => 0,
		  'usenan' => 1
		}
		,
	PDL_LD =>
		{
		  'complexversion' => 'CLD',
		  'convertfunc' => 'ldouble',
		  'ctype' => 'PDL_LDouble',
		  'defbval' => '-LDBL_MAX',
		  'floatsuffix' => 'l',
		  'identifier' => 'LD',
		  'integer' => 0,
		  'ioname' => 'ldouble',
		  'isfinite' => 'isfinite(%1$s)',
		  'isnan' => 'isnan(%1$s)',
		  'numval' => 11,
		  'ppforcetype' => 'ldouble',
		  'ppsym' => 'E',
		  'real' => 1,
		  'realctype' => 'long double',
		  'realversion' => 'E',
		  'shortctype' => 'LDouble',
		  'sym' => 'PDL_LD',
		  'unsigned' => 0,
		  'usenan' => 1
		}
		,
	PDL_CF =>
		{
		  'complexversion' => 'G',
		  'convertfunc' => 'cfloat',
		  'ctype' => 'PDL_CFloat',
		  'defbval' => '(-FLT_MAX - I*FLT_MAX)',
		  'floatsuffix' => 'f',
		  'identifier' => 'CF',
		  'integer' => 0,
		  'ioname' => 'cfloat',
		  'isfinite' => '(isfinite(crealf(%1$s)) && isfinite(cimagf(%1$s)))',
		  'isnan' => '(isnan(crealf(%1$s)) || isnan(cimagf(%1$s)))',
		  'numval' => 12,
		  'ppforcetype' => 'cfloat',
		  'ppsym' => 'G',
		  'real' => 0,
		  'realctype' => 'complex float',
		  'realversion' => 'F',
		  'shortctype' => 'CFloat',
		  'sym' => 'PDL_CF',
		  'unsigned' => 0,
		  'usenan' => 1
		}
		,
	PDL_CD =>
		{
		  'complexversion' => 'C',
		  'convertfunc' => 'cdouble',
		  'ctype' => 'PDL_CDouble',
		  'defbval' => '(-DBL_MAX - I*DBL_MAX)',
		  'floatsuffix' => '',
		  'identifier' => 'CD',
		  'integer' => 0,
		  'ioname' => 'cdouble',
		  'isfinite' => '(isfinite(creal(%1$s)) && isfinite(cimag(%1$s)))',
		  'isnan' => '(isnan(creal(%1$s)) || isnan(cimag(%1$s)))',
		  'numval' => 13,
		  'ppforcetype' => 'cdouble',
		  'ppsym' => 'C',
		  'real' => 0,
		  'realctype' => 'complex double',
		  'realversion' => 'D',
		  'shortctype' => 'CDouble',
		  'sym' => 'PDL_CD',
		  'unsigned' => 0,
		  'usenan' => 1
		}
		,
	PDL_CLD =>
		{
		  'complexversion' => 'H',
		  'convertfunc' => 'cldouble',
		  'ctype' => 'PDL_CLDouble',
		  'defbval' => '(-LDBL_MAX - I*LDBL_MAX)',
		  'floatsuffix' => 'l',
		  'identifier' => 'CLD',
		  'integer' => 0,
		  'ioname' => 'cldouble',
		  'isfinite' => '(isfinite(creall(%1$s)) && isfinite(cimagl(%1$s)))',
		  'isnan' => '(isnan(creall(%1$s)) || isnan(cimagl(%1$s)))',
		  'numval' => 14,
		  'ppforcetype' => 'cldouble',
		  'ppsym' => 'H',
		  'real' => 0,
		  'realctype' => 'complex long double',
		  'realversion' => 'LD',
		  'shortctype' => 'CLDouble',
		  'sym' => 'PDL_CLD',
		  'unsigned' => 0,
		  'usenan' => 1
		}
		,
); # end typehash definition
#line 431 "Basic/Core/Types.pm.PL"

# Cross-reference by common names
my @HASHES = sort {$a->{numval} <=> $b->{numval}} values %typehash;
my @RTKEYS = map $_->{sym}, @HASHES;
our %typenames;
for my $h (@HASHES) {
  my $n = $h->{numval};
  $typenames{$_} = $n for $n, @$h{qw(sym ioname ctype ppforcetype ppsym identifier)};
}

=head1 NAME

PDL::Types - define fundamental PDL Datatypes

=head1 SYNOPSIS

 use PDL::Types;

 $pdl = ushort( 2.0, 3.0 );
 print "The actual c type used to store ushort's is '" .
    $pdl->type->realctype() . "'\n";
 The actual c type used to store ushort's is 'unsigned short'

=head1 DESCRIPTION

Internal module - holds all the PDL Type info.  The type info can be
accessed easily using the C<PDL::Type> object returned by
the L<type|PDL::Core/type> method as shown in the synopsis.

Skip to the end of this document to find out how to change
the set of types supported by PDL.

=head1 FUNCTIONS

A number of functions are available for module writers
to get/process type information. These are used in various
places (e.g. C<PDL::PP>, C<PDL::Core>) to generate the
appropriate type loops, etc.

=head2 typesrtkeys

=for ref

Returns an array of keys of typehash sorted in order of type complexity

=for example

 pdl> @typelist = PDL::Types::typesrtkeys;
 pdl> print @typelist;
 PDL_SB PDL_B PDL_S PDL_US PDL_L PDL_UL PDL_IND PDL_ULL PDL_LL PDL_F PDL_D PDL_LD PDL_CF PDL_CD PDL_CLD
=cut

sub typesrtkeys { @RTKEYS }

=head2 ppdefs

=for ref

Returns an array of pp symbols for all real types. This informs the
default C<GenericTypes> for C<pp_def> functions, making support for
complex types require an "opt-in".

=for example

 pdl> print PDL::Types::ppdefs
 A B S U L K N P Q F D E

=cut

my @PPDEFS = map $_->{ppsym}, grep $_->{real}, @HASHES;
sub ppdefs { @PPDEFS }

=head2 ppdefs_complex

=for ref

Returns an array of pp symbols for all complex types.

=for example

 pdl> print PDL::Types::ppdefs_complex
 G C H

=cut

my @PPDEFS_CPLX = map $_->{ppsym}, grep !$_->{real}, @HASHES;
sub ppdefs_complex { @PPDEFS_CPLX }

=head2 ppdefs_all

=for ref

Returns an array of pp symbols for all types including complex.

=for example

 pdl> print PDL::Types::ppdefs_all
 A B S U L K N P Q F D E G C H

=cut

my @PPDEFS_ALL = map $_->{ppsym}, @HASHES;
sub ppdefs_all { @PPDEFS_ALL }

sub typefld {
  my ($type,$fld) = @_;
  croak "unknown type $type" unless exists $typehash{$type};
  croak "unknown field $fld in type $type"
     unless exists $typehash{$type}->{$fld};
  return $typehash{$type}->{$fld};
}

sub mapfld {
	my ($type,$src,$trg) = @_;
	my @keys = grep {$typehash{$_}->{$src} eq $type} typesrtkeys;
	return @keys > 0 ? $typehash{$keys[0]}->{$trg} : undef;
}

=head2 typesynonyms

=for ref

return type related synonym definitions to be included in pdl.h .
This routine must be updated to include new types as required.
Mostly the automatic updating should take care of the vital
things.

=cut

sub typesynonyms {
  my $add = join "\n",
      map {"#define PDL_".typefld($_,'ppsym')." ".typefld($_,'sym')}
        grep {"PDL_".typefld($_,'ppsym') ne typefld($_,'sym')} typesrtkeys;
  return "$add\n";
}

=head1 PDL TYPES OVERVIEW

As of 2.065, PDL supports these types:

=over

=item SByte

Signed 8-bit value.

=item Byte

Unsigned 8-bit value.

=item Short

Signed 16-bit value.

=item UShort

Unsigned 16-bit value.

=item Long

Signed 32-bit value.

=item ULong

Unsigned 32-bit value.

=item Indx

Signed value, same size as a pointer on the system in use.

=item ULongLong

Unsigned 64-bit value.

=item LongLong

Signed 64-bit value.

=item Float

L<IEEE 754|https://en.wikipedia.org/wiki/IEEE_754> single-precision real
floating-point value.

=item Double

IEEE 754 double-precision real value.

=item LDouble

A C99 "long double", defined as "at least as precise as a double",
but often more precise.

=item CFloat

A C99 complex single-precision floating-point value.

=item CDouble

A C99 complex double-precision floating-point value.

=item CLDouble

A C99 complex "long double" - see above for description.

=back

=head1 PDL::Type OBJECTS

This module declares one class - C<PDL::Type> - objects of this class
are returned by the L<type|PDL::Core/type> method of an ndarray.  It has
several methods, listed below, which provide an easy way to access
type information:

Additionally, comparison and stringification are overloaded so that
you can compare and print type objects, e.g.

  $nofloat = 1 if $pdl->type < float;
  die "must be double" if $type != double;

For further examples check again the
L<type|PDL::Core/type> method.

=over 4

=item enum

Returns the number representing this datatype (see L<get_datatype|PDL::Core/PDL::get_datatype>).

=item symbol

Returns one of 'PDL_SB', 'PDL_B', 'PDL_S', 'PDL_US', 'PDL_L',
'PDL_UL', 'PDL_IND', 'PDL_ULL', 'PDL_LL', 'PDL_F', 'PDL_D', 'PDL_LD',
'PDL_CF', 'PDL_CD', or 'PDL_CLD'.

=item ctype

Returns the macro used to represent this type in C code (eg 'PDL_Long').

=item convertfunc

Synonym for C<ctype>.

=item ppsym

The letter used to represent this type in PP code (eg 'U' for L<ushort|PDL::Core/ushort>).

=item realctype

The actual C type used to store this type.

=item shortctype

The value returned by C<ctype> without the 'PDL_' prefix.

=item badvalue

The special numerical value used to represent bad values for this type.
See L<PDL::Bad/badvalue> for more details.

=item isnan

Given a string representing a C value, will return a C expression for
this type that indicates whether that value is NaN (for complex values,
if I<either> is NaN).

=item isfinite

Given a string representing a C value, will return a C expression for
this type that indicates whether that value is finite (for complex values,
if I<both> are finite).

=item floatsuffix

The string appended to floating-point functions for this floating-point
type. Dies if called on non-floating-point type.

=item orig_badvalue

The default special numerical value used to represent bad values for this
type. (You can change the value that represents bad values for each type
during runtime.) See the
L<orig_badvalue routine in PDL::Bad|PDL::Bad/orig_badvalue> for more details.

=item bswap

Returns the appropriate C<bswap*> from L<PDL::IO::Misc> for the size of
this type, including a no-op for types of size 1. Note this means a
one-line construction means you must call the return value:

  $pdl->type->bswap->($pdl);

=item real

Returns whether the type is real-only (true) or can hold complex values
(false).

  die "Real data only!" if !$pdl->type->real;

=item unsigned

Returns whether the type can hold signed values (false) or not (true).

=item integer

Returns whether the type can hold non-integer, a.k.a. floating-point,
values (false) or not (true).

=back

=cut

my @CACHED_TYPES = map bless([$_->{numval}, $_], 'PDL::Type'), @HASHES;
# return all known types as type objects
sub types { @CACHED_TYPES }

{
    package PDL::Type;
    use Carp;
    sub new {
        my ($type,$val) = @_;
        return $val if "PDL::Type" eq ref $val;
        if(ref $val and $val->isa('PDL')) {
            PDL::Core::barf("Can't make a type out of non-scalar ndarray $val!")
              if $val->getndims != 0;
            $val = $val->at;
        }
        confess "Can't make a type out of non-scalar $val (".
            (ref $val).")!" if ref $val;
        confess "Unknown type string '$val' (should be one of ".
            join(",",map $PDL::Types::typehash{$_}->{ioname}, @names).
            ")\n"
            if !defined $PDL::Types::typenames{$val};
        $CACHED_TYPES[$PDL::Types::typenames{$val}];
    }

    sub enum { $_[0][0] }
    *symbol = \&sym;

    sub realversion {
      $CACHED_TYPES[$PDL::Types::typenames{ $_[0][1]{realversion} }];
    }

    sub complexversion {
      $CACHED_TYPES[$PDL::Types::typenames{ $_[0][1]{complexversion} }];
    }

    sub isnan { sprintf $_[0][1]{isnan}, $_[1] }
    sub isfinite { sprintf $_[0][1]{isfinite}, $_[1] }

    sub floatsuffix { $_[0][1]{floatsuffix} // 'floatsuffix called on non-float type' }

    my %bswap_cache;
    sub bswap {
      PDL::Core::barf('Usage: $type->bswap with no args') if @_ > 1;
      return $bswap_cache{$_[0][0]} if $bswap_cache{$_[0][0]};
      my $size = PDL::Core::howbig($_[0][0]);
      return $bswap_cache{$_[0][0]} = sub {} if $size < 2;
      require PDL::IO::Misc;
      $bswap_cache{$_[0][0]} =
        $size == 2 ? \&PDL::bswap2 :
        $size == 4 ? \&PDL::bswap4 :
        $size == 8 ? \&PDL::bswap8 :
        PDL::Core::barf("bswap couldn't find swap function for $_[0][1]{shortctype}");
    }

    sub ctype { $_[0][1]{ctype}; }
    sub ppsym { $_[0][1]{ppsym}; }
    sub convertfunc { $_[0][1]{convertfunc}; }
    sub shortctype { $_[0][1]{shortctype}; }
    sub sym { $_[0][1]{sym}; }
    sub numval { $_[0][1]{numval}; }
    sub ioname { $_[0][1]{ioname}; }
    sub defbval { $_[0][1]{defbval}; }
    sub realctype { $_[0][1]{realctype}; }
    sub ppforcetype { $_[0][1]{ppforcetype}; }
    sub usenan { $_[0][1]{usenan}; }
    sub real { $_[0][1]{real}; }
    sub unsigned { $_[0][1]{unsigned}; }
    sub integer { $_[0][1]{integer}; }
    sub identifier { $_[0][1]{identifier}; }
#line 808 "Basic/Core/Types.pm.PL"
sub badvalue {
  PDL::Bad::_badvalue_int( $_[1], $_[0][0] );
}
sub orig_badvalue {
  PDL::Bad::_default_badvalue_int($_[0][0]);
}

# make life a bit easier
use overload (
  '""'  => sub { lc $_[0]->shortctype },
  "eq"  => sub { my ($self, $other, $swap) = @_; ("$self" eq $other); },
  "cmp" => sub { my ($self, $other, $swap) = @_;
    $swap ? $other cmp "$self" : "$self" cmp $other;
  },
  "<=>" => sub { $_[2] ? $_[1][0] <=> $_[0][0] : $_[0][0] <=> $_[1][0] },
);

} # package: PDL::Type
# Return
1;

__END__

=head1 DEVELOPER NOTES ON ADDING/REMOVING TYPES

You can change the types that PDL knows about by editing entries in
the definition of the variable C<@types> that appears close to the
top of the file F<Types.pm.PL> (i.e. the file from which this module
was generated).

=head2 Format of a type entry

Each entry in the C<@types> array is a hash reference. Here is an example
taken from the actual code that defines the C<ushort> type:

	     {
	      identifier => 'US',
	      onecharident => 'U',   # only needed if different from identifier
	      pdlctype => 'PDL_Ushort',
	      realctype => 'unsigned short',
	      ppforcetype => 'ushort',
	      usenan => 0,
	      packtype => 'S*',
	      defaultbadval => 'USHRT_MAX',
	      real=>1,
	      integer=>1,
	      unsigned=>1,
	     },

Before we start to explain the fields please take this important
message on board:
I<entries must be listed in order of increasing complexity>. This
is critical to ensure that PDL's type conversion works correctly.
Basically, a less complex type will be converted to a more complex
type as required.

=head2 Fields in a type entry

Each type entry has a number of required and optional entry.

A list of all the entries:

=over

=item *

identifier

I<Required>. A short sequence of uppercase letters that identifies this
type uniquely. More than three characters is probably overkill.


=item *

onecharident

I<Optional>. Only required if the C<identifier> has more than one character.
This should be a unique uppercase character that will be used to reference
this type in PP macro expressions of the C<TBSULFD> type - see L<PDL::PP/$T>.

=item *

pdlctype

I<Required>. The C<typedef>ed name that will be used to access this type
from C code.

=item *

realctype

I<Required>. The C compiler type that is used to implement this type.
For portability reasons this one might be platform dependent.

=item *

ppforcetype

I<Required>. The type name used in PP signatures to refer to this type.

=item *

usenan

I<Required>. Flag that signals if this type has to deal with NaN issues.
Generally only required for floating point types.

=item *

packtype

I<Required>. The Perl pack type used to pack Perl values into the machine representation for this type. For details see C<perldoc -f pack>.

=item *

integer

I<Required>. Boolean - is this an integer type?

=item *

unsigned

I<Required>. Boolean - is this an unsigned type?

=item *

real

I<Required>. Boolean - is this a real (not complex) type?

=item *

realversion

String - the real version of this type (e.g. cdouble -> 'D').

=item *

complexversion

String - the complex version of this type (e.g. double -> 'C').

=back

Also have a look at the entries at the top of F<Types.pm.PL>.

The syntax is not written into stone yet and might change as the
concept matures.

=head2 Other things you need to do

You need to check modules that do I/O (generally in the F<IO>
part of the directory tree). In the future we might add fields to
type entries to automate this. This requires changes to those IO
modules first though.

You should also make sure that any type macros in PP files
(i.e. C<$TBSULFD...>) are updated to reflect the new type. PDL::PP::Dump
has a mode to check for type macros requiring updating. Do something like

    find . -name \*.pd -exec perl -Mblib=. -M'PDL::PP::Dump=typecheck' {} \;

from the PDL root directory I<after> updating F<Types.pm.PL> to check
for such places.

=cut

