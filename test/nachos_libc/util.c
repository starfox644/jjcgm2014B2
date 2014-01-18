/*
 * util.c
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */
#include "util.h"

int Atoi(char* variable){
	int num = 0,flag = 0,i;
	for(i=0;i<=StrLengh(variable);i++)
	{
	    if(variable[i] >= '0' && variable[i] <= '9'){
	    	num = num * 10 + variable[i] -'0';

	    }else if(variable[0] == '-' && i==0){
	        flag =1;
	    }else{
	        break;
	    }
	}
	if(flag == 1){
		num = num * -1;
	}

	return num;

}
/* reverse:  reverse string s in place */
 char* reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = StrLengh(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
     return s;
 }
char* Itoa(int variable){
	 	 int i,sign;
	 	 char *s = 0;
	     if ((sign = variable) < 0)  /* record sign */
	         variable = -variable;          /* make n positive */
	     i = 0;
	     do {       /* generate digits in reverse order */
	         s[i++] = variable % 10 + '0';   /* get next digit */
	     } while ((variable /= 10) > 0);     /* delete it */
	     if (sign < 0)
	         s[i++] = '-';
	     s[i] = '\0';
	     s = reverse(s);
	     return s;
}


