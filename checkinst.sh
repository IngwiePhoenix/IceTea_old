#!/bin/bash

CMD="ln -svf $PWD/build /usr/bin; ln -svf $PWD/bfilt-c++ /usr/bin"

if [ $UID == 0 ]; then
	bash -c "$CMD"
else
	echo This script needs root access...
	su root -c "$CMD"
fi
