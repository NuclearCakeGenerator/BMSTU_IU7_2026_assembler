#include <cstddef>
#include <iostream>
#include "types/index.h"

static std::size_t string_length_asm(const char* str) {
    std::size_t len = 0;

    asm volatile(
        "1:\n\t"
        "ldrb w1, [%[s], %[l]]\n\t"
        "cbz w1, 2f\n\t"
        "add %[l], %[l], #1\n\t"
        "b 1b\n\t"
        "2:\n\t"
        : [l] "+r"(len)
        : [s] "r"(str)
        : "w1", "cc", "memory");

    return len;
}

static void array_add_neon_asm(const int* a, const int* b, int* result, int size) {
    asm volatile(
        "cbz %[n], 2f\n\t"
        "1:\n\t"
        "cmp %[n], #4\n\t"
        "blt 3f\n\t"

        "ld1 {v0.4s}, [%[pa]], #16\n\t"
        "ld1 {v1.4s}, [%[pb]], #16\n\t"
        "add v2.4s, v0.4s, v1.4s\n\t"
        "st1 {v2.4s}, [%[pr]], #16\n\t"

        "sub %[n], %[n], #4\n\t"
        "b 1b\n\t"

        "3:\n\t"
        "cbz %[n], 2f\n\t"
        "4:\n\t"
        "ldr w9, [%[pa]], #4\n\t"
        "ldr w10, [%[pb]], #4\n\t"
        "add w9, w9, w10\n\t"
        "str w9, [%[pr]], #4\n\t"
        "subs %[n], %[n], #1\n\t"
        "b.ne 4b\n\t"

        "2:\n\t"
        : [pa] "+r"(a), [pb] "+r"(b), [pr] "+r"(result), [n] "+r"(size)
        :
        : "v0", "v1", "v2", "x9", "x10", "cc", "memory");
}

int main() {
    std::cout << "Subtask 1: string length via inline ARM asm\n";
    const char* text = "Hello from ARM asm";
    const std::size_t len = string_length_asm(text);
    std::cout << "String: \"" << text << "\"\n";
    std::cout << "Length: " << len << "\n\n";

    std::cout << "Subtask 2: array addition via inline NEON asm\n";
    const int size = ARRAY_SIZE;
    int a[size] = {1, 2, 3, 4, 5};
    int b[size] = {10, 20, 30, 40, 50};
    int result[size] = {};

    array_add_neon_asm(a, b, result, size);

    std::cout << "NEON processes 4 integers per step, then finishes the tail in asm.\n";
    std::cout << "Result of array addition: ";
    for (int i = 0; i < size; ++i) {
        std::cout << result[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}