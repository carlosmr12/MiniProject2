#pragma once

/*
	Functions used to handle with the operations related
	to files
*/

bool writeTitleTerms(char *title, char *id, FILE *terms);
bool writeBodyTerms(char *body, char *id, FILE *terms);

bool writePdates(char *date, char *id, FILE *pdates);

bool writePrices(char *date, char *id, FILE *pdates);

bool writeAd(char *adRec, FILE*  ads);
