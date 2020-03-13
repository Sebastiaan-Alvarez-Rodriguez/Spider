#ifndef RETRIEVER_H
#define RETRIEVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "retriever/structs/container/container.h"

// Takes a container (with images to download) and a starting number
// Downloads all images, to <number>.jpg, <number+1>.jpg, etc
// Returns next free number to use in the future
size_t StoreImages(const container_t* container, size_t start_nr);

// Takes 2 urls as parameter.
// First url is the (sub)page of a domain you want to get links from
// Second url is the base domain name (e.g. domain.test, if you want to get domain.test/subdir/test.html links)
// Returns a container, containing web links and image links, and title of visited url.
// Returns NULL on failure (e.g: website does not respond)
container_t* GetDataFromWebPage(const char* myhtmlpage, const char* myurl);
#ifdef __cplusplus
}
#endif
#endif