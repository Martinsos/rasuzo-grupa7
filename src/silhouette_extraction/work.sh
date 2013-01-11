#!/bin/bash

BACKGROUND=$1 # path to background picture

first=$2 # number of first picture (for example: 25)
last=$3 # number of alst picture (for example: 45)

for (( i = $first; i <= $last; i++ ))
do
    suf=$i
    while [ ${#suf} -lt 5 ]
    do
	suf="0${suf}"
    done

    PIC="PC2$suf.jpg"
    SIL="sil_${PIC}"

    ./silhouetteExtraction ../../slike/$PIC $BACKGROUND ../../siluete/$SIL

    echo $PIC
done
