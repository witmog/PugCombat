CC=gcc
CFLAGS=-I.
LIBS=-luser32 -lGdi32 -lKernel32

win32_main.obj: win32_main.c
	$(CC) -g -o PugCombat win32_main.c $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	DEL /Q *.obj *.exe
