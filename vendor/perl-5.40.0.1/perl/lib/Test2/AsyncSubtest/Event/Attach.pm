package Test2::AsyncSubtest::Event::Attach;
use strict;
use warnings;

our $VERSION = '1.302200';

use base 'Test2::Event';
use Test2::Util::HashBase qw/id/;

sub no_display { 1 }

sub callback {
    my $self = shift;
    my ($hub) = @_;

    my $id = $self->{+ID};
    my $ids = $hub->ast_ids;

    unless (defined $ids->{$id}) {
        require Test2::Event::Exception;
        my $trace = $self->trace;
        $hub->send(
            Test2::Event::Exception->new(
                trace => $trace,
                error => "Invalid AsyncSubtest attach ID: $id at " . $trace->debug . "\n",
            )
        );
        return;
    }

    if ($ids->{$id}++) {
        require Test2::Event::Exception;
        my $trace = $self->trace;
        $hub->send(
            Test2::Event::Exception->new(
                trace => $trace,
                error => "AsyncSubtest ID $id already attached at " . $trace->debug . "\n",
            )
        );
        return;
    }
}

1;

__END__

=pod

=encoding UTF-8

=head1 NAME

Test2::AsyncSubtest::Event::Attach - Event to attach a subtest to the parent.

=head1 DESCRIPTION

Used internally by L<Test2::AsyncSubtest>. No user serviceable parts inside.

=head1 SOURCE

The source code repository for Test2-AsyncSubtest can be found at
F<https://github.com/Test-More/test-more/>.

=head1 MAINTAINERS

=over 4

=item Chad Granum E<lt>exodist@cpan.orgE<gt>

=back

=head1 AUTHORS

=over 4

=item Chad Granum E<lt>exodist@cpan.orgE<gt>

=back

=head1 COPYRIGHT

Copyright Chad Granum E<lt>exodist7@gmail.comE<gt>.

This program is free software; you can redistribute it and/or
modify it under the same terms as Perl itself.

See F<http://dev.perl.org/licenses/>

=cut
