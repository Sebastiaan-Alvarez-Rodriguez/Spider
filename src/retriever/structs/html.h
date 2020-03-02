#ifndef HTML_H
#define HTML_H

#include <stdlib.h>

// Structure to communicate between caller and callback for html collecting
typedef struct {
    char* content;
    size_t used, len;
} html_t;

#endif