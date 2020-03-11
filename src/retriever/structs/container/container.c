#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "retriever/structs/url/url.h"
#include "container.h"

void container_create(container_t* container, size_t initial_capacity, const char* const base) {
    // prepare link storage
    container->urls = malloc(initial_capacity*sizeof(url_t));
    container->used_urls = 0;
    container->len_urls = initial_capacity;

    // prepare image storage
    container->imgs = malloc(initial_capacity*sizeof(url_t));
    container->used_imgs = 0;
    container->len_imgs = initial_capacity;

    // Store base url and prepare title storage
    container->baselen = strlen(base); //+1 is for \0
    container->base = (char*) malloc(container->baselen*sizeof(char));
    memcpy(container->base, base, container->baselen);
    container->title = NULL;
}

void container_changetitle(container_t* container, const char* const title, size_t len) {
    container->title = (char*) realloc(container->title, len*sizeof(char));
    strcpy(container->title, title);
}

bool container_insert_link(container_t* container, url_t* url) {
    if (container->used_urls == container->len_urls) {
        url_t* tmp = (url_t*) realloc(container->urls, 2*container->len_urls*sizeof(url_t));
        if (tmp == NULL)
            return false;
        container->urls = tmp;
        container->len_urls *= 2;
    }
    container->urls[container->used_urls].url = url->url;
    container->urls[container->used_urls].len = url->len;
    ++container->used_urls;
    return true;
}

bool container_insert_image(container_t* container, url_t* url) {
    if (container->used_imgs == container->len_imgs) {
        url_t* tmp = (url_t*) realloc(container->imgs, 2*container->len_imgs*sizeof(url_t));
        if (tmp == NULL)
            return false;
        container->imgs = tmp;
        container->len_imgs *= 2;
    }
    container->imgs[container->used_imgs].url = url->url;
    container->imgs[container->used_imgs].len = url->len;
    ++container->used_imgs;
    return true;
}

// void container_insert(container_t* container, const char* const title, const char* const url) {

// }

void container_destroy(container_t* container) {
    for (size_t x = 0; x < container->used_urls; ++x)
        url_destroy(&(container->urls[x]));
    for (size_t x = 0; x < container->used_imgs; ++x)
        url_destroy(&(container->imgs[x]));
    free(container->urls);
    free(container->imgs);
    free(container->base);
    free(container->title);
}