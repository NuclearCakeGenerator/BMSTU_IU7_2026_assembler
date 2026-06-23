#include <iostream>
#include <cstring>

extern "C" void* asm_memmove_copy64(char* dst, const char* src, unsigned long len);

unsigned long asm_strlen_inline64(const char* str)
{
	unsigned long count = 0;

	asm volatile(
		".intel_syntax noprefix\n\t"
		"mov rcx, -1\n\t"
		"cld\n\t"
		"xor al, al\n\t"
		"repne scasb\n\t"
		"not rcx\n\t"
		"dec rcx\n\t"
		".att_syntax prefix\n\t"
		: "=&c"(count) // из rcx to count
		: "D"(str)     // str to rdi
		: "rax", "memory"
	);

	return count;
}

bool safe_strlen64(const char* str, unsigned long* out)
{
	if (str == nullptr || out == nullptr) {
		return false;
	}

	*out = asm_strlen_inline64(str);
	return true;
}


bool test_strlen_empty_string()
{
	const char* input = "";
	unsigned long res = asm_strlen_inline64(input);
	unsigned long correct_ans = 0;
	bool ok = res == correct_ans;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}

bool test_strlen_hello_string()
{
	const char* input = "hello";
	unsigned long res = asm_strlen_inline64(input);
	unsigned long correct_ans = 5;
	bool ok = res == correct_ans;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}

bool test_strlen_null_ptr()
{
	const char* input = nullptr;
	unsigned long out = 0;
	bool res = safe_strlen64(input, &out);
	bool correct_ans = false;
	bool ok = res == correct_ans;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}


bool test_memmove_no_overlap()
{

	char expected[] = "34345";
	char actual[] = "12345";

	asm_memmove_copy64(actual, actual + 2, 2);

	char* res = actual;
	char* correct_ans = expected;
	bool ok = std::strcmp(res, correct_ans) == 0;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}


bool test_memmove_overlap_right()
{

	char expected[] = "11235";
	char actual[] = "12345";

	asm_memmove_copy64(actual + 1, actual, 3);

	char* res = actual;
	char* correct_ans = expected;
	bool ok = std::strcmp(res, correct_ans) == 0;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}


bool test_memmove_overlap_left()
{

	char expected[] = "23445";
	char actual[] = "12345";

	asm_memmove_copy64(actual, actual + 1, 3);

	char* res = actual;
	char* correct_ans = expected;
	bool ok = std::strcmp(res, correct_ans) == 0;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}

bool test_memmove_exact_overlap()
{
	char expected[] = "12345";
	char actual[] = "12345";

	asm_memmove_copy64(actual + 1, actual + 1, 2);

	char* res = actual;
	char* correct_ans = expected;
	bool ok = std::strcmp(res, correct_ans) == 0;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}

bool test_memmove_null_dst_ptr()
{
	const char* src = "12345";
	void* res = asm_memmove_copy64(nullptr, src, 0);
	void* correct_ans = nullptr;
	bool ok = res == correct_ans;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}

bool test_memmove_null_src_ptr()
{
	char dst[] = "12345";
	void* res = asm_memmove_copy64(dst, nullptr, 0);
	void* correct_ans = dst;
	bool ok = res == correct_ans;
	std::cout << __func__ << " " << (ok ? "SUCCESS" : "FAIL") << '\n';
	return ok;
}

int main()
{
	int passed = 0;
	int total = 0;

	++total; if (test_strlen_empty_string()) ++passed;
	++total; if (test_strlen_hello_string()) ++passed;
	++total; if (test_strlen_null_ptr()) ++passed;

	++total; if (test_memmove_no_overlap()) ++passed;
	++total; if (test_memmove_overlap_right()) ++passed;
	++total; if (test_memmove_overlap_left()) ++passed;
	++total; if (test_memmove_exact_overlap()) ++passed;
	++total; if (test_memmove_null_dst_ptr()) ++passed;
	++total; if (test_memmove_null_src_ptr()) ++passed;

	std::cout << "Summary " << passed << "/" << total << '\n';

	return 0;
}

