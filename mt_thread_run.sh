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
