cmd_drivers/md/raid6int2.c := perl /opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/md/unroll.pl 2 < drivers/md/raid6int.uc > drivers/md/raid6int2.c || ( rm -f drivers/md/raid6int2.c && exit 1 )