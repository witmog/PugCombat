CC=gcc
CFLAGS=-I.
LIBS=-luser32

win32_main.obj: win32_main.c
	$(CC) -o PugCombat win32_main.c $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	DEL /Q *.obj *.exe
