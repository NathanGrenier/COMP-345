package Alien::CFITSIO::Install::Files;
use strict;
use warnings;
require Alien::CFITSIO;
sub Inline { shift; Alien::CFITSIO->Inline(@_) }
1;

=begin Pod::Coverage

  Inline

=cut
