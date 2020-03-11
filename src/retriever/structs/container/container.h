#ifndef CONTAINER_H
#define CONTAINER_H


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
    size_t used_urls, len_urls;
    url_t* imgs;
    size_t used_imgs, len_imgs;

    char* base;
    size_t baselen;
    char* title;
} container_t;

void container_create(container_t* container, size_t initial_capacity, const char* const base);

void container_changetitle(container_t* container, const char* const title, size_t len);

bool container_insert_link(container_t* container, url_t* url);

bool container_insert_image(container_t* container, url_t* url);

// void container_insert(container_t* container, const char* const title, const char* const url) {

// }

void container_destroy(container_t* container);

#ifdef __cplusplus
}
#endif

#endif