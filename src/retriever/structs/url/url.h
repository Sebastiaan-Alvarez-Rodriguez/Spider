#ifndef URL_C_H
#define URL_C_H

#include <stdlib.h>

// Structure to communicate between caller and callback for html collecting
typedef struct {
    char* url;
    size_t len;
} url_t;

void url_destroy(url_t* url);
#endif