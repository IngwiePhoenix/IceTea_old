.PHONY: all clean parser-header lexer-header build-header 

.SILENT: all clean parser-header src/build.tab.c src/build.tab.o parser lexer-header src/build.yy.c src/build.yy.o lexer build-header src/viewerpercent.o src/targetfile.o src/functioncommandtolist.o src/cache.o src/targetfactory.o src/performcommand.o src/target.o src/viewerfactory.o src/regexp.o src/rule.o src/functiontargets.o src/functiontostring.o src/viewermake.o src/stringprocbuild.o src/functionfilesin.o src/performfactory.o src/build.o src/functionfactory.o src/action.o src/function.o src/functiondirectoriesin.o src/main.o src/functionregexp.o src/parser.o src/stringproc.o src/buildparser.o src/perform.o src/viewerplain.o src/viewer.o build 

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

src/viewerpercent.o: src/viewerpercent.cpp
	echo "   comp: src/viewerpercent.o"
	g++ -c src/viewerpercent.cpp -ggdb -Ilibbu++/src -Isrc -o src/viewerpercent.o

src/targetfile.o: src/targetfile.cpp
	echo "   comp: src/targetfile.o"
	g++ -c src/targetfile.cpp -ggdb -Ilibbu++/src -Isrc -o src/targetfile.o

src/functioncommandtolist.o: src/functioncommandtolist.cpp
	echo "   comp: src/functioncommandtolist.o"
	g++ -c src/functioncommandtolist.cpp -ggdb -Ilibbu++/src -Isrc -o src/functioncommandtolist.o

src/cache.o: src/cache.cpp
	echo "   comp: src/cache.o"
	g++ -c src/cache.cpp -ggdb -Ilibbu++/src -Isrc -o src/cache.o

src/targetfactory.o: src/targetfactory.cpp
	echo "   comp: src/targetfactory.o"
	g++ -c src/targetfactory.cpp -ggdb -Ilibbu++/src -Isrc -o src/targetfactory.o

src/performcommand.o: src/performcommand.cpp
	echo "   comp: src/performcommand.o"
	g++ -c src/performcommand.cpp -ggdb -Ilibbu++/src -Isrc -o src/performcommand.o

src/target.o: src/target.cpp
	echo "   comp: src/target.o"
	g++ -c src/target.cpp -ggdb -Ilibbu++/src -Isrc -o src/target.o

src/viewerfactory.o: src/viewerfactory.cpp
	echo "   comp: src/viewerfactory.o"
	g++ -c src/viewerfactory.cpp -ggdb -Ilibbu++/src -Isrc -o src/viewerfactory.o

src/regexp.o: src/regexp.cpp
	echo "   comp: src/regexp.o"
	g++ -c src/regexp.cpp -ggdb -Ilibbu++/src -Isrc -o src/regexp.o

src/rule.o: src/rule.cpp
	echo "   comp: src/rule.o"
	g++ -c src/rule.cpp -ggdb -Ilibbu++/src -Isrc -o src/rule.o

src/functiontargets.o: src/functiontargets.cpp
	echo "   comp: src/functiontargets.o"
	g++ -c src/functiontargets.cpp -ggdb -Ilibbu++/src -Isrc -o src/functiontargets.o

src/functiontostring.o: src/functiontostring.cpp
	echo "   comp: src/functiontostring.o"
	g++ -c src/functiontostring.cpp -ggdb -Ilibbu++/src -Isrc -o src/functiontostring.o

src/viewermake.o: src/viewermake.cpp
	echo "   comp: src/viewermake.o"
	g++ -c src/viewermake.cpp -ggdb -Ilibbu++/src -Isrc -o src/viewermake.o

src/stringprocbuild.o: src/stringprocbuild.cpp
	echo "   comp: src/stringprocbuild.o"
	g++ -c src/stringprocbuild.cpp -ggdb -Ilibbu++/src -Isrc -o src/stringprocbuild.o

src/functionfilesin.o: src/functionfilesin.cpp
	echo "   comp: src/functionfilesin.o"
	g++ -c src/functionfilesin.cpp -ggdb -Ilibbu++/src -Isrc -o src/functionfilesin.o

src/performfactory.o: src/performfactory.cpp
	echo "   comp: src/performfactory.o"
	g++ -c src/performfactory.cpp -ggdb -Ilibbu++/src -Isrc -o src/performfactory.o

src/build.o: src/build.cpp
	echo "   comp: src/build.o"
	g++ -c src/build.cpp -ggdb -Ilibbu++/src -Isrc -o src/build.o

src/functionfactory.o: src/functionfactory.cpp
	echo "   comp: src/functionfactory.o"
	g++ -c src/functionfactory.cpp -ggdb -Ilibbu++/src -Isrc -o src/functionfactory.o

src/action.o: src/action.cpp
	echo "   comp: src/action.o"
	g++ -c src/action.cpp -ggdb -Ilibbu++/src -Isrc -o src/action.o

src/function.o: src/function.cpp
	echo "   comp: src/function.o"
	g++ -c src/function.cpp -ggdb -Ilibbu++/src -Isrc -o src/function.o

src/functiondirectoriesin.o: src/functiondirectoriesin.cpp
	echo "   comp: src/functiondirectoriesin.o"
	g++ -c src/functiondirectoriesin.cpp -ggdb -Ilibbu++/src -Isrc -o src/functiondirectoriesin.o

src/main.o: src/main.cpp
	echo "   comp: src/main.o"
	g++ -c src/main.cpp -ggdb -Ilibbu++/src -Isrc -o src/main.o

src/functionregexp.o: src/functionregexp.cpp
	echo "   comp: src/functionregexp.o"
	g++ -c src/functionregexp.cpp -ggdb -Ilibbu++/src -Isrc -o src/functionregexp.o

src/parser.o: src/parser.cpp
	echo "   comp: src/parser.o"
	g++ -c src/parser.cpp -ggdb -Ilibbu++/src -Isrc -o src/parser.o

src/stringproc.o: src/stringproc.cpp
	echo "   comp: src/stringproc.o"
	g++ -c src/stringproc.cpp -ggdb -Ilibbu++/src -Isrc -o src/stringproc.o

src/buildparser.o: src/buildparser.cpp
	echo "   comp: src/buildparser.o"
	g++ -c src/buildparser.cpp -ggdb -Ilibbu++/src -Isrc -o src/buildparser.o

src/perform.o: src/perform.cpp
	echo "   comp: src/perform.o"
	g++ -c src/perform.cpp -ggdb -Ilibbu++/src -Isrc -o src/perform.o

src/viewerplain.o: src/viewerplain.cpp
	echo "   comp: src/viewerplain.o"
	g++ -c src/viewerplain.cpp -ggdb -Ilibbu++/src -Isrc -o src/viewerplain.o

src/viewer.o: src/viewer.cpp
	echo "   comp: src/viewer.o"
	g++ -c src/viewer.cpp -ggdb -Ilibbu++/src -Isrc -o src/viewer.o

build: src/functionfactory.o src/regexp.o src/viewerpercent.o src/targetfactory.o src/buildparser.o src/functiontargets.o src/stringprocbuild.o src/rule.o src/functioncommandtolist.o src/functiontostring.o src/action.o src/viewer.o src/main.o src/viewerfactory.o src/target.o src/performfactory.o src/build.o src/parser.o src/perform.o src/functiondirectoriesin.o src/performcommand.o src/function.o src/build.yy.o src/build.tab.o src/cache.o src/viewermake.o src/functionregexp.o src/functionfilesin.o src/viewerplain.o src/stringproc.o src/targetfile.o
	echo "    exe: build"
	g++ src/functionfactory.o src/regexp.o src/viewerpercent.o src/targetfactory.o src/buildparser.o src/functiontargets.o src/stringprocbuild.o src/rule.o src/functioncommandtolist.o src/functiontostring.o src/action.o src/viewer.o src/main.o src/viewerfactory.o src/target.o src/performfactory.o src/build.o src/parser.o src/perform.o src/functiondirectoriesin.o src/performcommand.o src/function.o src/build.yy.o src/build.tab.o src/cache.o src/viewermake.o src/functionregexp.o src/functionfilesin.o src/viewerplain.o src/stringproc.o src/targetfile.o -Llibbu++ -lbu++ -ldl -o build

clean:
	-if [ -f src/build.tab.c ] ; then echo "     rm: src/build.tab.c" ; rm src/build.tab.c ; fi
	-if [ -f src/build.tab.o ] ; then echo "     rm: src/build.tab.o" ; rm src/build.tab.o ; fi
	-if [ -f parser ] ; then echo "     rm: parser" ; rm parser ; fi
	-if [ -f src/build.yy.c ] ; then echo "     rm: src/build.yy.c" ; rm src/build.yy.c ; fi
	-if [ -f src/build.yy.o ] ; then echo "     rm: src/build.yy.o" ; rm src/build.yy.o ; fi
	-if [ -f lexer ] ; then echo "     rm: lexer" ; rm lexer ; fi
	-if [ -f src/viewerpercent.o ] ; then echo "     rm: src/viewerpercent.o" ; rm src/viewerpercent.o ; fi
	-if [ -f src/targetfile.o ] ; then echo "     rm: src/targetfile.o" ; rm src/targetfile.o ; fi
	-if [ -f src/functioncommandtolist.o ] ; then echo "     rm: src/functioncommandtolist.o" ; rm src/functioncommandtolist.o ; fi
	-if [ -f src/cache.o ] ; then echo "     rm: src/cache.o" ; rm src/cache.o ; fi
	-if [ -f src/targetfactory.o ] ; then echo "     rm: src/targetfactory.o" ; rm src/targetfactory.o ; fi
	-if [ -f src/performcommand.o ] ; then echo "     rm: src/performcommand.o" ; rm src/performcommand.o ; fi
	-if [ -f src/target.o ] ; then echo "     rm: src/target.o" ; rm src/target.o ; fi
	-if [ -f src/viewerfactory.o ] ; then echo "     rm: src/viewerfactory.o" ; rm src/viewerfactory.o ; fi
	-if [ -f src/regexp.o ] ; then echo "     rm: src/regexp.o" ; rm src/regexp.o ; fi
	-if [ -f src/rule.o ] ; then echo "     rm: src/rule.o" ; rm src/rule.o ; fi
	-if [ -f src/functiontargets.o ] ; then echo "     rm: src/functiontargets.o" ; rm src/functiontargets.o ; fi
	-if [ -f src/functiontostring.o ] ; then echo "     rm: src/functiontostring.o" ; rm src/functiontostring.o ; fi
	-if [ -f src/viewermake.o ] ; then echo "     rm: src/viewermake.o" ; rm src/viewermake.o ; fi
	-if [ -f src/stringprocbuild.o ] ; then echo "     rm: src/stringprocbuild.o" ; rm src/stringprocbuild.o ; fi
	-if [ -f src/functionfilesin.o ] ; then echo "     rm: src/functionfilesin.o" ; rm src/functionfilesin.o ; fi
	-if [ -f src/performfactory.o ] ; then echo "     rm: src/performfactory.o" ; rm src/performfactory.o ; fi
	-if [ -f src/build.o ] ; then echo "     rm: src/build.o" ; rm src/build.o ; fi
	-if [ -f src/functionfactory.o ] ; then echo "     rm: src/functionfactory.o" ; rm src/functionfactory.o ; fi
	-if [ -f src/action.o ] ; then echo "     rm: src/action.o" ; rm src/action.o ; fi
	-if [ -f src/function.o ] ; then echo "     rm: src/function.o" ; rm src/function.o ; fi
	-if [ -f src/functiondirectoriesin.o ] ; then echo "     rm: src/functiondirectoriesin.o" ; rm src/functiondirectoriesin.o ; fi
	-if [ -f src/main.o ] ; then echo "     rm: src/main.o" ; rm src/main.o ; fi
	-if [ -f src/functionregexp.o ] ; then echo "     rm: src/functionregexp.o" ; rm src/functionregexp.o ; fi
	-if [ -f src/parser.o ] ; then echo "     rm: src/parser.o" ; rm src/parser.o ; fi
	-if [ -f src/stringproc.o ] ; then echo "     rm: src/stringproc.o" ; rm src/stringproc.o ; fi
	-if [ -f src/buildparser.o ] ; then echo "     rm: src/buildparser.o" ; rm src/buildparser.o ; fi
	-if [ -f src/perform.o ] ; then echo "     rm: src/perform.o" ; rm src/perform.o ; fi
	-if [ -f src/viewerplain.o ] ; then echo "     rm: src/viewerplain.o" ; rm src/viewerplain.o ; fi
	-if [ -f src/viewer.o ] ; then echo "     rm: src/viewer.o" ; rm src/viewer.o ; fi
	-if [ -f build ] ; then echo "     rm: build" ; rm build ; fi

