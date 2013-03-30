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
	xmlNode *aux1;
	char text[3] = "id";

	for(aux1 = aux; aux; aux = aux->next){
		if(strcmp((char*)aux->name,text) == 0){
			strcpy(id, (char*)aux->children->content);
			break;
		}
	}
}

void printNodeChildren(xmlNode *node, FILE *terms){
	xmlNode *aux;
	char id[5];

	getIDByNode(node, id);

	for(aux = node; node; node = node->next){
		if(node->children){
			if(strcmp((char*)node->name, title) == 0){
				writeTitleTerms((char*)node->children->content, id, terms);	
			}
			else if(strcmp((char*)node->name, body) == 0){
				writeBodyTerms((char*)node->children->content, id, terms);	
			}
		}
	}
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
	FILE *terms = fopen("terms.txt","w+");
	
	document = xmlReadFile(xmlfilename, NULL, XML_PARSE_RECOVER);
	root = xmlDocGetRootElement(document);
	
	fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
	first_child = root->children;
	
	int i = 0;

	for(node = first_child; node; node = node->next){

		if(node->type == 1){
			++i;
			fprintf(stdout, "\n\n%d - \t <%s>\ttype:(%i)\n\n", i, node->name, node->type);
			printNodeChildren(node->children, terms);
		}
	}

	xmlFreeDoc(document);

	xmlCleanupParser();

	fprintf(stdout, "...!\n");

	return 0;
}
