#ifndef LINK_H
#define LINK_H

#include <stdlib.h>

// Structure to communicate between caller and callback for link collecting
typedef struct {
    char* content;
    size_t used, len;
    const char* base;
    size_t baselen;
} link_t;

#endif