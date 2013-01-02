all: src/Makefile
	$(MAKE) -C src $@

.DEFAULT: src/Makefile
	$(MAKE) -C src $@

