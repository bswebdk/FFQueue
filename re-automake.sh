#!/bin/bash
# Part of FFQueue: Used to re-create the automake files

function remove() {
  if [ -d $1 ]; then
    echo "Removing directory $1"
    rm -r $1
  elif [ -f $1 ]; then
    echo "Removing file $1"
    rm $1
  fi
}

if [ -z "$(which autoreconf)" ]; then
  echo Command "autoreconf" is not available, please install package "automake" to fix it.
  echo Nothing possible, exiting script..
  exit
fi

remove autom4te.cache
remove aclocal.m4
remove compile
remove configure
remove depcomp
remove install-sh
remove Makefile.in
remove missing
remove src/Makefile.in

autoreconf --install
