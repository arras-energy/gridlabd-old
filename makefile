all:
	+test ! -f Makefile && autoreconf -isf && ./configure 
	+make -f Makefile $(MAKECMDGOALS)
