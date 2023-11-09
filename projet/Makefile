.PHONY: all clean lib app debug debug-app

LIBDIR=lib
APPDIR=app
DEBUGFLAG=

all: lib

lib: 
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) $(DEBUGFLAG) -C $(LIBDIR)

app: lib
	$(MAKE) -C $(APPDIR) clean
	$(MAKE) $(DEBUGFLAG) -C $(APPDIR) install

clean:
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) -C $(APPDIR) clean

debug: DEBUGFLAG += debug
debug: all

debug-app: DEBUGFLAG += debug
debug-app: app
