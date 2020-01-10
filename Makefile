all: win

win:
	+$(MAKE) -f Makefile.win32


.PHONY: clean
clean:
	+$(MAKE) clean -f Makefile.win32
