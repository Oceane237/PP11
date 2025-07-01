#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "json_list.h"

JObject* parse_json(const char *filename) {
    json_t *root;
    json_error_t error;
    root = json_load_file(filename, 0, &error);

    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return NULL;
    }

    if (!json_is_array(root)) {
        fprintf(stderr, "Error: root is not a JSON array\n");
        json_decref(root);
        return NULL;
    }

    JObject *head = NULL;

    size_t index;
    json_t *item;

    json_array_foreach(root, index, item) {
        json_t *jname = json_object_get(item, "name");
        json_t *jage = json_object_get(item, "age");

        if (!json_is_string(jname) || !json_is_integer(jage)) {
            fprintf(stderr, "Invalid JSON object at index %zu\n", index);
            continue;
        }

        JObject *node = malloc(sizeof(JObject));
        if (!node) exit(EXIT_FAILURE);

        node->name = strdup(json_string_value(jname));
        node->age = (int)json_integer_value(jage);
        node->next = head;
        head = node;
    }

    json_decref(root);
    return head;
}

void print_list(JObject *head) {
    for (JObject *p = head; p; p = p->next) {
        printf("Name: %s, Age: %d\n", p->name, p->age);
    }
}

void free_list(JObject *head) {
    while (head) {
        JObject *next = head->next;
        free(head->name);
        free(head);
        head = next;
    }
}

