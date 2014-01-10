#!/bin/sh
#script creer pour lancer tous les programmes de tests
#pour l'étape 2 à la suite.

#recuperation de l'emplacement ou nous sommes
PWD=`pwd` 
#on rajoute notre dossier d'étape correspondant
directory=$PWD/etape2/ 
#on lance notre recherche pour avoir les noms des fichiers executable
command=`find $directory -executable` 
#on efface les .o pour eviter des problèmes.
echo `rm $directory/*.o`
#Pour chaque programme executable existant dans notre recherche

for files in *$command*
do
	if [ '$files' != $directory ]
	then
		#on affiche le nom du programme et on lance le programme
		echo ''
		echo '***************************************************************************************'
		echo '* lancement du programme de test :' $files'*'
		echo '***************************************************************************************'
		echo ''	
		./nachos-step2 -x $files > /tmp/resultat_test_etape2
		cat /tmp/resultat_test_etape2
		echo ''
	fi
done



