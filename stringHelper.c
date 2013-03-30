
/*
	Function definitions of stringHelper.h
*/
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "stringHelper.h"

void removeSpaces(char* str){
	char *write = str, *read = str;
	do while(isspace(*read)) 
		read++; 
	while((*write++ = *read++));
}

void prepStr(char *str){
	int j = 0;
	int size = (int)strlen(str);

	for(j=0; j < size; j++){
		str[j] = (char)tolower(str[j]);
	}
	removeSpaces(str);
}

