#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "retriever/structs/url/url.h"
#include "linkcontainer.h"

void container_create(linkcontainer_t* container, size_t initial_capacity, const char* const base) {
    container->urls = malloc(initial_capacity*sizeof(url_t));
    container->used = 0;
    container->len = initial_capacity;

    container->baselen = strlen(base)+1; //+1 is for \0
    container->base = (char*) malloc(container->baselen*sizeof(char));
    strcpy(container->base, base);
    container->title = NULL;
}

void container_changetitle(linkcontainer_t* container, const char* const title, size_t len) {
    container->title = (char*) realloc(container->title, len*sizeof(char));
    strcpy(container->title, title);
}

bool container_insert(linkcontainer_t* container, url_t* url) {
    if (container->used == container->len) {
        url_t* tmp = (url_t*) realloc(container->urls, 2*container->len);
        if (tmp == NULL)
            return false;
        container->urls = tmp;
        container->len *= 2;
    }

    container->urls[container->used].url = url->url;
    container->urls[container->used].len = url->len;
    ++container->used;
    return true;
}

// void container_insert(linkcontainer_t* container, const char* const title, const char* const url) {

// }

void container_destroy(linkcontainer_t* container) {
    for (size_t x = 0; x < container->used; ++x)
        url_destroy(&(container->urls[x]));
    free(container->urls);
    free(container->base);
    free(container->title);
}