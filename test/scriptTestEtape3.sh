#!/bin/sh
#script creer pour lancer tous les programmes de tests
#pour l'étape 3 à la suite.

echo ********************************************
echo * Lancement des tests de la partie 3	*
echo ********************************************
echo 
#recuperation de l'emplacement ou nous somme
PWD=`pwd`
cd ..

#on rajoute notre dossier d'étape correspondant
directory=$PWD/build/etape2/ 

#on efface les .o pour eviter des problèmes.
`rm $directory*.o`
#on lance notre recherche pour avoir les noms des fichiers executable
command=`find $directory -executable` 

#Pour chaque programme executable existant dans notre recherche
cd build
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
                ./nachos-step3 -rs 0 -x  $files > /tmp/resultat_test_Partie3_etape2
                cat /tmp/resultat_test_Partie3_etape2
                echo ''
		read -p "Passer au test suivant ? [O/N]" rep
		case $rep in
			[Oo\n]* );;
			[Nn]* ) echo 'Au revoir!'; exit;;
		esac
        fi
done

###########################################################################################################################################################################
#on change de dossier pour faire les test de l'etape3

`rm $directory/*.o`
PWD2=`pwd`
directory=$PWD2/etape3/
#on efface les .o pour eviter des problèmes.
`rm $directory*.o`
#on lance notre recherche pour avoir les noms des fichiers executable
command=`find $directory -executable` 
#Pour chaque programme executable existant dans notre recherchecd build
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
                ./nachos-step3 -rs 0 -x  $files > /tmp/resultat_test_Partie3_etape3
                cat /tmp/resultat_test_Partie3_etape3
                echo ''
		read -p "Passer au test suivant ? [O/N]" rep
		case $rep in
			[Oo\n]* );;
			[Nn]* ) echo 'Au revoir!'; exit;;
		esac
        fi
done

echo 'Tests finis!'
