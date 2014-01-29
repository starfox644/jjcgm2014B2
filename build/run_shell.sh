#!/bin/bash

#file="/tmp/testNachOS_Shell"

#on teste si les executables sont presents
if [ ! -x ./nachos-myfilesys ]
then
	echo "./nachos-myfilesys manquant"
fi

if [ ! -x ./my_shell/my_shell ]
then
	echo "./my_shell/my_shell manquant"
fi

#on formate le disque
./nachos-myfilesys -f

#on copie my_shell
./nachos-myfilesys -cp ./my_shell/my_shell my_shell

#echo "cp ../filesys/test/small a" > $file
#echo "ls" >> $file
#echo "cat a" >> $file
#echo "quit" >> $file

#enfin on lance le shell
./nachos-myfilesys -sh #< $file


