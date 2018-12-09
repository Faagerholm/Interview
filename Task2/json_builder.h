

#define JSON_BUFFER_SIZE 256

struct json_object{
	char buffer[JSON_BUFFER_SIZE];
};

void json_object_init(struct json_object *obj);
void json_object_add_object(struct json_object *obj, char *key, char *value);
void json_object_send(struct json_object *obj);
