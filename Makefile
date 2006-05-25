SRC:=$(wildcard src/*.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SRC))
VER:=r$(shell svn info | grep "Revision" | cut -d\  -f2)
LIBS:=bu++
LIBDIRS:=$(patsubst %,lib%,$(LIBS))
LIBFILES:=$(foreach dir,$(LIBDIRS),$(dir)/$(dir).a)
DEPFILES:=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all clean dist checklibs

all: checklibs src/version.h build

checklibs:
	for ldir in $(LIBDIRS); do make -C $$ldir; done

-include $(DEPFILES)

src/version.h: .svn
	echo "#define SVNREV \"$(VER)\"" > src/version.h

%.o: %.cpp %.d
	g++ -ggdb $(CXXFLAGS) -Isrc $(patsubst %,-I%/src,$(LIBDIRS)) -c -o $@ $<

clean:
	-rm $(foreach dir,src,$(wildcard $(dir)/*.o)) build src/version.h

depclean:
	-rm $(foreach dir,src src/lib $(TOOLDIRS),$(wildcard $(dir)/*.d))

%.d: %.cpp
	g++ -Isrc -Isrc/lib $(CXXFLAGS) $(patsubst %,-I%/src,$(LIBDIRS)) -M $(CPPFLAGS) $< | sed 's,\($(patsubst src/%,%,$*)\)\.o[:]*,src/\1.o $@:,g' > $@

$(LIBFILES):
	make -C $(dir $@) $(notdir $@)

test:
	-build

build: $(LIBFILES) $(OBJS)
	g++ -ggdb $(OBJS) -o build $(patsubst %,-L%,$(LIBDIRS)) $(patsubst %,-l%,$(LIBS))

api: $(LIBSRC) $(TOOLSRC)
	sed 's/PROJECT_NUMBER.*/PROJECT_NUMBER         = $(VER)/' < Doxyfile > Doxyfile.tmp
	doxygen Doxyfile.tmp
	rm Doxyfile.tmp
	make -C api/latex

dist: clean src/version.h
	mkdir build-$(VER)
	cp -a --target-directory=build-$(VER) Makefile src
	tar --exclude=\.svn -c build-$(VER) | bzip2 -9 > build-$(VER).tar.bz2
	rm -Rf build-$(VER)
	-for dir in $(LIBDIRS); do make -C $$dir dist; done

