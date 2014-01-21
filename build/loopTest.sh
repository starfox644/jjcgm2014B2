#!/bin/bash

# Ce script permet de tester un programme en boucle N fois sur N graines (graines de 1 à N)
# Pour l'utiliser il faut faire par exemple : ./loopTest.sh 10 etape4/waitPidAll
# Cela lancera le programme sur les graines de 1 à 10.

# Le 3e argument (facultatif) permet d'afficher uniquement les lignes contenant une certaine chaine,
# par exemple : ./loopTest.sh 10 etape4/waitPidAll segmentation
# Cela fera la meme chose mais en affichant uniquement les lignes contenant le mot "segmentation" (ne marche pas avec 0)


if [ -z "$1" ]
then
	echo "Usage : ./loopTest.sh nbLoop cheminTest (chaineATrouver-option)"
elif [ -z "$2" ]
then
	echo "Usage : ./loopTest.sh nbLoop cheminTest (chaineATrouver-option)"
elif [ -z "$3" ]
then
	exec
	(
		for i in `seq 1 $1`; do
			echo "************************************************************"
			echo "Lancement de $2 avec la graine $i/$1"
			echo "************************************************************"
			./nachos-step4 -rs $i -x $2
		done
	)
else
	exec
	(
		for i in `seq 1 $1`
			do ./nachos-step4 -rs $i -x $2
		done
	) | grep $3
fi
