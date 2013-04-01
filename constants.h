#pragma once

/*
	Constants file definition
*/
char TITLE[6] = "title";
char BODY[5] = "body";
char ID[3] = "id";
char AD[3] = "ad";
char PDATE[6] = "pdate";
char PRICE[6] = "price";

FILE *terms = fopen("terms.txt","w+");
FILE *pdates = fopen("pdates.txt","w+");
FILE *prices = fopen("prices.txt","w+");
FILE *ads = fopen("ads.txt","w+");

