#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <haut/haut.h>
#include <haut/tag.h>

#include "callbacks.h"
#include "retriever/structs/container/container.h"
#include "retriever/structs/html.h"
#include "retriever/structs/url/url.h"

static int callback_contains_get_variable(const char* const str, size_t len) {
    for (size_t x = 0; x < len; ++x)
        if (str[x] == '?')
            return x;
    return -1;
}

size_t callback_store_image(char* buffer, size_t size, size_t nmemb, FILE* file) {
    fwrite(buffer, size, nmemb, file);
    return size * nmemb;
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

void callback_element_parsed(haut_t* p, strfragment_t* key, strfragment_t* value) {
    container_t* container = (container_t*) p->userdata;

    if (haut_currentElementTag(p) == TAG_TITLE && value && value->data && value->size > 0) {
        printf("OOF I GOT A TITLE\n");
        container_changetitle(container, value->data, value->size);
    } else if((haut_currentElementTag(p) == TAG_A && strfragment_icmp(key, "href") && value && value->data && value->size > 0)
        || ((haut_currentElementTag(p) == TAG_IMG || haut_currentElementTag(p) == TAG_IMAGE) && strfragment_icmp(key, "src") && value && value->data && value->size > 0)) {

        size_t size = (size_t) value->size;
        bool link_is_relative = value->data[0] == '/';

        if (haut_currentElementTag(p) == TAG_A) {
            if (value->data[0] == '#' || (value->data[0] == '/' && value->size==1)) //Self referencing url tag. Should skip
                return;
            if (value->data[0] == 't' && value->data[1] != 'e' && value->data[2] != 'l' && value->data[3] == ':') // I noticed someone annoying put a tel:<telephone number here> inside a <a href=""/>
                return;
        
            int tmp = callback_contains_get_variable(value->data, size);
            if (tmp != -1)
                size -= (size - tmp);
            else if (value->data[size-1] == '/')
                size -= 1;
        } else {
            if (!(value->data[value->size-3] == 'j' && value->data[value->size-2] == 'p' && value->data[value->size-1] == 'g')
                && !(value->data[value->size-4] == 'j' && value->data[value->size-3] == 'p' && value->data[value->size-2] == 'e'
                && value->data[value->size-2] == 'g'))
                return; //We only want jpg images
        }

        if (link_is_relative)
            size += container->baselen;

        url_t url;
        url.url = malloc(size*sizeof(char));
        if (link_is_relative) {
            memcpy(url.url, container->base, container->baselen); //copy base url
            memcpy((url.url+container->baselen), value->data, size-container->baselen); //copy relative part
        } else {
            memcpy(url.url, value->data, size);
        }
        url.len = size;
        if (haut_currentElementTag(p) == TAG_A)
            container_insert_link(container, &url);
        else
            container_insert_image(container, &url);
    }
}