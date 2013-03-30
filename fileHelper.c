/*
	Function definitions of fileHelper.h
*/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "fileHelper.h"
#include "stringHelper.h"

bool writeTitleTerms(char *title, char *id, FILE *terms){
	char *term;

	term = strtok(title, " \n,!@#$%^&*+-=<’;\'\":>?/.");

	while(term != NULL){
		prepStr(term);
		prepStr(term);
		if(strlen(term) > 2){
			fprintf(stdout, "t-%s:%s\n",term, id);
			fprintf(terms, "t-%s:%s\n",term, id);
		}
		term = strtok(NULL, " \n,!@#$%^&*’+-=\'\"<;:>?/.");
	}

	return true;
}

bool writeBodyTerms(char *body, char *id, FILE *terms){
	char *term;

	term = strtok(body, " \n,!@#$%^&*+-=<'’;:>\'\"?/.");

	while(term != NULL){
		prepStr(term);
		if(strlen(term) > 2){
			fprintf(stdout, "b-%s:%s\n",term, id);
			fprintf(terms, "b-%s:%s\n",term, id);
		}
		term = strtok(NULL, " \n,!@#$%^&*+'’-=<;:>\'\"?/.");
	}

	return true;
}
