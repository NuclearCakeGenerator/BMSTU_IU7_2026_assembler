#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INPUT_FILE "input.txt"

typedef struct {
    size_t rows;
    size_t cols;
    double* data;
} Matrix;

static void free_matrix(Matrix* m) {
    if (m->data) free(m->data);
    m->data = NULL;
    m->rows = m->cols = 0;
}

static int print_number(double v, char* buf, size_t n) {
    if (fabs(v - round(v)) < 1e-9) {
        long long iv = llround(v);
        return snprintf(buf, n, "%lld", iv);
    }
    /* print with 6 decimals then strip trailing zeros */
    char tmp[64];
    int len = snprintf(tmp, sizeof(tmp), "%.6f", v);
    if (len <= 0) return snprintf(buf, n, "%.6f", v);

    /* strip trailing zeros and optional dot */
    while (len > 0 && tmp[len-1] == '0') --len;
    if (len > 0 && tmp[len-1] == '.') --len;
    if ((size_t)len >= n) len = (int)(n-1);
    memcpy(buf, tmp, (size_t)len);
    buf[len] = '\0';
    return len;
}

static int read_matrix(FILE* f, Matrix* m, const char* name) {
    if (fscanf(f, "%zu %zu", &m->rows, &m->cols) != 2) {
        fprintf(stderr, "Failed to read dimensions for %s\n", name);
        return 0;
    }
    if (m->rows == 0 || m->cols == 0) {
        fprintf(stderr, "%s must have positive dimensions\n", name);
        return 0;
    }
    m->data = (double*)malloc(m->rows * m->cols * sizeof(double));
    if (!m->data) {
        fprintf(stderr, "Out of memory\n");
        return 0;
    }
    for (size_t i = 0; i < m->rows * m->cols; ++i) {
        if (fscanf(f, "%lf", &m->data[i]) != 1) {
            fprintf(stderr, "Failed to read data for %s at element %zu\n", name, i+1);
            free_matrix(m);
            return 0;
        }
    }
    return 1;
}

static int load_input(const char* path, Matrix* a, Matrix* b) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Cannot open input file: %s\n", path);
        return 0;
    }
    if (!read_matrix(f, a, "first matrix")) { fclose(f); return 0; }
    if (!read_matrix(f, b, "second matrix")) { fclose(f); free_matrix(a); return 0; }

    char extra[32];
    if (fscanf(f, "%31s", extra) == 1) {
        fprintf(stderr, "Input file contains extra data after the second matrix\n");
        fclose(f);
        free_matrix(a); free_matrix(b);
        return 0;
    }
    fclose(f);
    if (a->cols != b->rows) {
        fprintf(stderr, "Inconsistent matrix sizes: %zux%zu cannot be multiplied by %zux%zu\n",
                a->rows, a->cols, b->rows, b->cols);
        free_matrix(a); free_matrix(b);
        return 0;
    }
    return 1;
}


static void multiply_block_avx2(const double* left_row,
                                const double* right_matrix,
                                size_t inner_size,
                                size_t right_cols,
                                size_t column_start,
                                double* output_block) {
    const double* left_ptr = left_row;
    const double* right_ptr = right_matrix + column_start;
    size_t remaining = inner_size;
    const size_t right_stride_bytes = right_cols * sizeof(double);

    __asm__ volatile(
        "vxorpd %%ymm0, %%ymm0, %%ymm0\n\t"
        "test %[remaining], %[remaining]\n\t"
        "jz 2f\n\t"
        "1:\n\t"
        "vbroadcastsd (%[left_ptr]), %%ymm1\n\t"
        "vmovupd (%[right_ptr]), %%ymm2\n\t"
        "vmulpd %%ymm2, %%ymm1, %%ymm1\n\t"
        "vaddpd %%ymm1, %%ymm0, %%ymm0\n\t"
        "add $8, %[left_ptr]\n\t"
        "add %[right_stride], %[right_ptr]\n\t"
        "dec %[remaining]\n\t"
        "jnz 1b\n\t"
        "2:\n\t"
        "vmovupd %%ymm0, (%[output_block])\n\t"
        "vzeroupper\n\t"
        : [left_ptr] "+r"(left_ptr),
          [right_ptr] "+r"(right_ptr),
          [remaining] "+r"(remaining)
        : [right_stride] "r"(right_stride_bytes),
          [output_block] "r"(output_block)
        : "ymm0", "ymm1", "ymm2", "memory"
    );
}

static Matrix multiply_matrices(const Matrix* left, const Matrix* right) {
    Matrix res;
    res.rows = left->rows;
    res.cols = right->cols;
    res.data = (double*)calloc(res.rows * res.cols, sizeof(double));

    for (size_t r = 0; r < left->rows; ++r) {
        const double* left_row = left->data + r * left->cols;
        double* result_row = res.data + r * res.cols;

        size_t c = 0;
        for (; c + 3 < right->cols; c += 4) {
            multiply_block_avx2(left_row, right->data, left->cols, right->cols, c, result_row + c);
        }
        for (; c < right->cols; ++c) {
            double sum = 0.0;
            for (size_t k = 0; k < left->cols; ++k) {
                sum += left_row[k] * right->data[k * right->cols + c];
            }
            result_row[c] = sum;
        }
    }
    return res;
}

static void print_matrix(const Matrix* m, const char* title) {
    printf("%s (%zux%zu)\n", title, m->rows, m->cols);
    char buf[80];
    for (size_t r = 0; r < m->rows; ++r) {
        for (size_t c = 0; c < m->cols; ++c) {
            if (c) putchar(' ');
            print_number(m->data[r * m->cols + c], buf, sizeof(buf));
            fputs(buf, stdout);
        }
        putchar('\n');
    }
}

int main(void) {
    Matrix A = {0,0,NULL}, B = {0,0,NULL};
    if (!load_input(INPUT_FILE, &A, &B)) return 1;
    Matrix R = multiply_matrices(&A, &B);

    printf("Input file: %s\n", INPUT_FILE);
    print_matrix(&A, "First matrix");
    print_matrix(&B, "Second matrix");
    print_matrix(&R, "Result matrix");

    free_matrix(&A); free_matrix(&B); free_matrix(&R);
    return 0;
}