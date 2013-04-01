/*
	System libraries
*/
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xmlreader.h>

/*
	Helpers and Extra libraries
*/
#include "constants.h"
#include "stringHelper.h"
#include "fileHelper.h"

void getIDByNode(xmlNode *node, char *id){

	xmlNode *aux = node;

	for(; aux; aux = aux->next){
		if(strcmp((char*)aux->name,ID) == 0){
			strcpy(id, (char*)aux->children->content);
			break;
		}
	}
}

void generateTXTFiles(xmlNode *node){

	char id[5];
	char adRec[500] = "";

	strcat(adRec, "<");
	strcat(adRec, AD);
	strcat(adRec, ">");

	getIDByNode(node, id);

	for(; node; node = node->next){

		if(node->children){

			strcat(adRec, "<");
			strcat(adRec, (char*) node->name);
			strcat(adRec, ">");

			strcat(adRec, (char*)node->children->content);

			if(strcmp((char*)node->name, TITLE) == 0){
				writeTitleTerms((char*)node->children->content, id, terms);	
			}
			else if(strcmp((char*)node->name, BODY) == 0){
				writeBodyTerms((char*)node->children->content, id, terms);	
			}
			else if(strcmp((char*)node->name, PDATE) == 0){
				writePdates((char*)node->children->content, id, pdates);	
			}
			else if(strcmp((char*)node->name, PRICE) == 0){
				writePrices((char*)node->children->content, id, prices);	
			}

			strcat(adRec, "</");
			strcat(adRec, (char*)node->name);
			strcat(adRec, ">");

		}

	}

	strcat(adRec, "</");
	strcat(adRec, AD);
	strcat(adRec, ">");
	
	writeAd(adRec, ads);

}

int main(int argc, char *argv[]){
	
	xmlDoc *document;
	xmlNode *root, *first_child, *node;

	char *xmlfilename;

	if(argc < 2){
		fprintf(stderr, "Usage: %s filename.xml terms.txt\n", argv[0]);
		return (10);
	}

	xmlfilename = argv[1];
	
	document = xmlReadFile(xmlfilename, NULL, XML_PARSE_RECOVER);
	root = xmlDocGetRootElement(document);
	
	first_child = root->children;
	
	int i = 0;

	for(node = first_child; node; node = node->next){

		if(node->type == 1){
			++i;
//			fprintf(stdout, "\n\n%d - \t <%s>\ttype:(%i)\n\n", i, node->name, node->type);
			generateTXTFiles(node->children);
		}
	}

	xmlFreeDoc(document);

	xmlCleanupParser();

	fprintf(stdout, "...!\n");

	return 0;
}
