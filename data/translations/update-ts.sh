#!/bin/bash

LUPDATE=lupdate-qt5

if [ -z $1 ] ; then
	echo "Please specify target language"
	exit 1
fi

# ignore .ts if specified
_LANG=`echo $1 | cut -d'.' -f1`

if ! which $LUPDATE > /dev/null ; then
	LUPDATE=lupdate
fi

if ! which $LUPDATE > /dev/null ; then
	echo "Can't not find lupdate"
	exit 1
fi

$LUPDATE  ../../src -locations relative -target-language ${_LANG} -ts ${_LANG}.ts
