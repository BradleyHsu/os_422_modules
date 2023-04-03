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
