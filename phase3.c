#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>

#define MAX_LEN 256

void addId(char **ids, char *value, int *idCount, int *idSize);
int findId(char **ids, char *value, int idCount);
void intersect(char **ids, int *idCount, int *idSize, char **values, int valuesCount);

int main(int argc, char *argv[]) {
	printf("Enter your query (enter 'E' to exit): ");

	char line[MAX_LEN];
	while (fgets(line, sizeof(line), stdin) > 0) {
		if (strcmp(line, "E\n") == 0) {
			break;
		}
		
		//Ids array
		char **ids;
		int idCount = 0;
		int idSize = 100;
		int i;
		ids = malloc(idSize * sizeof(char*));
		for(i = idCount; i < idSize; i++) {
			ids[i] = malloc(MAX_LEN * sizeof(char));
		}

		//Split the terms
		char *term;
		term = strtok(line, " <>\n");
		while (term != NULL) {
			DB *db;
			DBC* dbcp;
			DBT key, data;
			char id[MAX_LEN];
			char name[MAX_LEN];
			//Create the database object
			db_create(&db, NULL, 0);
			//Clear the key and data
			memset(&key, 0, sizeof(DBT)); 
			memset(&data, 0, sizeof(DBT));

			//Values array
			char **values;
			int valuesCount = 0;
			int valuesSize = 100;
			values = malloc(valuesSize * sizeof(char*));
			for(i = valuesCount; i < valuesSize; i++) {
				values[i] = malloc(MAX_LEN * sizeof(char));
			}

			//If price token
			if (strcmp(term, "price") == 0) {
				//Open the database
				db->open(db, NULL, "pr.idx", NULL, DB_BTREE, 0, 0664);
				//Create the cursor
				db->cursor(db, NULL, &dbcp, 0);

				//Get the type (less than or greather than) and the value
				char *type;
				char *value;
				type = strtok(NULL, " \n");
				value = strtok(NULL, " <>\n");

				//Set the key to the value specified
				sprintf(id, "%8s", value);
				key.data = id;
				key.size = strlen(id);

				int ret;
				if (strncmp(type, "<", 1) == 0) {
					ret = dbcp->c_get(dbcp, &key, &data, DB_FIRST);
					while (ret != DB_NOTFOUND) {
						//If greater than or equal, exit loop
						if (strncmp(key.data, id, key.size) >= 0) {
							break;
						}
						
						//Get id
						strncpy(name, data.data, data.size);
						name[data.size] = 0;						
						//Add id
						addId(values, name, &valuesCount, &valuesSize);
						
						ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
					} 
				}
				else if (strncmp(type, ">", 1) == 0) {					
					ret = dbcp->c_get(dbcp, &key, &data, DB_SET_RANGE);
					while (ret != DB_NOTFOUND) {
						if (strncmp(key.data, id, key.size) == 0) {
							ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
							continue;
						}
						//Get id
						strncpy(name, data.data, data.size);
						name[data.size] = 0;				
						//Add id
						addId(values, name, &valuesCount, &valuesSize);		

						ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
					} 
				}
			}
			else if (strcmp(term, "since") == 0) {
				//Open the database
				db->open(db, NULL, "da.idx", NULL, DB_BTREE, 0, 0664);
				//Create the cursor
				db->cursor(db, NULL, &dbcp, 0);

				//Get the date value
				char *date;
				date = strtok(NULL, " <>\n");

				//Set the key to the value specified
				sprintf(id, "%s", date);
				key.data = id;
				key.size = strlen(id);

				int ret;
				ret = dbcp->c_get(dbcp, &key, &data, DB_SET_RANGE);
				while (ret != DB_NOTFOUND) {
					//Get id
					strncpy(name, data.data, data.size);
					name[data.size] = 0;						
					//Add id
					addId(values, name, &valuesCount, &valuesSize);		
										
					ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
				} 
			}
			else if (strcmp(term, "until") == 0) {
				//Open the database
				db->open(db, NULL, "da.idx", NULL, DB_BTREE, 0, 0664);
				//Create the cursor
				db->cursor(db, NULL, &dbcp, 0);

				//Get the date value
				char *date;
				date = strtok(NULL, " <>\n");

				//Set the key to the value specified
				sprintf(id, "%s", date);
				key.data = id;
				key.size = strlen(id);

				int ret;
				ret = dbcp->c_get(dbcp, &key, &data, DB_FIRST);
				while (ret != DB_NOTFOUND) {
					//If greater than or equal, exit loop
					if (strncmp(key.data, id, key.size) > 0) {
						break;
					}
					
					//Get id
					strncpy(name, data.data, data.size);
					name[data.size] = 0;						
					//Add id
					addId(values, name, &valuesCount, &valuesSize);		
					
					ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
				} 
			}
			else {
				//Open the database
				db->open(db, NULL, "te.idx", NULL, DB_BTREE, 0, 0664);
				//Create the cursor
				db->cursor(db, NULL, &dbcp, 0);

				//Test if title or body, or both
				int both = 1;
				if (strncmp(term, "t-", 2) == 0 || strncmp(term, "b-", 2) == 0) {
					both = 0;
				}
				//Test if wildcard is at end
				int wild = 0;
				if (term[strlen(term) - 2] == '%') {
					wild = 1;
				}

				int ret;
				if (both) {
					//Set the key to the value specified
					sprintf(id, "b-%s", term);
					key.data = id;
					key.size = strlen(id) - ((wild) ? 1 : 0);
					
					ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_SET_RANGE : DB_SET);
					while (ret != DB_NOTFOUND) {
						//Get id
						strncpy(name, data.data, data.size);
						name[data.size] = 0;
						
						if (wild) {
							if (strncmp(key.data, term, strlen(term) - 2) == 0) {
								addId(values, name, &valuesCount, &valuesSize);		
							}
							else {
								ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_NEXT : DB_NEXT_DUP);
								continue;
							}
						}
						else {
							//Add id
							addId(values, name, &valuesCount, &valuesSize);		
						}
						
						ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_NEXT : DB_NEXT_DUP);
					}

					//Set the key to the value specified
					sprintf(id, "t-%s", term);
					key.data = id;
					key.size = strlen(id) - ((wild) ? 1 : 0);
					
					ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_SET_RANGE : DB_SET);
					while (ret != DB_NOTFOUND) {
						//Get id
						strncpy(name, data.data, data.size);
						name[data.size] = 0;
						
						if (wild) {
							if (strncmp(key.data, term, strlen(term) - 2) == 0) {
								addId(values, name, &valuesCount, &valuesSize);		
							}
							else {
								ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_NEXT : DB_NEXT_DUP);
								continue;
							}
						}
						else {
							//Add id
							addId(values, name, &valuesCount, &valuesSize);		
						}
						
						ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_NEXT : DB_NEXT_DUP);
					} 
				}
				else {
					//Set the key to the value specified
					sprintf(id, "%s", term);
					key.data = id;
					key.size = strlen(id) - ((wild) ? 1 : 0);

					ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_SET_RANGE : DB_SET);
					while (ret != DB_NOTFOUND) {
						//Get id
						strncpy(name, data.data, data.size);
						name[data.size] = 0;
						
						if (wild) {
							if (strncmp(key.data, term, strlen(term) - 2) == 0) {
								addId(values, name, &valuesCount, &valuesSize);		
							}
							else {
								ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_NEXT : DB_NEXT_DUP);
								continue;
							}
						}
						else {
							//Add id
							addId(values, name, &valuesCount, &valuesSize);		
						}
						
						ret = dbcp->c_get(dbcp, &key, &data, (wild) ? DB_NEXT : DB_NEXT_DUP);
					} 
				}
			}
			
			intersect(ids, &idCount, &idSize, values, valuesCount);
			for(i = 0; i < valuesSize; i++) {
				free(values[i]);
			}
			free(values);

			//Close the database and cursor
			dbcp->c_close(dbcp);
			db->close(db, 0);

			term = strtok(NULL, " <>\n");
		}

		for (i = 0; i < idCount; i++) {
			if (strncmp(ids[i], "\0", 1) != 0) {
				printf("%s\n", ids[i]);
			}
		}

		for(i = 0; i < idSize; i++) {
			free(ids[i]);
		}
		free(ids);

		printf("Enter your query (enter 'E' to exit): ");
	}

	return 0;
} 

void addId(char **ids, char *value, int *idCount, int *idSize) {
	if (*idCount == *idSize) {
		*idSize = *idSize * 2;

		ids = realloc(ids, *idSize * sizeof(char ));

		int i;
		for(i = *idCount; i < *idSize; i++) {
			ids[i] = malloc(MAX_LEN * sizeof(char));
		}
	}

	if (findId(ids, value, *idCount) == -1) {
		strncpy(ids[*idCount], value, MAX_LEN);
		*idCount = *idCount + 1;
	}
}

int findId(char **ids, char *value, int idCount) {
	int i;
	for (i = 0; i < idCount; i++) {
		if (strncmp(ids[i], value, strlen(value)) == 0) {
			return i;
		}
	}
	return -1;
}

void intersect(char **ids, int *idCount, int *idSize, char **values, int valuesCount) {
	int i;
	if (*idCount == 0) {
		for (i = 0; i < valuesCount; i++) {
			addId(ids, values[i], idCount, idSize);
		}
	}
	else {
		for (i = 0; i < *idCount; i++) {
			if (findId(values, ids[i], valuesCount) == -1) {
				strncpy(ids[i], "\0", MAX_LEN);
			}
		}				   
	}
}
