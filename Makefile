CC=gcc
CFLAGS=-Icommon -Igame/source -Wall -g
EXEC=PugCombat

all: windows

windows: game/source/win32_main.c 
	@$(CC) -o $(EXEC) game/source/win32_main.c $(CFLAGS) -luser32 -lGdi32 -lKernel32
.PHONY: clean
clean:
	rm *.exe
	
