rm ./k_monitor.c
cp ./lab1/k_monitor.c ./k_monitor.c
LINUX_SOURCE=../linux_source/linux/
make -C $LINUX_SOURCE ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- M=$PWD modules
