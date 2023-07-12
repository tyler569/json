#pragma once
#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "list.h"

enum json_type {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT,
};

typedef list(struct json_value *) json_array;

struct json_value {
    enum json_type type;
    bool boolean;
    long number;
    const char *string;

    json_array array;

    const char *key;
};

struct json_value *json_new_value(enum json_type type);
struct json_value *json_new_null();
struct json_value *json_new_bool(bool boolean);
struct json_value *json_new_number(long number);
struct json_value *json_new_string(const char *string);
struct json_value *json_new_array();
struct json_value *json_new_object();
void json_add_to_array(struct json_value *array, struct json_value *value);
void json_add_to_object(struct json_value *object, const char *key, struct json_value *value);
void json_add_null_to_object(struct json_value *object, const char *key);
void json_add_bool_to_object(struct json_value *object, const char *key, bool boolean);
void json_add_number_to_object(struct json_value *object, const char *key, long number);
void json_add_string_to_object(struct json_value *object, const char *key, const char *string);
void json_add_bool_to_array(struct json_value *array, bool boolean);
void json_add_number_to_array(struct json_value *array, long number);
void json_add_string_to_array(struct json_value *array, const char *string);
bool json_object_contains(struct json_value *value, const char *key);
bool json_object_get_bool(struct json_value *value, const char *key);
long json_object_get_number(struct json_value *value, const char *key);
const char *json_object_get_string(struct json_value *value, const char *key);
struct json_value *json_object_get(struct json_value *value, const char *key);
size_t json_array_length(struct json_value *value);
bool json_array_get_bool(struct json_value *value, size_t index);
long json_array_get_number(struct json_value *value, size_t index);
const char *json_array_get_string(struct json_value *value, size_t index);
struct json_value *json_array_get(struct json_value *value, size_t index);
void json_print(struct json_value *value);
void json_free(struct json_value *value);
struct json_value *json_parse(const char *string);

#endif //JSON_JSON_H
