rm -rf process_test
rm -rf thread_test
rm -rf *.o
rm -rf *.c~

gcc -O3 process_test.c -o process_test
gcc -O3 thread_test.c -o thread_test -lpthread

cd syscall
rm -rf *.ko
rm -rf *.o
rm -rf *.mod*
rm -rf Module*
rm -rf modules*
rm -rf *.c~
make
rmmod syscall_wrapper.ko
dmesg -C
insmod syscall_wrapper.ko

cd ..
./process_test $1
./thread_test $1
cd syscall
rmmod syscall_wrapper.ko
dmesg

