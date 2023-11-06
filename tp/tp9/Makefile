.PHONY: all library interpreter loader clean debug debug-interpreter debug-loader

DEBUGFLAG=
INSTALLFLAG=

all: library

library: 
	$(MAKE) -C ../lib clean
	$(MAKE) $(DEBUGFLAG) -C ../lib

interpreter: 
	$(MAKE) -C interpreter clean
	$(MAKE) $(DEBUGFLAG) -C interpreter install

loader: 
	$(MAKE) -C loader clean
	$(MAKE) $(DEBUGFLAG) -C loader install

clean:
	$(MAKE) -C ../lib clean
	$(MAKE) -C interpreter clean
	$(MAKE) -C loader clean

debug: DEBUGFLAG += debug
debug: all

debug-interpreter: DEBUGFLAG += debug
debug-interpreter: library
debug-interpreter: interpreter

debug-loader: DEBUGFLAG += debug
debug-loader: library
debug-loader: interpreter
