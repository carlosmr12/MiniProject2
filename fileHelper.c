/*
	Function definitions of fileHelper.h
*/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "fileHelper.h"
#include "stringHelper.h"

bool writeTitleTerms(char *title, char *id, FILE* terms){
	char *term;

	term = strtok(title, " \n,!@#$%^&*+-=<’;\'\":>?/.");

	while(term != NULL){
		prepStr(term);
		prepStr(term);
		if(strlen(term) > 2 && strncmp(term, "&#", 2) != 0){
			fprintf(terms, "t-%s:%s\n",term, id);
		}
		term = strtok(NULL, " \n,!@#$%^&*’+-=\'\"<;:>?/.");
	}

	return true;
}

bool writeBodyTerms(char *body, char *id, FILE* terms){
	char *term;

	term = strtok(body, " \n,!@#$%^&*+-=<'’;:>\'\"?/.");

	while(term != NULL){
		prepStr(term);
		if(strlen(term) > 2 && strncmp(term, "&#", 2) != 0){
			fprintf(terms, "b-%s:%s\n",term, id);
		}
		term = strtok(NULL, " \n,!@#$%^&*+'’-=<;:>\'\"?/.");
	}

	return true;
}

bool writePdates(char *date, char *id, FILE* pdates){
	
	fprintf(pdates, "%s:%s\n",date, id);

	return true;
}

bool writePrices(char *price, char *id, FILE* prices){

	int auxp = atoi(price);
	fprintf(prices, "%8d:%s\n", auxp, id);

	return true;
}

bool writeAd(char *adRec, char *id, FILE*  ads){

	fprintf(ads, "%s:%s\n", id, adRec);

	return true;
}

bool closeFiles(FILE* terms, FILE* pdates, FILE* prices, FILE* ads){

	if(fclose(terms) == EOF){
		return false;
	}
	if(fclose(pdates) == EOF){
		return false;
	}
	if(fclose(prices) == EOF){
		return false;
	}
	if(fclose(ads) == EOF){
		return false;
	}

	return true;
	
}
