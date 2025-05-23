# This file is auto-generated by the Perl DateTime Suite time zone
# code generator (0.08) This code generator comes with the
# DateTime::TimeZone module distribution in the tools/ directory

#
# Generated from /tmp/S2_G3OrWui/southamerica.  Olson data version 2024a
#
# Do not edit this file directly.
#
package DateTime::TimeZone::America::La_Paz;

use strict;
use warnings;
use namespace::autoclean;

our $VERSION = '2.62';

use Class::Singleton 1.03;
use DateTime::TimeZone;
use DateTime::TimeZone::OlsonDB;

@DateTime::TimeZone::America::La_Paz::ISA = ( 'Class::Singleton', 'DateTime::TimeZone' );

my $spans =
[
    [
DateTime::TimeZone::NEG_INFINITY, #    utc_start
59611177956, #      utc_end 1890-01-01 04:32:36 (Wed)
DateTime::TimeZone::NEG_INFINITY, #  local_start
59611161600, #    local_end 1890-01-01 00:00:00 (Wed)
-16356,
0,
'LMT',
    ],
    [
59611177956, #    utc_start 1890-01-01 04:32:36 (Wed)
60929728356, #      utc_end 1931-10-15 04:32:36 (Thu)
59611161600, #  local_start 1890-01-01 00:00:00 (Wed)
60929712000, #    local_end 1931-10-15 00:00:00 (Thu)
-16356,
0,
'CMT',
    ],
    [
60929728356, #    utc_start 1931-10-15 04:32:36 (Thu)
60943375956, #      utc_end 1932-03-21 03:32:36 (Mon)
60929715600, #  local_start 1931-10-15 01:00:00 (Thu)
60943363200, #    local_end 1932-03-21 00:00:00 (Mon)
-12756,
1,
'BST',
    ],
    [
60943375956, #    utc_start 1932-03-21 03:32:36 (Mon)
DateTime::TimeZone::INFINITY, #      utc_end
60943361556, #  local_start 1932-03-20 23:32:36 (Sun)
DateTime::TimeZone::INFINITY, #    local_end
-14400,
0,
'-04',
    ],
];

sub olson_version {'2024a'}

sub has_dst_changes {1}

sub _max_year {2034}

sub _new_instance {
    return shift->_init( @_, spans => $spans );
}



1;

