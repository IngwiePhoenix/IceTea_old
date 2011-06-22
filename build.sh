#!/bin/bash

BUSRC="stack.cpp string.cpp hash.cpp list.cpp trace.cpp stream.cpp formatter.cpp util.cpp sharedcore.cpp exceptionbase.cpp heap.cpp archivebase.cpp archive.cpp queue.cpp archival.cpp sio.cpp stdstream.cpp process.cpp plugger.cpp optparser.cpp signals.cpp array.cpp membuf.cpp file.cpp regex.cpp variant.cpp"
BUHDR="stack.h string.h hash.h list.h trace.h stream.h formatter.h util.h sharedcore.h exceptionbase.h heap.h archivebase.h archive.h queue.h archival.h sio.h stdstream.h process.h config.h compat/linux.h compat/win32.h compat/osx.h plugger.h singleton.h optparser.h array.h membuf.h file.h regex.h variant.h fmt.h extratypes.h"

function bld()
{
	OUTFILE="$1"
	if [ ! -e "$OUTFILE" ]; then
		return 1
	fi
	shift
	for INFILE in $*; do
		if [ "$INFILE" -nt "$OUTFILE" ]; then
			return 1
		fi
	done
	return 0
}

function cmd()
{
	printf "%8s $2\n" $1
	shift 2
	$* || exit
}

function gpp()
{
	bld "$1" "$2" || cmd CXX "$1" g++ -ggdb -fPIC -rdynamic -W -Wall -Iminibu -c -o "$1" "$2"
}

if [ ! -z "$1" ]; then
	if [ "$1" == "clean" -o "$1" == "-c" ]; then
		echo "Cleaning up, deleting all object code and downloaded source code..."
		echo 
		rm -Rf minibu src/*.o
		exit
	else
		echo "The only option supported is \"clean\", otherwise run $0"
		echo "with no parameters to compile build."
		echo
		exit
	fi
fi

for dir in minibu/src minibu/bu minibu/bu/compat; do
	cmd MKDIR ${dir} mkdir -p ${dir}
done
cmd FAKE minibu/bu/autoconfig.h touch minibu/bu/autoconfig.h
for file in $(cd bootstrap; ls); do
	cmd BOOTSTRAP minibu/bu/${file} cp bootstrap/${file} minibu/bu
done
for F in $BUSRC; do
	bld minibu/src/$F || cmd WGET minibu/src/$F wget -q http://svn.xagasoft.com/libbu++/trunk/src/$F -O minibu/src/$F
done
for F in $BUHDR; do
	bld minibu/bu/$F || cmd WGET minibu/bu/$F wget -q http://svn.xagasoft.com/libbu++/trunk/src/$F -O minibu/bu/$F
done

bld src/build.tab.c src/build.y || cmd BISON src/build.tab.c bison -bsrc/build src/build.y
bld src/build.yy.c src/build.l || cmd FLEX src/build.yy.c flex src/build.l

for F in $BUSRC; do
	OUTPUT=${F%.*}.o
	bld minibu/src/$OUTPUT || gpp minibu/src/$OUTPUT minibu/src/$F
done

for F in src/*.c src/*.cpp; do
	OUTPUT=${F%.*}.o
	gpp "$OUTPUT" "$F"
done
bld build src/*.o minibu/src/*.o || cmd LINK build g++ -fPIC -rdynamic -Wl,-export-dynamic -o build src/*.o minibu/src/*.o -ldl
bld build~ build || cmd CP build~ cp build build~
