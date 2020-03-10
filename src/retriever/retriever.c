#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <haut/haut.h>

#include "callbacks/callbacks.h"
#include "retriever/structs/linkcontainer/linkcontainer.h"
#include "retriever/structs/html.h"
#include "retriever.h"


// Takes a url as parameter, and returns the page html on success, NULL otherwise
static char* GetWebPage(const char* myurl) {
    html_t html;    
    html.content = malloc(1024*sizeof(char));
    html.used = 0;
    html.len = 1024;

    CURL *curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, myurl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); //We stop trying to connect after 10s (probably dead link)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
    CURLcode curl_res = curl_easy_perform(curl);

    char* ret = NULL;
    if (curl_res == 0 ) {
        ret = malloc(html.used * sizeof(char)+1);
        memcpy(ret, html.content, html.used);
        ret[html.used] = '\0';
    } else {
        ret = NULL;
    }
    curl_easy_cleanup(curl);

    free(html.content);
    return ret;
}

// Takes 2 urls as parameter, and a function. First url is the (sub)page of a domain you want to get links from
// Second url is the base domain name (e.g. domain.test, if you want to get domain.test/subdir/test.html links)
// The function argument must handle extraction of any requested data, and concatenation using '\n'
// Returns found links with newlines ('\n') between them
static linkcontainer_t* parseWithAttributeCallback(const char* myhtmlpage, const char* myurl, void (*function)(haut_t*, strfragment_t*, strfragment_t*)) {
    char* const html = GetWebPage(myhtmlpage);
    if (html == NULL)
        return NULL;
    const size_t len = strlen(html);

    linkcontainer_t* container = malloc(sizeof(linkcontainer_t));
    container_create(container, 128, myurl);

    haut_t parser;
    haut_init(&parser);
    parser.userdata = container;
    parser.events.attribute = function;
    
    haut_setInput(&parser, html, len);
    haut_parse(&parser);

    haut_destroy(&parser);
    free(html);
    return container;
}

// Takes 2 urls as parameter, and a function. First url is the (sub)page of a domain you want to get links from (in <a href="url.test"/> format).
// Second url is the base domain name (e.g. domain.test, if you want to get domain.test/subdir/test.html links)
// Returns found links with newlines ('\n') between them
linkcontainer_t* GetLinksFromWebPage(const char* myhtmlpage, const char* myurl) {
    return parseWithAttributeCallback(myhtmlpage, myurl, callback_link);
}