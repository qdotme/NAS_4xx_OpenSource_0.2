cmd_drivers/md/built-in.o :=  arm-none-linux-gnueabi-ld -EL   -r -o drivers/md/built-in.o drivers/md/linear.o drivers/md/raid0.o drivers/md/raid1.o drivers/md/raid10.o drivers/md/raid456.o drivers/md/xor.o drivers/md/md-mod.o drivers/md/dm-mod.o drivers/md/dm-crypt.o