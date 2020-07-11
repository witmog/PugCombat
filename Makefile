CC=gcc
CFLAGS=-Icommon -Igame/source -Wall -g

windows: game/source/win32_main.c
	$(CC) game/source/win32_main.c $(CFLAGS) -luser32 -lGdi32 -lKernel32
linux: game/source/linux_main.c
	$(CC) game/source/linux_main.c $(CFLAGS) -lSDL2 -lm


.PHONY: clean run

# Note: these will have to be adjusted to work for Windows
run:
	make
	./a.out
clean:
	rm ./a.out
