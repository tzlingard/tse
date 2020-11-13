#!/bin/bash

#no arguments needed
if [ $# -gt 0 ]
then
		echo "usage: valgrind_list.sh"
		exit 1
fi

make clean
make all
valgrind --leak-check=full ./crawler "https://thayer.github.io/engs50/" "../pages" 1 2

