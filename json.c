#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

struct json_value *json_new_value(enum json_type type) {
    struct json_value *value = calloc(1, sizeof(struct json_value));
    value->type = type;
    return value;
}

struct json_value *json_new_null() {
    return json_new_value(JSON_NULL);
}

struct json_value *json_new_bool(bool boolean) {
    struct json_value *value = json_new_value(JSON_BOOL);
    value->boolean = boolean;
    return value;
}

struct json_value *json_new_number(long number) {
    struct json_value *value = json_new_value(JSON_NUMBER);
    value->number = number;
    return value;
}

struct json_value *json_new_string(const char *string) {
    struct json_value *value = json_new_value(JSON_STRING);
    value->string = string;
    return value;
}

struct json_value *json_new_array() {
    struct json_value *value = json_new_value(JSON_ARRAY);
    return value;
}

struct json_value *json_new_object() {
    struct json_value *value = json_new_value(JSON_OBJECT);
    return value;
}

void json_add_to_array(struct json_value *array, struct json_value *value) {
    list_push(&array->array, value);
}

void json_add_to_object(struct json_value *object, const char *key, struct json_value *value) {
    value->key = key;
    list_push(&object->array, value);
}

void json_add_null_to_object(struct json_value *object, const char *key) {
    json_add_to_object(object, key, json_new_null());
}

void json_add_bool_to_object(struct json_value *object, const char *key, bool boolean) {
    json_add_to_object(object, key, json_new_bool(boolean));
}

void json_add_number_to_object(struct json_value *object, const char *key, long number) {
    json_add_to_object(object, key, json_new_number(number));
}

void json_add_string_to_object(struct json_value *object, const char *key, const char *string) {
    json_add_to_object(object, key, json_new_string(string));
}

void json_add_bool_to_array(struct json_value *array, bool boolean) {
    json_add_to_array(array, json_new_bool(boolean));
}

void json_add_number_to_array(struct json_value *array, long number) {
    json_add_to_array(array, json_new_number(number));
}

void json_add_string_to_array(struct json_value *array, const char *string) {
    json_add_to_array(array, json_new_string(string));
}

bool json_object_contains(struct json_value *value, const char *key) {
    if (value->type != JSON_OBJECT) {
        return false;
    }

    for_each(&value->array) {
        if (strcmp((*it)->key, key) == 0) {
            return true;
        }
    }
    return false;
}

bool json_object_get_bool(struct json_value *value, const char *key) {
    if (value->type != JSON_OBJECT) {
        return false;
    }

    for_each(&value->array) {
        if (strcmp((*it)->key, key) == 0) {
            if ((*it)->type != JSON_BOOL) {
                return false;
            }
            return (*it)->boolean;
        }
    }
    return false;
}

long json_object_get_number(struct json_value *value, const char *key) {
    if (value->type != JSON_OBJECT) {
        return 0;
    }

    for_each(&value->array) {
        if (strcmp((*it)->key, key) == 0) {
            if ((*it)->type != JSON_NUMBER) {
                return 0;
            }
            return (*it)->number;
        }
    }
    return 0;
}

const char *json_object_get_string(struct json_value *value, const char *key) {
    if (value->type != JSON_OBJECT) {
        return nullptr;
    }

    for_each(&value->array) {
        if (strcmp((*it)->key, key) == 0) {
            if ((*it)->string == nullptr) {
                return nullptr;
            }
            return (*it)->string;
        }
    }
    return nullptr;
}

struct json_value *json_object_get(struct json_value *value, const char *key) {
    if (value->type != JSON_OBJECT) {
        return nullptr;
    }

    for_each(&value->array) {
        if (strcmp((*it)->key, key) == 0) {
            return *it;
        }
    }
    return nullptr;
}

size_t json_array_length(struct json_value *value) {
    if (value->type != JSON_ARRAY) {
        return 0;
    }
    return value->array.len;
}

bool json_array_get_bool(struct json_value *value, size_t index) {
    if (index >= value->array.len) {
        return false;
    }

    struct json_value *it = list_at(&value->array, index);
    if (it->type != JSON_BOOL) {
        return false;
    }
    return it->boolean;
}

long json_array_get_number(struct json_value *value, size_t index) {
    if (index >= value->array.len) {
        return 0;
    }

    struct json_value *it = list_at(&value->array, index);
    if (it->type != JSON_NUMBER) {
        return 0;
    }
    return it->number;
}

const char *json_array_get_string(struct json_value *value, size_t index) {
    if (index >= value->array.len) {
        return nullptr;
    }

    struct json_value *it = list_at(&value->array, index);
    if (it->type != JSON_STRING) {
        return nullptr;
    }
    return it->string;
}

struct json_value *json_array_get(struct json_value *value, size_t index) {
    if (index >= value->array.len) {
        return nullptr;
    }

    struct json_value *it = list_at(&value->array, index);
    return it;
}

void json_free(struct json_value *value) {
    switch (value->type) {
    case JSON_NULL:
    case JSON_BOOL:
    case JSON_NUMBER:
        break;
    case JSON_STRING:
        free((void *)value->string);
        break;
    case JSON_ARRAY:
    case JSON_OBJECT:
        for_each (&value->array) {
            json_free(*it);
        }
        break;
    }
    free(value);
}

void json_print(struct json_value *value) {
    switch (value->type) {
    case JSON_NULL:
        printf("null");
        break;
    case JSON_BOOL:
        printf(value->boolean ? "true" : "false");
        break;
    case JSON_NUMBER:
        printf("%ld", value->number);
        break;
    case JSON_STRING:
        printf("\"%s\"", value->string);
        break;
    case JSON_ARRAY:
        printf("[");
        for_each(&value->array) {
            json_print(*it);
            if (it != &list_last(&value->array)) {
                printf(", ");
            }
        }
        printf("]");
        break;
    case JSON_OBJECT:
        printf("{");
        for_each(&value->array) {
            printf("\"%s\": ", (*it)->key);
            json_print(*it);
            if (it != &list_last(&value->array)) {
                printf(", ");
            }
        }
        printf("}");
        break;
    }
}

#define EAT(c, char) \
    do {          \
        assert(*(c) == char); \
        (c)++; \
    } while (0)

#define PASS_WHITESPACE(c) \
    do {                   \
        while (isspace(*c)) { \
            c++; \
        } \
    } while (0)

static size_t json_string_outer_len(const char *c) {
    const char *start = c;

    EAT(c, '"');
    while (*c && *c != '"') {
        if (*c == '\\') {
            c++;
        }
        c++;
    }
    EAT(c, '"');

    return c - start;
}

static const char *json_string_parse(const char *json_string, const char **out) {
    const char *c = json_string;
    size_t len = json_string_outer_len(c);
    char *string = (char *)malloc(len + 1);
    const char *s = string;

    bool in_escape = false;

    EAT(c, '"');

    while (*c != '"') {
        if (in_escape) {
            switch(*c) {
            case '"':
            case '\\':
            case '/':
                string[len++] = *c;
                break;
            case 'b':
                string[len++] = '\b';
                break;
            case 'f':
                string[len++] = '\f';
                break;
            case 'n':
                string[len++] = '\n';
                break;
            case 'r':
                string[len++] = '\r';
                break;
            case 't':
                string[len++] = '\t';
                break;
            case 'u':
                assert(false);
            }
        } else if (*c == '\\') {
            in_escape = true;
            c++;
        } else {
            *string++ = *c++;
        }
    }

    *string = '\0';
    EAT(c, '"');

    assert(json_string + len == c);
    *out = c;
    return s;
}

static struct json_value *json_parse_inner(const char *json_string, const char **out) {
    const char *c = json_string;
    struct json_value *value;

    PASS_WHITESPACE(c);
    while (*c != '\0') {
        switch (*c) {
        case 'n':
            EAT(c, 'n');
            EAT(c, 'u');
            EAT(c, 'l');
            EAT(c, 'l');
            value = json_new_null();
            goto done;
        case 't':
            EAT(c, 't');
            EAT(c, 'r');
            EAT(c, 'u');
            EAT(c, 'e');
            value = json_new_bool(true);
            goto done;
        case 'f':
            EAT(c, 'f');
            EAT(c, 'a');
            EAT(c, 'l');
            EAT(c, 's');
            EAT(c, 'e');
            value = json_new_bool(false);
            goto done;
        case '"': {
            value = json_new_string(json_string_parse(c, &c));
            goto done;
        }
        case '[': {
            struct json_value *array = json_new_array();
            EAT(c, '[');
            PASS_WHITESPACE(c);
            while (*c != ']') {
                PASS_WHITESPACE(c);
                json_add_to_array(array, json_parse_inner(c, &c));
                PASS_WHITESPACE(c);
                if (*c == ',') {
                    c++;
                }
            }
            EAT(c, ']');
            value = array;
            goto done;
        }
        case '{': {
            struct json_value *object = json_new_object();
            EAT(c, '{');
            PASS_WHITESPACE(c);
            while (*c != '}') {
                const char *key = json_string_parse(c, &c);
                PASS_WHITESPACE(c);
                EAT(c, ':');
                PASS_WHITESPACE(c);
                struct json_value *value = json_parse_inner(c, &c);
                value->key = key;
                json_add_to_array(object, value);
                PASS_WHITESPACE(c);
                if (*c == ',') {
                    c++;
                }
                PASS_WHITESPACE(c);
            }
            EAT(c, '}');
            value = object;
            goto done;
        }
        }
        if (isdigit(*c)) {
            long number = 0;
            while (isdigit(*c)) {
                number = number * 10 + (*c - '0');
                c++;
            }
            value = json_new_number(number);
            goto done;
        }
    }

    assert(0);
done:
    if (out) {
        *out = c;
    }
    return value;
}

struct json_value *json_parse(const char *json_string) {
    return json_parse_inner(json_string, NULL);
}
