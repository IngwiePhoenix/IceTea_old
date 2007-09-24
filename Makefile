LIBBU:=$(foreach f,paramproc.h paramproc.cpp fstring.h fstring.cpp exceptions.h exceptions.cpp exceptionbase.cpp exceptionbase.h archive.cpp archive.h archival.cpp archival.h plugger.h plugger.cpp singleton.h hash.cpp hash.h file.h file.cpp stream.h stream.cpp list.h list.cpp set.h set.cpp,src/$f)
SRC:=$(sort $(wildcard src/*.cpp) $(patsubst src/%,src/bu/%,$(filter %.cpp,$(LIBBU))))
OBJ:=src/build.yy.o src/build.tab.o $(patsubst %.cpp,%.o,$(SRC)) 
REV:=$(shell svn info | grep "Revision" | cut -d\  -f2)
VER:=r$(REV)
PKG:=build-$(VER).tar.bz2

.PHONY: all extern clean distclean package
# .SILENT:

all: extern build

package: clean distclean extern $(PKG)

$(PKG):
	mkdir build-$(VER)
	mkdir build-$(VER)/src
	mkdir build-$(VER)/src/bu
	cp -a src/*.{cpp,h,y,l} build-$(VER)/src
	cp -a src/bu/*.{cpp,h} build-$(VER)/src/bu
	cp -a build.conf build-$(VER)
	cp -a Makefile.slim build-$(VER)/Makefile
	tar -c build-$(VER) | bzip2 -9 > $(PKG)
	rm -Rf build-$(VER)

clean:
	-rm -Rf build $(OBJ)

distclean:
	-rm -Rf $(LIBBU)
	-rm -Rf src/bu

extern: $(LIBBU)

$(LIBBU):
	mkdir -p src/bu
	wget -nv -Psrc/bu http://svn.xagasoft.com/libbu++/trunk/$@

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
