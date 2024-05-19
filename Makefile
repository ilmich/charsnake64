TARGET ?= c64
CFLAGS = -Osr -Cl
DEPS =

snake.prg: main.s main.o
	ld65 -t $(TARGET) -o snake.prg main.o $(TARGET).lib
d64: snake.prg
	cc1541 -q -f snake -w snake.prg -n charsnake64 charsnake64.d64
%.o: %.s
	ca65 -t $(TARGET) -o $@ $<
%.s: %.c game.h
	cc65 $(CFLAGS) --add-source -t $(TARGET) -o $@ $<
clean:
	rm -f main.o main.s charsnake64.d64
test: d64
	x64 +warp charsnake64.d64