#!/bin/bash

file="/tmp/NachOS_Commant.tmp"
readme="/tmp/NachOS_README.tmp"

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

echo "Hi, this is a README file" > $readme


echo "cp $readme README" > $file
echo "mkdir usr" >> $file
echo "cd usr" >> $file
echo "mkdir doc" >> $file
echo "mkdir lib" >> $file
echo "cd doc" >> $file
echo "cp ../filesys/test/small file" >> $file
echo "quit" >> $file

./nachos-myfilesys -sh < $file > /dev/null
./nachos-myfilesys -sh 

rm $file $readme

