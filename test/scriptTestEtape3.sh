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

#programme que l'on veut tester pour les test
PROG=$PWD/build/nachos-step3

#on rajoute notre dossier d'étape correspondant
directory=$PWD/build/etape2/ 

#on lance notre recherche pour avoir les noms des fichiers executable
command=`find $directory -executable` 
#on efface les .o pour eviter des problèmes.
`rm $directory/*.o`
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
                .$PROG -x -rs 0 $files > /tmp/resultat_test_Partie3_etape2
                cat /tmp/resultat_test_Partie3_etape2
                echo ''
        fi
done

###########################################################################################################################################################################
#on change de dossier pour faire les test de l'etape3

cd ..
PWD2=`pwd`
directory=$PWD/etape3/

#on lance notre recherche pour avoir les noms des fichiers executable
command=`find $directory -executable` 
#on efface les .o pour eviter des problèmes.
`rm $directory/*.o`
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
                .$PROG -x -rs 0 $files > /tmp/resultat_test_Partie3_etape3
                cat /tmp/resultat_test_Partie3_etape3
                echo ''
        fi
done

