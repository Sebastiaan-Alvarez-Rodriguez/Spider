#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <haut/haut.h>
#include <haut/tag.h>

#include "callbacks.h"
#include "../structs/link.h"
#include "../structs/html.h"

static int callback_contains_get_variable(const char* const str, size_t len) {
    for (size_t x = 0; x < len; ++x)
        if (str[x] == '?')
            return x;
    return -1;
}

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
        if (value->data[0] == '#') //Self referencing url tag. Should skip
            return;
        if (value->data[0] == 't' && value->data[1] != 'e' && value->data[2] != 'l' && value->data[3] == ':') // I noticed someone annoying put a tel:<telephone number here> inside a <a href=""/>
            return;
        size_t size = (size_t) value->size;
        bool link_is_relative = value->data[0] == '/';

        int tmp = callback_contains_get_variable(value->data, size);
        if (tmp != -1)
            size -= (size - tmp);
        else if (value->data[size-1] == '/')
            size -= 1;

        if (link_is_relative)
            size += links->baselen;
        

        while (size+1 > links->len-(links->used+1)) {
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