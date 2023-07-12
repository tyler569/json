#include <stdio.h>

#include "json.h"

int main() {
    struct json_value *object = json_new_object();
    json_add_null_to_object(object, "null");
    json_add_bool_to_object(object, "bool", true);
    json_add_number_to_object(object, "number", 42);
    json_add_string_to_object(object, "string", "Hello, World!");

    struct json_value *array = json_new_array();
    json_add_bool_to_array(array, true);
    json_add_number_to_array(array, 42);
    json_add_string_to_array(array, "Hello, World!");
    json_add_to_array(array, json_new_object());
    json_add_to_array(array, json_new_array());

    json_add_to_object(object, "array", array);

    json_print(object);
    printf("\n");

    FILE *file = fopen("test.json", "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    char *buffer = malloc(1024);
    int n = fread(buffer, 1024, 1, file);
    if (n < 0) {
        perror("fread");
        return 1;
    }
    fclose(file);

    struct json_value *parsed = json_parse(buffer);

    json_print(parsed);
    printf("\n");

    struct json_value *obj = json_object_get(parsed, "object");
    if (obj) {
        printf("object = ");
        json_print(obj);
        printf("\n");
        const char *str = json_object_get_string(obj, "a");
        if (str) {
            printf("object->a = %s\n", str);
        } else {
            printf("object->a = nothing\n");
        };
    }
    struct json_value *arr = json_object_get(parsed, "array");
    if (arr) {
        printf("array = ");
        json_print(arr);
        printf("\n");
        long val = json_array_get_number(arr, 1);
        printf("array[1] = %li\n", val);
    }


    return 0;
}
