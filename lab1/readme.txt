1. Bradley Hsu, b.hsu@wustl.edu

2. Module Design

3.
pi@btheshoe:~ $ sudo insmod k_monitor.ko log_sec=1 log_nsec=0
pi@btheshoe:~ $ tail -f /var/log/syslog
Apr  2 17:31:03 btheshoe rngd[532]: stats: Lowest ready-buffers level: 2
Apr  2 17:31:03 btheshoe rngd[532]: stats: Entropy starvations: 0
Apr  2 17:31:03 btheshoe rngd[532]: stats: Time spent starving for entropy: (min=0; avg=0.000; max=0)us
Apr  2 17:34:21 btheshoe kernel: [11010.899959] k_monitor module initialized
Apr  2 17:34:23 btheshoe kernel: [11011.900029] lab 1 module timer callback
Apr  2 17:34:24 btheshoe kernel: [11012.900024] lab 1 module timer callback
Apr  2 17:34:25 btheshoe kernel: [11013.900039] lab 1 module timer callback
Apr  2 17:34:26 btheshoe kernel: [11014.900039] lab 1 module timer callback
Apr  2 17:34:27 btheshoe kernel: [11015.900045] lab 1 module timer callback
Apr  2 17:34:28 btheshoe kernel: [11016.900048] lab 1 module timer callback

pi@btheshoe:~ $ sudo insmod k_monitor.ko log_sec=2 log_nsec=0
pi@btheshoe:~ $ tail -f /var/log/syslog
Apr  2 17:34:58 btheshoe kernel: [11047.980590] k_monitor module initialized
Apr  2 17:35:02 btheshoe kernel: [11049.980665] lab 1 module timer callback
Apr  2 17:35:04 btheshoe kernel: [11051.980672] lab 1 module timer callback
Apr  2 17:35:06 btheshoe kernel: [11053.980677] lab 1 module timer callback
Apr  2 17:35:08 btheshoe kernel: [11055.980684] lab 1 module timer callback
Apr  2 17:35:10 btheshoe kernel: [11057.980692] lab 1 module timer callback

pi@btheshoe:~ $ sudo insmod k_monitor.ko log_sec=0 log_nsec=50000000
pi@btheshoe:~ $ tail -f /var/log/syslog
Apr  2 17:36:06 btheshoe kernel: [11116.298497] lab 1 module timer callback
Apr  2 17:36:06 btheshoe kernel: [11116.348498] lab 1 module timer callback
Apr  2 17:36:06 btheshoe kernel: [11116.398497] lab 1 module timer callback
Apr  2 17:36:07 btheshoe kernel: [11116.448497] lab 1 module timer callback
Apr  2 17:36:07 btheshoe kernel: [11116.498497] lab 1 module timer callback
Apr  2 17:36:07 btheshoe kernel: [11116.548497] lab 1 module timer callback
Apr  2 17:36:07 btheshoe kernel: [11116.598497] lab 1 module timer callback

4.
cat ./k_thread_run.sh
sudo dmesg --clear
sudo insmod k_monitor.ko log_sec=2 log_nsec=0
sleep 6
sudo rmmod k_monitor
dmesg

sudo dmesg --clear
sudo insmod k_monitor.ko log_sec=1 log_nsec=0
sleep 3
sudo rmmod k_monitor
dmesg

sudo dmesg --clear
sudo insmod k_monitor.ko log_sec=0 log_nsec=100000000
sleep .3
sudo rmmod k_monitor
dmesg
[44756.277973] k_monitor module initialized
[44756.278571] lab 1 module body callback starting
[44756.278608] lab 1 module body callback
[44756.278631] nvcsw: 1, nivcsw: 1
[44756.278647] iterations: 1
[44758.278403] lab 1 module timer callback
[44758.278482] lab 1 module body callback
[44758.278510] nvcsw: 2, nivcsw: 2
[44758.278530] iterations: 2
[44760.278410] lab 1 module timer callback
[44760.278492] lab 1 module body callback
[44760.278519] nvcsw: 3, nivcsw: 2
[44760.278545] iterations: 3
[44762.278416] lab 1 module timer callback
[44762.278491] lab 1 module body callback
[44762.278518] nvcsw: 4, nivcsw: 2
[44762.278547] iterations: 4
[44762.368231] k_monitor module is being unloaded
[44762.368342] lab 1 module body callback terminating
[44762.595528] k_monitor module initialized
[44762.595940] lab 1 module body callback starting
[44762.595968] lab 1 module body callback
[44762.596966] nvcsw: 1, nivcsw: 1
[44762.596982] iterations: 1
[44763.595935] lab 1 module timer callback
[44763.596022] lab 1 module body callback
[44763.596058] nvcsw: 2, nivcsw: 1
[44763.596074] iterations: 2
[44764.595937] lab 1 module timer callback
[44764.596029] lab 1 module body callback
[44764.596066] nvcsw: 3, nivcsw: 1
[44764.596082] iterations: 3
[44765.595939] lab 1 module timer callback
[44765.596028] lab 1 module body callback
[44765.596063] nvcsw: 4, nivcsw: 1
[44765.596079] iterations: 4
[44765.680859] k_monitor module is being unloaded
[44765.680963] lab 1 module body callback terminating
[44765.897315] k_monitor module initialized
[44765.897707] lab 1 module body callback starting
[44765.897734] lab 1 module body callback
[44765.897757] nvcsw: 1, nivcsw: 0
[44765.897772] iterations: 1
[44765.997701] lab 1 module timer callback
[44765.997771] lab 1 module body callback
[44765.997798] nvcsw: 2, nivcsw: 0
[44765.997825] iterations: 2
[44766.097699] lab 1 module timer callback
[44766.097766] lab 1 module body callback
[44766.097793] nvcsw: 3, nivcsw: 0
[44766.097808] iterations: 3
[44766.197698] lab 1 module timer callback
[44766.197765] lab 1 module body callback
[44766.197792] nvcsw: 4, nivcsw: 0
[44766.197807] iterations: 4
[44766.283629] k_monitor module is being unloaded
[44766.283732] lab 1 module body callback terminating

5.
cat ./mt_thread_run.sh
sudo dmesg --clear
sudo insmod mt_k_monitor.ko log_sec=2 log_nsec=0
sleep 6
sudo rmmod mt_k_monitor
dmesg

sudo dmesg --clear
sudo insmod mt_k_monitor.ko log_sec=1 log_nsec=0
sleep 3
sudo rmmod mt_k_monitor
dmesg

sudo dmesg --clear
sudo insmod mt_k_monitor.ko log_sec=0 log_nsec=100000000
sleep .3
sudo rmmod mt_k_monitor
dmesg
[44528.191824] k_monitor module initialized
[44528.192774] lab 1 module body callback starting
[44528.192780] lab 1 module body callback starting
[44528.192785] lab 1 module body callback starting
[44528.192795] lab 1 module body callback
[44528.192799] lab 1 module body callback
[44528.192809] nvcsw: 1, nivcsw: 0
[44528.192821] nvcsw: 1, nivcsw: 0
[44528.192830] lab 1 module body callback
[44528.192835] iterations: 1
[44528.192844] nvcsw: 1, nivcsw: 0
[44528.192856] iterations: 1
[44528.192860] iterations: 1
[44528.192868] thread: 1
[44528.192872] thread: 0
[44528.192881] thread: 2
[44528.194244] lab 1 module body callback starting
[44528.194274] lab 1 module body callback
[44528.194296] nvcsw: 1, nivcsw: 0
[44528.194311] iterations: 1
[44528.194325] thread: 3
[44530.192787] lab 1 module timer callback
[44530.192872] lab 1 module body callback
[44530.192878] lab 1 module body callback
[44530.192894] nvcsw: 2, nivcsw: 0
[44530.192903] lab 1 module body callback
[44530.192907] lab 1 module body callback
[44530.192920] nvcsw: 2, nivcsw: 0
[44530.192926] nvcsw: 2, nivcsw: 0
[44530.192931] nvcsw: 2, nivcsw: 0
[44530.192940] iterations: 2
[44530.192944] iterations: 2
[44530.192954] iterations: 2
[44530.192958] thread: 1
[44530.192962] thread: 0
[44530.192971] iterations: 2
[44530.192981] thread: 2
[44530.192992] thread: 3
[44532.192790] lab 1 module timer callback
[44532.192870] lab 1 module body callback
[44532.192875] lab 1 module body callback
[44532.192890] nvcsw: 3, nivcsw: 0
[44532.192899] lab 1 module body callback
[44532.192904] lab 1 module body callback
[44532.192916] nvcsw: 3, nivcsw: 0
[44532.192921] nvcsw: 3, nivcsw: 0
[44532.192927] nvcsw: 3, nivcsw: 0
[44532.192935] iterations: 3
[44532.192940] iterations: 3
[44532.192949] iterations: 3
[44532.192953] thread: 0
[44532.192957] thread: 3
[44532.192965] iterations: 3
[44532.192975] thread: 2
[44532.192985] thread: 1
[44534.192792] lab 1 module timer callback
[44534.192869] lab 1 module body callback
[44534.192874] lab 1 module body callback
[44534.192880] lab 1 module body callback
[44534.192892] nvcsw: 4, nivcsw: 0
[44534.192897] nvcsw: 4, nivcsw: 0
[44534.192906] lab 1 module body callback
[44534.192910] iterations: 4
[44534.192921] nvcsw: 4, nivcsw: 0
[44534.192927] nvcsw: 4, nivcsw: 0
[44534.192930] thread: 0
[44534.192941] iterations: 4
[44534.192945] iterations: 4
[44534.192955] thread: 1
[44534.192960] thread: 3
[44534.192968] iterations: 4
[44534.192985] thread: 2
[44534.282825] k_monitor module is being unloaded
[44534.282919] lab 1 module body callback terminating
[44534.283138] lab 1 module body callback terminating
[44534.283255] lab 1 module body callback terminating
[44534.283364] lab 1 module body callback terminating
[44534.499966] k_monitor module initialized
[44534.501421] lab 1 module body callback starting
[44534.501429] lab 1 module body callback starting
[44534.501434] lab 1 module body callback starting
[44534.501446] lab 1 module body callback
[44534.501450] lab 1 module body callback
[44534.501462] nvcsw: 1, nivcsw: 1
[44534.501472] nvcsw: 1, nivcsw: 0
[44534.501483] lab 1 module body callback
[44534.501487] iterations: 1
[44534.501496] nvcsw: 1, nivcsw: 0
[44534.501508] iterations: 1
[44534.501515] thread: 2
[44534.501520] thread: 0
[44534.501528] iterations: 1
[44534.501544] thread: 1
[44534.501618] lab 1 module body callback starting
[44534.501643] lab 1 module body callback
[44534.501666] nvcsw: 1, nivcsw: 0
[44534.501681] iterations: 1
[44534.501696] thread: 3
[44535.501437] lab 1 module timer callback
[44535.501522] lab 1 module body callback
[44535.501527] lab 1 module body callback
[44535.501539] lab 1 module body callback
[44535.501546] nvcsw: 2, nivcsw: 0
[44535.501555] lab 1 module body callback
[44535.501561] nvcsw: 2, nivcsw: 0
[44535.501569] iterations: 2
[44535.501576] nvcsw: 2, nivcsw: 0
[44535.501588] nvcsw: 2, nivcsw: 1
[44535.501593] thread: 2
[44535.501597] iterations: 2
[44535.501606] iterations: 2
[44535.501610] thread: 3
[44535.501618] iterations: 2
[44535.501628] thread: 1
[44535.501639] thread: 0
[44536.501432] lab 1 module timer callback
[44536.501511] lab 1 module body callback
[44536.501517] lab 1 module body callback
[44536.501522] lab 1 module body callback
[44536.501534] nvcsw: 3, nivcsw: 1
[44536.501543] nvcsw: 3, nivcsw: 0
[44536.501555] nvcsw: 3, nivcsw: 0
[44536.501562] lab 1 module body callback
[44536.501566] iterations: 3
[44536.501577] iterations: 3
[44536.501582] nvcsw: 3, nivcsw: 0
[44536.501586] thread: 2
[44536.501593] thread: 0
[44536.501597] iterations: 3
[44536.501609] iterations: 3
[44536.501612] thread: 3
[44536.501625] thread: 1
[44537.501439] lab 1 module timer callback
[44537.501513] lab 1 module body callback
[44537.501519] lab 1 module body callback
[44537.501534] nvcsw: 4, nivcsw: 0
[44537.501543] lab 1 module body callback
[44537.501555] lab 1 module body callback
[44537.501561] nvcsw: 4, nivcsw: 1
[44537.501566] nvcsw: 4, nivcsw: 0
[44537.501576] nvcsw: 4, nivcsw: 0
[44537.501582] iterations: 4
[44537.501591] iterations: 4
[44537.501595] iterations: 4
[44537.501599] thread: 1
[44537.501607] iterations: 4
[44537.501610] thread: 3
[44537.501621] thread: 0
[44537.501625] thread: 2
[44537.590216] k_monitor module is being unloaded
[44537.590296] lab 1 module body callback terminating
[44537.591134] lab 1 module body callback terminating
[44537.591257] lab 1 module body callback terminating
[44537.591369] lab 1 module body callback terminating
[44537.829128] k_monitor module initialized
[44537.830048] lab 1 module body callback starting
[44537.830054] lab 1 module body callback starting
[44537.830067] lab 1 module body callback
[44537.830085] lab 1 module body callback
[44537.830098] nvcsw: 1, nivcsw: 0
[44537.830114] iterations: 1
[44537.830125] thread: 1
[44537.830139] nvcsw: 1, nivcsw: 0
[44537.830155] iterations: 1
[44537.830169] thread: 0
[44537.830180] lab 1 module body callback starting
[44537.830187] lab 1 module body callback starting
[44537.830200] lab 1 module body callback
[44537.830218] lab 1 module body callback
[44537.830232] nvcsw: 1, nivcsw: 0
[44537.830244] nvcsw: 1, nivcsw: 0
[44537.830254] iterations: 1
[44537.830262] iterations: 1
[44537.830274] thread: 3
[44537.830277] thread: 2
[44537.930175] lab 1 module timer callback
[44537.930259] lab 1 module body callback
[44537.930263] lab 1 module body callback
[44537.930268] lab 1 module body callback
[44537.930274] lab 1 module body callback
[44537.930287] nvcsw: 2, nivcsw: 0
[44537.930293] nvcsw: 2, nivcsw: 0
[44537.930298] nvcsw: 2, nivcsw: 0
[44537.930306] iterations: 2
[44537.930310] iterations: 2
[44537.930321] nvcsw: 2, nivcsw: 0
[44537.930325] thread: 1
[44537.930328] thread: 2
[44537.930336] iterations: 2
[44537.930350] iterations: 2
[44537.930353] thread: 0
[44537.930366] thread: 3
[44538.030176] lab 1 module timer callback
[44538.030254] lab 1 module body callback
[44538.030259] lab 1 module body callback
[44538.030264] lab 1 module body callback
[44538.030269] lab 1 module body callback
[44538.030281] nvcsw: 3, nivcsw: 0
[44538.030287] nvcsw: 3, nivcsw: 0
[44538.030293] nvcsw: 3, nivcsw: 0
[44538.030302] iterations: 3
[44538.030306] iterations: 3
[44538.030314] iterations: 3
[44538.030319] thread: 1
[44538.030330] nvcsw: 3, nivcsw: 0
[44538.030335] thread: 3
[44538.030353] thread: 2
[44538.030363] iterations: 3
[44538.030379] thread: 0
[44538.130180] lab 1 module timer callback
[44538.130260] lab 1 module body callback
[44538.130264] lab 1 module body callback
[44538.130269] lab 1 module body callback
[44538.130276] lab 1 module body callback
[44538.130287] nvcsw: 4, nivcsw: 0
[44538.130293] nvcsw: 4, nivcsw: 0
[44538.130298] nvcsw: 4, nivcsw: 0
[44538.130307] iterations: 4
[44538.130311] iterations: 4
[44538.130322] nvcsw: 4, nivcsw: 0
[44538.130325] thread: 1
[44538.130329] thread: 2
[44538.130337] iterations: 4
[44538.130350] iterations: 4
[44538.130354] thread: 0
[44538.130367] thread: 3
[44538.224527] k_monitor module is being unloaded
[44538.224609] lab 1 module body callback terminating
[44538.224730] lab 1 module body callback terminating
[44538.224844] lab 1 module body callback terminating
[44538.224936] lab 1 module body callback terminating
