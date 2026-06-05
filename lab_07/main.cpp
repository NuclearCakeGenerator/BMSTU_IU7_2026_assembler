#include <iostream>
#include <cstring>

extern "C" void asm_memmove_copy(char* dst, const char* src, unsigned long len);

unsigned long asm_strlen_inline(const char* str)
{
    const char* scan_ptr = str;
    unsigned long count = static_cast<unsigned long>(-1);

    asm volatile(
        ".intel_syntax noprefix\n\t"
        "cld\n\t"
        "xor al, al\n\t"
        "repne scasb\n\t"
        "not rcx\n\t"
        "dec rcx\n\t"
        ".att_syntax prefix\n\t"
        : "+D"(scan_ptr), "+c"(count)
        :
        : "rax", "memory"
    );

    return count;
}

int main()
{
    int passed = 0;
    int total = 0;

    std::cout << "=== strlen_asm tests ===\n";
    ++total; if (test_strlen_case("empty", "")) ++passed;
    ++total; if (test_strlen_case("one char", "a")) ++passed;
    ++total; if (test_strlen_case("simple", "Hello")) ++passed;
    ++total; if (test_strlen_case("with spaces", "  abc def  ")) ++passed;
    ++total; if (test_strlen_case("digits", "0123456789")) ++passed;
    ++total; if (test_strlen_case("long", "Assembly string operations for asm tests")) ++passed;

    std::cout << "\n=== memmove_asm tests ===\n";
    ++total; if (test_memmove_case("len = 0", 64, 0, 0, 0)) ++passed;
    ++total; if (test_memmove_case("no overlap dst < src", 128, 8, 64, 24)) ++passed;
    ++total; if (test_memmove_case("no overlap dst > src", 128, 80, 16, 24)) ++passed;
    ++total; if (test_memmove_case("exact overlap", 64, 12, 12, 20)) ++passed;

    ++total; if (test_memmove_case("partial overlap right shift 1", 96, 11, 10, 30)) ++passed;
    ++total; if (test_memmove_case("partial overlap right shift 2", 96, 12, 10, 30)) ++passed;
    ++total; if (test_memmove_case("partial overlap left shift 1", 96, 10, 11, 30)) ++passed;
    ++total; if (test_memmove_case("partial overlap left shift 3", 96, 10, 13, 30)) ++passed;

    ++total; if (test_memmove_case("touch end boundary", 128, 100, 20, 28)) ++passed;
    ++total; if (test_memmove_case("src near end", 128, 4, 100, 28)) ++passed;
    ++total; if (test_memmove_case("tiny len 1", 64, 21, 20, 1)) ++passed;
    ++total; if (test_memmove_case("tiny len 2", 64, 20, 21, 2)) ++passed;
    ++total; if (test_memmove_case("large overlap", 512, 128, 64, 300)) ++passed;

    std::cout << "\nSummary: " << passed << "/" << total << " tests passed\n";

    return 0;
}