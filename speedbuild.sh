#!/bin/bash
set -x

# Compile every and each .cpp file available here - .c also...
CXXFLAGS=" -Iminibu -Isrc -Isrc/functions -Isrc/views -Isrc/conditions -Isrc/misc"
CXX="g++";
FLEX="flex";
BISON="bison";

# Run it.
$FLEX src/build.l
cd src; $BISON build.y; cd ..

find . -name "*.cpp" | while read srcfile; do
	if [ -f $srcfile.o ]; then continue; fi;
	$CXX $CXXFLAGS -c $srcfile -o $srcfile.o
	[ "$?" != "0" ] && exit 1;
done
find . -name "*.c" | while read srcfile; do
	$CXX $CXXFLAGS -c $srcfile -o $srcfile.o
	if [ $? != 0 ]; then exit $?; fi
done

# Gather all o's and put them together. Long, long command.
OFILES="$(find . -name '*.o' | sed 's/\n/ /g')"
$CXX -o build_speedbuild $OFILES
