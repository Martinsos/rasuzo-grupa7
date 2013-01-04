#!/bin/bash

BACKGROUND="../../RASUZOslikeSmanjene/PC200038.jpg"

first=31
last=37

# ./a.out ../../RASUZOslikeSmanjene/PC200021.jpg $BACKGROUND ../../siluete/sil_PC200021.jpg

for (( i = $first; i <= $last; i++ ))
do
    suf=$i
    while [ ${#suf} -lt 5 ]
    do
	suf="0${suf}"
    done

    PIC="PC2$suf.jpg"
    SIL="sil_${PIC}"

    ./a.out ../../RASUZOslikeSmanjene/$PIC $BACKGROUND ../../siluete/$SIL

    echo $PIC
done
