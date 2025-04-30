our $lvl;

sub amb(;&@)
{
    my ($act, $mine, @remain) = @_;
    my @ret;
    local $lvl = $lvl + 1;

    die("no solution\n") unless @_ > 1;

    print STDERR ("$lvl: mine $mine, remain @remain\n");
    
    {
        eval { @ret = $act->($mine); last };
        return amb($act, @remain);
    }

    @ret[0 .. $#ret];
}

sub p
{
    return substr($_[0], -1, 1) eq substr($_[1], 0, 1);
}

my @r = amb {
    my $w0 = shift;

    amb {
        my $w1 = shift;
        amb unless p($w0, $w1);

        amb {
            my $w2 = shift;
            amb unless p($w1, $w2);
            
            amb {
                my $w3 = shift;
                amb unless p($w2, $w3);
                
                ($w0, $w1, $w2, $w3);
            } qw(slowly quickly);
            
        } qw(walked treaded grows);
        
    } qw(frog elephant thing);
    
} qw(the that a);

print("@r\n");
