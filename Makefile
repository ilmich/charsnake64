CFLAGS=-O
DEPS=

snake.prg: main.s main.o
	ld65 -t c64 -o snake.prg main.o c64.lib
charsnake64.d64: snake.prg
	cc1541 -q -f snake -w snake.prg -n charsnake64 charsnake64.d64
%.o: %.s
	ca65 -t c64 -o $@ $<
%.s: %.c
	cc65 $(CFLAGS) -t c64 -o $@ $<
clean:
	rm -f main.o main.s charsnake64.d64
test: charsnake64.d64
	x64 +warp charsnake64.d64