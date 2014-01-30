#!/bin/bash

file="/tmp/testNachOS_Shell"

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

echo ' mkdir files q' > $file
./nachos-myfilesys -mt < $file
echo ' mkdir pendu q' > $file
./nachos-myfilesys -mt < $file
./nachos-myfilesys -cp ../filesys/test/small files/small
./nachos-myfilesys -cp etape5/testFile files/test
rm $file

#copie du pendu
./nachos-myfilesys -cp ./etape5/pendu pendu/pendu
./nachos-myfilesys -cp ../test/etape5/dico.txt pendu/dico.txt

#echo "cp ../filesys/test/small a" > $file
#echo "ls" >> $file
#echo "cat a" >> $file
#echo "quit" >> $file

#enfin on lance le shell
#./nachos-myfilesys -sh #< $file


