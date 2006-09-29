LIBBU:=$(foreach f,paramproc.h paramproc.cpp staticstring.h staticstring.cpp exceptions.h exceptions.cpp serializable.h serializable.cpp exceptionbase.h exceptionbase.cpp plugger.h plugger.cpp singleton.h hashtable.cpp hashtable.h hashfunction.h hashfunction.cpp hashfunctionint.cpp hashfunctionint.h hashfunctionstring.cpp hashfunctionstring.h serializerbinary.cpp serializerbinary.h serializer.cpp serializer.h stringrep.h stringrep.cpp,src/$f)
SRC:=$(sort $(wildcard src/*.cpp) $(filter %.cpp,$(LIBBU)))
OBJ:=src/build.yy.o src/build.tab.o $(patsubst %.cpp,%.o,$(SRC)) 
REV:=$(shell svn info | grep "Revision" | cut -d\  -f2)
VER:=r$(REV)
PKG:=build-$(VER).tar.bz2

.PHONY: all extern clean depclean package
# .SILENT:

all: extern build

package: clean depclean extern $(PKG)

$(PKG):
	mkdir build-$(VER)
	mkdir build-$(VER)/src
	cp -a src/*.{cpp,h,y,l} build-$(VER)/src
	cp -a build.conf build-$(VER)
	cp -a Makefile.slim build-$(VER)/Makefile
	tar -c build-$(VER) | bzip2 -9 > $(PKG)
	rm -Rf build-$(VER)

clean:
	-rm -Rf build $(OBJ)

depclean:
	-rm -Rf $(LIBBU)

extern: $(LIBBU)

$(LIBBU):
	wget -nv -Psrc/ http://svn.xagasoft.com/libbu++/trunk/$@

build: src/build.yy.c src/build.tab.c $(OBJ)
	g++ -ggdb -ldl -o build $(OBJ)

%.o: %.cpp
	g++ -ggdb -Isrc -c -o $@ $<

%.o: %.c
	g++ -ggdb -Isrc -c -o $@ $<

%.yy.c: %.l
	flex --bison-bridge --bison-locations -o $@ $<

%.tab.c: %.y
	bison -v -b$(patsubst %.tab.c,%,$@) $<
