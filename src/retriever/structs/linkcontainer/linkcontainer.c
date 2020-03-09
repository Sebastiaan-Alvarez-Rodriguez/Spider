#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "linkcontainer.h"

void container_create(linkcontainer_t* container, size_t initial_capacity, const char* const base) {
    container->urls = (char**) malloc(initial_capacity*sizeof(char*));
    container->len = initial_capacity;

    container->baselen = strlen(base);
    container->base = (char*) malloc(container->baselen*sizeof(char));
    strcpy(container->base, base);
    container->title = NULL;
}

void container_changetitle(linkcontainer_t* container, const char* const title, size_t len) {
    container->title = (char*) realloc(container->title, len*sizeof(char));
    strcpy(container->title, title);
}

bool container_insert(linkcontainer_t* container, char* url) {
    if (container->used == container->len) {
        char** tmp = (char**) realloc(container->urls, 2*container->len);
        if (tmp == NULL)
            return false;
        container->urls = tmp;
    }

    container->urls[container->used] = url;
    ++container->used;
    return true;
}

// void container_insert(linkcontainer_t* container, const char* const title, const char* const url) {

// }

void container_destroy(linkcontainer_t* container) {
    for (size_t x = 0; x < container->used; ++x)
        free(container->urls[x]);
    free(container->urls);
    free(container->base);
    free(container->title);
}