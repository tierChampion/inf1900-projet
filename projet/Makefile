PHONYS := all universal clean lib \
					app debug-app \
					line-test line-test-debug \
					distance-test distance-test-debug \
					corners-test corners-test-debug \
					menus-test menus-test-debug \
					moves-test moves-test-debug \
					pathfinding-test pathfinding-test-debug

.PHONY: $(PHONYS)

LIBDIR=lib
DEBUGFLAG=
UNIVERSALDIR=

all: lib

lib: 
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) $(DEBUGFLAG) -C $(LIBDIR)

universal: lib
	$(MAKE) -C $(UNIVERSALDIR) clean
	@echo -n "Is the B port free? [y/N] " && read ans && [ $${ans:-N} = y ]
	$(MAKE) $(DEBUGFLAG) -C $(UNIVERSALDIR) install

app: UNIVERSALDIR += app
app: universal

debug-app: DEBUGFLAG += debug
debug-app: app

line-test: UNIVERSALDIR += test/line-sensor
line-test: universal
line-test-debug: DEBUGFLAG += debug
line-test-debug: line-test

distance-test: UNIVERSALDIR += test/distance-sensor
distance-test: universal
distance-test-debug: DEBUGFLAG += debug
distance-test-debug: distance-test

corners-test: UNIVERSALDIR += test/corners
corners-test: universal
corners-test-debug: DEBUGFLAG += debug
corners-test-debug: corners-test

menus-test: UNIVERSALDIR += test/menus
menus-test: universal
menus-test-debug: DEBUGFLAG += debug
menus-test-debug: menus-test

moves-test: UNIVERSALDIR += test/movements
moves-test: universal
moves-test-debug: DEBUGFLAG += debug
moves-test-debug: moves-test

pathfinding-test: UNIVERSALDIR += test/pathfinding
pathfinding-test: universal
pathfinding-test-debug: DEBUGFLAG += debug
pathfinding-test-debug: pathfinding-test

clean:
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) -C app clean
	$(MAKE) -C test/line-sensor clean
	$(MAKE) -C test/distance-sensor clean
	$(MAKE) -C test/corners clean
	$(MAKE) -C test/menus clean
	$(MAKE) -C test/movements clean
	$(MAKE) -C test/pathfinding clean
