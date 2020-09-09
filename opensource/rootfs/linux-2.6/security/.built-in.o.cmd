cmd_security/built-in.o :=  arm-none-linux-gnueabi-ld -EL   -r -o security/built-in.o security/trustees/built-in.o security/security.o security/dummy.o security/inode.o
