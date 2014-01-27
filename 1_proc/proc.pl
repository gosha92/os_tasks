use Getopt::Long;

# ================== ПРОВОДА ========================
# Чтобы сломать провод, надо установить ему
# свойство 'off' => 1
my %provod = (
	'адрком' => {'value' => 0}
);
# Функция provod может выступать в качестве lvalue,
# поэтому проверка работоспособности провода
# происходит внутри нее
$null; # Пустота
sub provod : lvalue {
	$null = 0;
	s/[\r\n]//g;
	if (!$provod{$_[0]}{'off'}) {
		return $provod{$_[0]}{'value'};
	} else {
		return $null;
	}
}
# ===================================================



# ==================== ФЛАГИ ========================
my %flag = (
	'пуск' => 1
);
# ===================================================



# ==================== КОМАНДЫ ======================
my %commands = (
	'00' => {            'п' => '0', 'оп' => '0', 'перех' => '0'},
	'11' => {'и' => '0', 'п' => '1', 'оп' => '1', 'перех' => '0'},
	'15' => {'и' => '1', 'п' => '1', 'оп' => '1', 'перех' => '0'},
	'02' => {            'п' => '2', 'оп' => '0', 'перех' => '0'},
	'21' => {'и' => '0', 'п' => '1', 'оп' => '2', 'перех' => '0'},
	'25' => {'и' => '1', 'п' => '1', 'оп' => '2', 'перех' => '0'},
	'31' => {'и' => '0', 'п' => '1', 'оп' => '3', 'перех' => '0'},
	'FE' => {            'п' => '4', 'оп' => 'F', 'перех' => '1'},
	'F0' => {            'п' => '4', 'оп' => 'F'                },
	'F1' => {            'п' => '4', 'оп' => 'F'                },
	'F4' => {            'п' => '4', 'оп' => 'F'                },
	'F5' => {            'п' => '4', 'оп' => 'F'                }, 
	'FF' => {            'п' => '4', 'оп' => 'F', 'перех' => '0'}
);
# ===================================================



my $UKKOM = 0; # УККОМ
my @M;     # ПАМЯТЬ
my @RK;    # РЕГКОМ
my $IR = 0;
my $RON = 0;
my $RON_PR;
my ($tmp, $tmp2);



##### ОПЦИИ КОМАНДНОЙ СТРОКИ, ОТКРЫТИЕ ФАЙЛА #####
sub help {
	print "\n	-help, -h    -  Print this help message\n	-f filename  -  File with source code\n	-t           -  Print temporary dump\n	-s           -  Sleep after dump print\n";
}
unless (@ARGV) {
	help();
	exit(0);
}
#
my ($f, $t, $s);
GetOptions ( "help" => \&help, "h" => \&help, "f=s" => \$f, "t" => \$t, "s" => \$s );
#
unless ($f) {
	help();
	exit(0);
}
open F, $f or die "No such file (${f})\n";
while(<F>) {
	next unless $_;
	for my$i (split /\s+/, $_) {
		push @M, $i;
	}
}
close F;
###################################################

while(1) {

	last if !$flag{'пуск'};
	
	provod('адрком') = $UKKOM;
	
	# Перенос команды в РЕГКОМ
	provod('ком') = $M[provod('адрком')];
	$RK[0] = provod('ком');
	provod('ком') = $M[provod('адрком') + 1];
	$RK[1] = provod('ком');
	provod('ком') = $M[provod('адрком') + 2];
	$RK[2] = provod('ком');
	
	# Разделение команды
	provod('коп') = $RK[0];
	provod('а') = $RK[1].$RK[2];
	
	# Сложение
	provod('инд') = $IR;
	provod('иа') = hex(provod('инд')) + hex(provod('а'));
	
	provod('сп') = hex($M[provod('иа')].$M[provod('иа') + 1]);
	
	# Установка флагов
	for (keys %flag) {$flag{$_} = 0}; # Сброс флагов
	$flag{'пуск'}  = 1 if (provod('коп') ne 'FF');
	$flag{'взап1'} = 1 if ($commands{provod('коп')}{'п'} == 3);
	$flag{'зам1'}  = 1 if ($commands{provod('коп')}{'п'} == 1);
	$flag{'зам2'}  = 1 if ($commands{provod('коп')}{'п'} != 3);
	$flag{'чист'}  = 1 if !($commands{provod('коп')}{'п'} == 2 || $commands{provod('коп')}{'п'} == 3);
	$flag{'оп'}    = $commands{provod('коп')}{'оп'};
	$flag{'выб'}   = $commands{provod('коп')}{'и'};
	$flag{'запп'}  = 1 if !($commands{provod('коп')}{'п'});
	
	# M1
	  if ($flag{'выб'} eq '0') {provod('м-алу') = provod('сп')}
	  if ($flag{'выб'} eq '1') {provod('м-алу') = provod('иа')}
	
	# АЛУ
	provod('сум') = $RON;
	  if ($flag{'оп'} eq '0') {provod('рез1') = provod('сум')}
	  if ($flag{'оп'} eq '1') {provod('рез1') = provod('м-алу')}
	  if ($flag{'оп'} eq '2') {provod('рез1') = provod('сум') + provod('м-алу')}
	  if ($flag{'оп'} eq '3') {provod('рез1') = provod('сум') - provod('м-алу')}
	  if ($flag{'оп'} eq 'F') {provod('рез1') = provod('сум')}
	# Установка провода ПР
	$tmp = provod('сум');
	$tmp2 = '';
	  if ($tmp == 0) {$tmp2  = '0'}
	  if ($tmp != 0) {$tmp2  = '1'}
	  if ($tmp <= 0) {$tmp2 .= '0'}
	  if ($tmp > 0)  {$tmp2 .= '1'}
	provod('пр') = $tmp2;
	
	# Запись в память
	if($flag{'запп'}) {
		$tmp = &ashex(provod('рез1'));
		$M[provod('иа')]     = substr($tmp, 0, 2);
		$M[provod('иа') + 1] = substr($tmp, 2, 2);
	}
	
	# Установка РОН
	if ($flag{'зам1'}) {
		$RON    = provod('рез1');
	} else {
		$RON_PR = provod('пр');
	}
	
	# Установка провода ПРЗНК
	provod('прзнк') = $RON_PR;
	
	# Установка флага ПЕРЕХ
	if (provod('коп') eq 'FE' || (provod('коп') eq 'F0' && provod('прзнк') =~ /0./) || (provod('коп') eq 'F1' && provod('прзнк') =~ /.1/)) {
		$flag{'перех'} = 1;
	}
	
	# Запись в ИР
	if ($flag{'чист'}) {
		provod('м-ир') = 0;
	} else {
		provod('м-ир') = provod('рез1');
	}
	$IR = provod('м-ир');
	
	# Новый адрес команды
	provod('адрком') += 3;
	if ($flag{'перех'}) {
		$UKKOM = provod('иа');
	} else {
		$UKKOM = provod('адрком');
	}
	
	# Сброс проводов
	for (keys %provod) {$provod{$_}{'value'} = 0};
	
	&mem_dump() if $t;
}

print "\n\n                  FINAL DUMP\n\n";
&mem_dump();

sub ashex {
	my $a = sprintf('%X', $_[0]);
	$a = ('0'x(4-length($a))).$a;
	return $a;
}

sub mem_dump {
	$, = ' ';
	my $l = scalar @M;
	my $a = $l / 16;
	my $b = $l % 16;
	my $i = 0;
	for (1..$a) {
		print @M[$i..($i+15)];
		print "\n";
		$i+=16;
	}
	if ($b) {
		print @M[$i..($i+$b)];
		print "\n";
	}
	print "\n";
	sleep(1) if $s;
}
