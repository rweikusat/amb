#
# recursive Perl implementation of https://rosettacode.org/wiki/Amb
#
# [I did this for fun because I haven't programmed anything for a month
#  and to fill the additional files slot]
#

my @in = (
          [qw(the that a)],
          [qw(frog elephant thing)],
          [qw(walked treaded grows)],
          [qw(slowly quickly)]);

sub last_eq_first
{
    return substr($_[0], -1, 1) eq substr($_[1], 0, 1);
}

sub chained
{
    my $cur = shift;

    for (@_) {
        return unless last_eq_first($cur, $_);
        $cur = $_;
    }

    return 1;
}

sub amb
{
    my ($elems, $pred, $comb) = @_;
    my ($mine, @remain);

    return $pred->(@$comb) unless @$elems;

    @remain = @$elems;
    $mine = shift(@remain);
    
    for (@$mine) {
        push(@$comb, $_);
        return @$comb if amb(\@remain, $pred, $comb);
        pop(@$comb);
    }

    return;
}

my @res = amb(\@in, \&chained);

if (@res) {
    print("@res\n");
} else {
    print("no solution\n");
}
