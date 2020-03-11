#ifndef RETRIEVER_H
#define RETRIEVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "retriever/structs/container/container.h"

size_t StoreImages(const container_t* container, size_t start_nr);

container_t* GetDataFromWebPage(const char* myhtmlpage, const char* myurl);
#ifdef __cplusplus
}
#endif
#endif