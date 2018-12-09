#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./json_builder.h"

void json_object_init(struct json_object *obj){
	strcpy(obj->buffer,"{}");
}

void json_object_add_object(struct json_object *obj, char *key, char *value){
	char *p = obj->buffer;
	p[strlen(p)-1] = 0;
	if(strlen(p) > 2) p[strlen(p)-1] = ',';
	
	strcat(obj->buffer, " \"");
	strcat(obj->buffer,key);
	strcat(obj->buffer, "\":\"");
	strcat(obj->buffer,value);
	strcat(obj->buffer,"\" }");
}

void json_object_send(struct json_object *obj){
	//TODO: send object, from demonstration purposes, print to terminal

	fprintf(stdout, "%s\n",obj->buffer);
}
