#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>

uint16_t getbits(uint16_t buf_big_endian, int start, int count = 1)
{
	constexpr int opcode_size = 4;

	if (count <= 0 || start + count > opcode_size)
		return 0;

	return (buf_big_endian << (start * opcode_size)) >> (opcode_size * (opcode_size - count));
}

int main(void)
{
	std::cout << "Hello world!" << std::endl;

	std::ifstream f;
	f.open("test_opcode.ch8", std::ios::binary | std::ios::in);

	if (f.bad())
	{
		return 1;
	}

	char buf[2];
	f.seekg(0, f.end);
	int size = f.tellg();
	f.seekg(0, f.beg);
	int seekpos = 0;

	while (seekpos < size)
	{
		f.read(buf, 2);

		std::swap(buf[0], buf[1]); // little endian conversor

		uint16_t opcode = *reinterpret_cast<uint16_t*>(buf);
		uint16_t byte = getbits(opcode, 0,2);

		seekpos += 2;
	}

	return 0;
}