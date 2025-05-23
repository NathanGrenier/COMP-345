# This file is auto-generated by the Perl DateTime Suite time zone
# code generator (0.08) This code generator comes with the
# DateTime::TimeZone module distribution in the tools/ directory

#
# Generated from /tmp/S2_G3OrWui/southamerica.  Olson data version 2024a
#
# Do not edit this file directly.
#
package DateTime::TimeZone::America::Caracas;

use strict;
use warnings;
use namespace::autoclean;

our $VERSION = '2.62';

use Class::Singleton 1.03;
use DateTime::TimeZone;
use DateTime::TimeZone::OlsonDB;

@DateTime::TimeZone::America::Caracas::ISA = ( 'Class::Singleton', 'DateTime::TimeZone' );

my $spans =
[
    [
DateTime::TimeZone::NEG_INFINITY, #    utc_start
59611177664, #      utc_end 1890-01-01 04:27:44 (Wed)
DateTime::TimeZone::NEG_INFINITY, #  local_start
59611161600, #    local_end 1890-01-01 00:00:00 (Wed)
-16064,
0,
'LMT',
    ],
    [
59611177664, #    utc_start 1890-01-01 04:27:44 (Wed)
60308944060, #      utc_end 1912-02-12 04:27:40 (Mon)
59611161604, #  local_start 1890-01-01 00:00:04 (Wed)
60308928000, #    local_end 1912-02-12 00:00:00 (Mon)
-16060,
0,
'CMT',
    ],
    [
60308944060, #    utc_start 1912-02-12 04:27:40 (Mon)
61977933000, #      utc_end 1965-01-01 04:30:00 (Fri)
60308927860, #  local_start 1912-02-11 23:57:40 (Sun)
61977916800, #    local_end 1965-01-01 00:00:00 (Fri)
-16200,
0,
'-0430',
    ],
    [
61977933000, #    utc_start 1965-01-01 04:30:00 (Fri)
63332866800, #      utc_end 2007-12-09 07:00:00 (Sun)
61977918600, #  local_start 1965-01-01 00:30:00 (Fri)
63332852400, #    local_end 2007-12-09 03:00:00 (Sun)
-14400,
0,
'-04',
    ],
    [
63332866800, #    utc_start 2007-12-09 07:00:00 (Sun)
63597769200, #      utc_end 2016-05-01 07:00:00 (Sun)
63332850600, #  local_start 2007-12-09 02:30:00 (Sun)
63597753000, #    local_end 2016-05-01 02:30:00 (Sun)
-16200,
0,
'-0430',
    ],
    [
63597769200, #    utc_start 2016-05-01 07:00:00 (Sun)
DateTime::TimeZone::INFINITY, #      utc_end
63597754800, #  local_start 2016-05-01 03:00:00 (Sun)
DateTime::TimeZone::INFINITY, #    local_end
-14400,
0,
'-04',
    ],
];

sub olson_version {'2024a'}

sub has_dst_changes {0}

sub _max_year {2034}

sub _new_instance {
    return shift->_init( @_, spans => $spans );
}



1;

