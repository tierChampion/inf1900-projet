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
interpreter: library

loader: 
	$(MAKE) -C loader clean
	$(MAKE) $(DEBUGFLAG) -C loader install
loader: library

clean:
	$(MAKE) -C ../lib clean
	$(MAKE) -C interpreter clean
	$(MAKE) -C loader clean

debug: DEBUGFLAG += debug
debug: all

debug-interpreter: DEBUGFLAG += debug
debug-interpreter: interpreter

debug-loader: DEBUGFLAG += debug
debug-loader: loader
