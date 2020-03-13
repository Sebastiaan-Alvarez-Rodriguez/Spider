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
    size_t used_title, len_title;
} container_t;

// Allocate required buffers for container
void container_create(container_t* container, size_t initial_capacity, const char* const base);

// Adds title to existing title. Haut separates inner text on spaces for some reason.
// In this function, they are appended back. As a consequence, each title has a trailing space
void container_changetitle(container_t* container, const char* const title, size_t len);

// Fixes trailing space for title. Call this if you are sure no more title parts will follow
void container_fixtrailingspace(container_t* container);

// Insert url in container
bool container_insert_link(container_t* container, url_t* url);

// Insert image url in container
bool container_insert_image(container_t* container, url_t* url);

// Deallocate buffers for container
void container_destroy(container_t* container);

#ifdef __cplusplus
}
#endif

#endif