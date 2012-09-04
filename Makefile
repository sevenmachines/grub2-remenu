#~/bin/sh

SRCDIR=Release
all:
	cd ${SRCDIR}; make; cd ..

clean:
	cd ${SRCDIR}; make clean; cd ..
	
.PHONY: all clean
