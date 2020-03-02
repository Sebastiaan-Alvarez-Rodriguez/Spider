#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <haut/haut.h>
#include <haut/tag.h>

#include "callbacks.h"
#include "../structs/link.h"
#include "../structs/html.h"


size_t callback_write(char* buffer, size_t size, size_t nmemb, html_t* html) {
    const size_t realsize = size * nmemb;

    while (realsize > (html->len - html->used)) {
        void* const newBuffer = realloc(html->content, html->len * 2);
        if (!newBuffer) {
            fprintf(stderr, "Could not allocate more memory on this machine.\n");
            return -1;
        }
        html->content = newBuffer;
        html->len *= 2;
    }

    char* const writeHead = html->content + html->used;
    memcpy(writeHead, buffer, realsize);
    html->used += realsize;

    return realsize;
}

void callback_link(haut_t* p, strfragment_t* key, strfragment_t* value) {
    link_t* links = (link_t*) p->userdata;

    if(haut_currentElementTag(p) == TAG_A && strfragment_icmp(key, "href") && value && value->data && value->size > 0) {
        size_t size = (size_t) value->size;
        bool link_is_relative = value->data[0] == '/';

        if (link_is_relative)
            size += links->baselen;
        

        while (size > links->len-(links->used+1)) {
            void* const newBuffer = realloc(links->content, links->len * 2);
            if (!newBuffer) {
                fprintf(stderr, "Could not allocate more memory on this machine.\n");
                return;
            }
            links->content = newBuffer;
            links->len *= 2;
        }

        char* const writeHead = links->content + links->used;
        if (link_is_relative) {
            memcpy(writeHead, links->base, links->baselen); //copy base url
            memcpy(writeHead+links->baselen, value->data, size-links->baselen); //copy relative part
        } else {
            memcpy(links->content + links->used, value->data, size);
        }
        writeHead[size] = '\n';
        links->used += size+1;
    }
}