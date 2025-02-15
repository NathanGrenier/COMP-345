# automatically built from Config.pm.PL
# don't modify, all changes will be lost !!!!
package PDL::Doc::Config;

$PDL::Doc::pager = 'more /e';
$PDL::Doc::pager = $ENV{PAGER} if defined $ENV{PAGER};
$PDL::Doc::pager = $ENV{PERLDOC_PAGER} if defined $ENV{PERLDOC_PAGER};
$PDL::Doc::DefaultFile = '';

1;

