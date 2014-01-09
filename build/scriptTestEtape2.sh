#!/bin/sh
#script creer pour lancer tous les programmes de tests
#pour l'étape 2 à la suite.

#recuperation de l'emplacement ou nous sommes
PWD=`pwd` 
#on rajoute notre dossier d'étape correspondant
directory=$PWD/etape2/ 
#on lance notre recherche pour avoir les noms des fichiers executable

 
#Pour chaque programme executable existant dans notre recherche

for files in $directory/`find -executable`
do
	#on affiche le nom du programme et on lance le programme
	echo ''
	echo '**********************************************'
	echo '* lancement du programme de test :' $files  '*'
	echo '**********************************************'
	echo ''	
	echo `./nachos-step2 -x $files`
	echo ''
done

