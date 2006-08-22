.PHONY: all clean parser-header lexer-header build-header 

.SILENT: all clean parser-header src/build.tab.c src/build.tab.o parser lexer-header src/build.yy.c src/build.yy.o lexer build-header src/builder.o src/main.o src/perform.o src/performfactory.o src/functionregexp.o src/functioncommandtolist.o src/performcommand.o src/functionfilesin.o src/functionfactory.o src/function.o src/functiontostring.o src/viewer.o src/functiondirectoriesin.o build 

all: parser-header parser lexer-header lexer build-header build 

parser-header:
	echo "--- parser ---"

src/build.tab.c: src/build.y
	echo "   comp: src/build.tab.c"
	cd src; bison ../src/build.y; cd ..

src/build.tab.o: src/build.tab.c
	echo "   comp: src/build.tab.o"
	g++ -c src/build.tab.c -ggdb -Ilibbu++/src -Isrc -o src/build.tab.o

parser: src/build.tab.o
	echo " stupid: parser"
	

lexer-header:
	echo
	echo "--- lexer ---"

src/build.yy.c: src/build.l
	echo "   comp: src/build.yy.c"
	flex --bison-bridge --bison-locations -o src/build.yy.c src/build.l

src/build.yy.o: src/build.yy.c
	echo "   comp: src/build.yy.o"
	g++ -c src/build.yy.c -ggdb -Ilibbu++/src -Isrc -o src/build.yy.o

lexer: src/build.yy.o
	echo " stupid: lexer"
	

build-header:
	echo
	echo "--- build ---"

src/builder.o: src/builder.cpp
	echo "   comp: src/builder.o"
	g++ -c src/builder.cpp -ggdb -Ilibbu++/src -Isrc -o src/builder.o

src/main.o: src/main.cpp
	echo "   comp: src/main.o"
	g++ -c src/main.cpp -ggdb -Ilibbu++/src -Isrc -o src/main.o

src/perform.o: src/perform.cpp
	echo "   comp: src/perform.o"
	g++ -c src/perform.cpp -ggdb -Ilibbu++/src -Isrc -o src/perform.o

src/performfactory.o: src/performfactory.cpp
	echo "   comp: src/performfactory.o"
	g++ -c src/performfactory.cpp -ggdb -Ilibbu++/src -Isrc -o src/performfactory.o

src/functionregexp.o: src/functionregexp.cpp
	echo "   comp: src/functionregexp.o"
	g++ -c src/functionregexp.cpp -ggdb -Ilibbu++/src -Isrc -o src/functionregexp.o

src/functioncommandtolist.o: src/functioncommandtolist.cpp
	echo "   comp: src/functioncommandtolist.o"
	g++ -c src/functioncommandtolist.cpp -ggdb -Ilibbu++/src -Isrc -o src/functioncommandtolist.o

src/performcommand.o: src/performcommand.cpp
	echo "   comp: src/performcommand.o"
	g++ -c src/performcommand.cpp -ggdb -Ilibbu++/src -Isrc -o src/performcommand.o

src/functionfilesin.o: src/functionfilesin.cpp
	echo "   comp: src/functionfilesin.o"
	g++ -c src/functionfilesin.cpp -ggdb -Ilibbu++/src -Isrc -o src/functionfilesin.o

src/functionfactory.o: src/functionfactory.cpp
	echo "   comp: src/functionfactory.o"
	g++ -c src/functionfactory.cpp -ggdb -Ilibbu++/src -Isrc -o src/functionfactory.o

src/function.o: src/function.cpp
	echo "   comp: src/function.o"
	g++ -c src/function.cpp -ggdb -Ilibbu++/src -Isrc -o src/function.o

src/functiontostring.o: src/functiontostring.cpp
	echo "   comp: src/functiontostring.o"
	g++ -c src/functiontostring.cpp -ggdb -Ilibbu++/src -Isrc -o src/functiontostring.o

src/viewer.o: src/viewer.cpp
	echo "   comp: src/viewer.o"
	g++ -c src/viewer.cpp -ggdb -Ilibbu++/src -Isrc -o src/viewer.o

src/functiondirectoriesin.o: src/functiondirectoriesin.cpp
	echo "   comp: src/functiondirectoriesin.o"
	g++ -c src/functiondirectoriesin.cpp -ggdb -Ilibbu++/src -Isrc -o src/functiondirectoriesin.o

build: src/functionfactory.o src/functiontostring.o src/perform.o src/functiondirectoriesin.o src/viewer.o src/main.o src/performcommand.o src/functionregexp.o src/builder.o src/build.yy.o src/functionfilesin.o src/function.o src/performfactory.o src/functioncommandtolist.o src/build.tab.o
	echo "    exe: build"
	g++ src/functionfactory.o src/functiontostring.o src/perform.o src/functiondirectoriesin.o src/viewer.o src/main.o src/performcommand.o src/functionregexp.o src/builder.o src/build.yy.o src/functionfilesin.o src/function.o src/performfactory.o src/functioncommandtolist.o src/build.tab.o -Llibbu++ -lbu++ -ldl -o build

clean:
	-if [ -f src/build.tab.c ] ; then echo "     rm: src/build.tab.c" ; rm src/build.tab.c ; fi
	-if [ -f src/build.tab.o ] ; then echo "     rm: src/build.tab.o" ; rm src/build.tab.o ; fi
	-if [ -f parser ] ; then echo "     rm: parser" ; rm parser ; fi
	-if [ -f src/build.yy.c ] ; then echo "     rm: src/build.yy.c" ; rm src/build.yy.c ; fi
	-if [ -f src/build.yy.o ] ; then echo "     rm: src/build.yy.o" ; rm src/build.yy.o ; fi
	-if [ -f lexer ] ; then echo "     rm: lexer" ; rm lexer ; fi
	-if [ -f src/builder.o ] ; then echo "     rm: src/builder.o" ; rm src/builder.o ; fi
	-if [ -f src/main.o ] ; then echo "     rm: src/main.o" ; rm src/main.o ; fi
	-if [ -f src/perform.o ] ; then echo "     rm: src/perform.o" ; rm src/perform.o ; fi
	-if [ -f src/performfactory.o ] ; then echo "     rm: src/performfactory.o" ; rm src/performfactory.o ; fi
	-if [ -f src/functionregexp.o ] ; then echo "     rm: src/functionregexp.o" ; rm src/functionregexp.o ; fi
	-if [ -f src/functioncommandtolist.o ] ; then echo "     rm: src/functioncommandtolist.o" ; rm src/functioncommandtolist.o ; fi
	-if [ -f src/performcommand.o ] ; then echo "     rm: src/performcommand.o" ; rm src/performcommand.o ; fi
	-if [ -f src/functionfilesin.o ] ; then echo "     rm: src/functionfilesin.o" ; rm src/functionfilesin.o ; fi
	-if [ -f src/functionfactory.o ] ; then echo "     rm: src/functionfactory.o" ; rm src/functionfactory.o ; fi
	-if [ -f src/function.o ] ; then echo "     rm: src/function.o" ; rm src/function.o ; fi
	-if [ -f src/functiontostring.o ] ; then echo "     rm: src/functiontostring.o" ; rm src/functiontostring.o ; fi
	-if [ -f src/viewer.o ] ; then echo "     rm: src/viewer.o" ; rm src/viewer.o ; fi
	-if [ -f src/functiondirectoriesin.o ] ; then echo "     rm: src/functiondirectoriesin.o" ; rm src/functiondirectoriesin.o ; fi
	-if [ -f build ] ; then echo "     rm: build" ; rm build ; fi

