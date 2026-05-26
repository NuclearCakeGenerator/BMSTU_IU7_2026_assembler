// ...existing code...
#include <iostream>
#include "types/index.h"

static int add_scalar_asm(int lhs, int rhs) {
    int sum;
    asm volatile(
        "add %0, %1, %2"
        : "=r"(sum)
        : "r"(lhs), "r"(rhs)
        : "cc");
    return sum;
}

static void array_add_asm(const int* a, const int* b, int* result, int size) {
    for (int i = 0; i < size; ++i) {
        int va;
        int vb;
        asm volatile(
            "ldr %0, [%2]\n\t"
            "ldr %1, [%3]\n\t"
            "add %0, %0, %1\n\t"
            "str %0, [%4]"
            : "=&r"(va), "=&r"(vb)
            : "r"(a + i), "r"(b + i), "r"(result + i)
            : "memory", "cc");
    }
}

int main() {
    std::cout << "Subtask 1: scalar addition via inline ARM asm\n";
    const int x = 7;
    const int y = 11;
    const int scalar_sum = add_scalar_asm(x, y);
    std::cout << x << " + " << y << " = " << scalar_sum << "\n\n";

    std::cout << "Subtask 2: array addition via inline ARM asm\n";
    const int size = ARRAY_SIZE;
    int a[size] = {1, 2, 3, 4, 5};
    int b[size] = {10, 20, 30, 40, 50};
    int result[size] = {};

    array_add_asm(a, b, result, size);

    std::cout << "Result of array addition: ";
    for (int i = 0; i < size; ++i) {
        std::cout << result[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}