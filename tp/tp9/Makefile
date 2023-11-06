.PHONY: library interpreter loader clean

library: 
	$(MAKE) -C ../lib clean
	$(MAKE) -C ../lib

interpreter: 
	$(MAKE) -C interpreter clean
	$(MAKE) -C interpreter

loader: 
	$(MAKE) -C loader clean
	$(MAKE) -C loader

clean:
	$(MAKE) -C ../lib clean
	$(MAKE) -C interpreter clean
	$(MAKE) -C loader clean

debug: 
	$(MAKE) -C ../lib clean
	$(MAKE) debug -C ../lib
	$(MAKE) -C interpreter clean
	$(MAKE) debug -C interpreter
	$(MAKE) -C loader clean
	$(MAKE) debug -C loader
