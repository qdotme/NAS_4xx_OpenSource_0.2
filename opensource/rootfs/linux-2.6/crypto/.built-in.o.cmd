cmd_crypto/built-in.o :=  arm-none-linux-gnueabi-ld -EL   -r -o crypto/built-in.o crypto/api.o crypto/scatterwalk.o crypto/cipher.o crypto/digest.o crypto/compress.o crypto/crypto_algapi.o crypto/blkcipher.o crypto/crypto_hash.o crypto/cryptomgr.o crypto/hmac.o crypto/crypto_null.o crypto/md4.o crypto/md5.o crypto/sha1.o crypto/sha256.o crypto/sha512.o crypto/cbc.o crypto/des.o crypto/blowfish.o crypto/twofish.o crypto/twofish_common.o crypto/aes.o crypto/cast5.o crypto/cast6.o crypto/arc4.o crypto/tea.o crypto/deflate.o crypto/crc32c.o crypto/ocf/built-in.o