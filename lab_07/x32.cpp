#include <iostream>
#include <cstring>

extern "C" void* asm_memmove_copy32(char* dst, const char* src, unsigned long len);

unsigned long asm_strlen_inline32(const char* str)
{
	const char* scan_ptr = str;
	unsigned long count = static_cast<unsigned long>(-1);

	asm volatile(
		".intel_syntax noprefix\n\t"
		"cld\n\t"
		"xor al, al\n\t"
		"repne scasb\n\t"
		"not ecx\n\t"
		"dec ecx\n\t"
		".att_syntax prefix\n\t"
		: "+D"(scan_ptr), "+c"(count)
		:
		: "eax", "memory"
	);

	return count;
}

bool test_strlen32(const char* s)
{
	return asm_strlen_inline32(s) == static_cast<unsigned long>(std::strlen(s));
}

bool test_memmove32(unsigned long size, unsigned long dst_off, unsigned long src_off, unsigned long len)
{
	if (dst_off + len > size || src_off + len > size) {
		return false;
	}

	char* expected = new char[size];
	char* actual = new char[size];

	for (unsigned long i = 0; i < size; ++i) {
		char value = static_cast<char>('A' + (i % 26));
		expected[i] = value;
		actual[i] = value;
	}

	std::memmove(expected + dst_off, expected + src_off, static_cast<size_t>(len));
	asm_memmove_copy32(actual + dst_off, actual + src_off, len);

	bool ok = std::memcmp(expected, actual, static_cast<size_t>(size)) == 0;
	delete[] expected;
	delete[] actual;
	return ok;
}

int main()
{
	std::cout << "x32 strlen empty: " << test_strlen32("") << '\n';
	std::cout << "x32 strlen hello: " << test_strlen32("hello") << '\n';
	std::cout << "x32 strlen long: " << test_strlen32("0123456789abcdefghijklmnopqrstuvwxyz") << '\n';

	std::cout << "x32 memmove no overlap: " << test_memmove32(128, 8, 64, 24) << '\n';
	std::cout << "x32 memmove overlap right: " << test_memmove32(128, 17, 16, 32) << '\n';
	std::cout << "x32 memmove overlap left: " << test_memmove32(128, 16, 17, 32) << '\n';
	std::cout << "x32 memmove exact: " << test_memmove32(128, 40, 40, 20) << '\n';

	return 0;
}
