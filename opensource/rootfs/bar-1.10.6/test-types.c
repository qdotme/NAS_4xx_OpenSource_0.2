#include "config.h"

#include "headers.h"
#include "types.h"

void test_u8(void)
{
	uint8 u8 = 0;
	uint8 u8_old = 0;
	unsigned int c = 0;

	u8 = 1;
	for (c = 0; u8 != u8_old; c++) {
		u8_old = u8;
		u8 <<= 1;
		u8 |= 1;
	}
	assert(c == 8);
}

void test_u16(void)
{
	uint16 u16 = 0;
	uint16 u16_old = 0;
	unsigned int c = 0;

	u16 = 1;
	for (c = 0; u16 != u16_old; c++) {
		u16_old = u16;
		u16 <<= 1;
		u16 |= 1;
	}
	assert(c == 16);
}

void test_u32(void)
{
	uint32 u32 = 0;
	uint32 u32_old = 0;
	unsigned int c = 0;

	u32 = 1;
	for (c = 0; u32 != u32_old; c++) {
		u32_old = u32;
		u32 <<= 1;
		u32 |= 1;
	}
	assert(c == 32);
}

void test_u64(void)
{
	uint64 u64 = 0;
	uint64 u64_old = 0;
	unsigned int c = 0;

	u64 = 1;
	for (c = 0; u64 != u64_old; c++) {
		u64_old = u64;
		u64 <<= 1;
		u64 |= 1;
	}
	assert(c == 64);
}

int main(int argc, char* argv[])
{
	test_u8();
	test_u16();
	test_u32();
	test_u64();
	return(0);
}
