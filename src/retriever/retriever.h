#ifndef RETRIEVER_H
#define RETRIEVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "retriever/structs/linkcontainer/linkcontainer.h"

linkcontainer_t* GetLinksFromWebPage(const char* myhtmlpage, const char* myurl);
#ifdef __cplusplus
}
#endif
#endif