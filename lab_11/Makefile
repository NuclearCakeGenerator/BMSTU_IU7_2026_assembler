CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -mavx2
LDLIBS = -lm

OUT_DIR = out

all: $(OUT_DIR)/matrix_mul

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/matrix_mul: main.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

run: $(OUT_DIR)/matrix_mul
	@$(OUT_DIR)/matrix_mul

clean:
	rm -rf $(OUT_DIR)