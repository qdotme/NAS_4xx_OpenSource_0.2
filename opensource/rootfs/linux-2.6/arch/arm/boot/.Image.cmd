cmd_arch/arm/boot/Image := arm-none-linux-gnueabi-objcopy -O binary -R .note -R .comment -S  vmlinux arch/arm/boot/Image
