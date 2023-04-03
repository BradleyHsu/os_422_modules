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