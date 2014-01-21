/*
 * userTestTouche.C
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"

int main(){
	int kb;
	while((char)kb != 'q'){
		kb=GetChar();
		if(kb == 65){
			PutString("Vous avez appuyé sur la fleche du haut\n");
		}else if(kb == 66){
			PutString("Vous avez appuyé sur la fleche du bas\n");
		}else if(kb == 67){
			PutString("Vous avez appuyé sur la fleche de droite\n");
		}else if(kb == 68){
					PutString("Vous avez appuyé sur la fleche de gauche\n");
		}

		PutString("valeur touche :");
		PutInt(kb);
		PutString("\t");
		PutInt((int)kb);
		PutString("\n");
	}

	return 0;
}



