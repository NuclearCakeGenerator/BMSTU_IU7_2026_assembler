CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -mavx2
LDLIBS = -lm

OUT_DIR = out

all: run

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

app.exe: main.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

run: app.exe
	@./app.exe

clean:
	rm -rf $(OUT_DIR)