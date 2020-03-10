#include "url.h"

#include <string.h>

void url_destroy(url_t* url) {
    free(url->url);
}