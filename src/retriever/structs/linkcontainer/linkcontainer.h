#ifndef LINKCONTAINER_H
#define LINKCONTAINER_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "retriever/structs/url/url.h"
// Structure to communicate between caller and callback for link collecting
typedef struct {
    url_t* urls;
    size_t used, len;
    char* base;
    size_t baselen;
    char* title;
} linkcontainer_t;

void container_create(linkcontainer_t* container, size_t initial_capacity, const char* const base);

void container_changetitle(linkcontainer_t* container, const char* const title, size_t len);

bool container_insert(linkcontainer_t* container, url_t* url);

// void container_insert(linkcontainer_t* container, const char* const title, const char* const url) {

// }

void container_destroy(linkcontainer_t* container);

#ifdef __cplusplus
}
#endif

#endif