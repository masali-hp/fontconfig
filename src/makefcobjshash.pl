my $compiler=shift;
my $compiler_args=shift;
my $gperf_h=shift;
my $filename=shift;

my $command="\"$compiler\" $compiler_args $gperf_h";
print $command ."\n";
my $output=`$command`;

open (HEADER, $filename) or die("Unable to open $filename");
open(my $fh_gperf, '>', 'fcobjshash.gperf') or die("Unable to open fcobjshash.gperf");

my $no_write=0;

while (<HEADER>) {
	$_ =~ s/^ *//;
	$_ =~ s/ *, */,/;
	if (/CUT_OUT_BEGIN/) {
		$no_write=1;
	}
	elsif (/CUT_OUT_END/) {
		$no_write=0;
	}
	elsif ($no_write eq 0 and /^[^#]\S/) {
		print $fh_gperf $_;
	}
}

close $fh_gperf;

`gperf --pic -m 100 fcobjshash.gperf > fcobjshash.h`;