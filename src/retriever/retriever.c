#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <haut/haut.h>

#include "callbacks/callbacks.h"
#include "structs/link.h"
#include "structs/html.h"
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
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    CURLcode curl_res = curl_easy_perform(curl);

    char* ret;
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
static char* parseWithAttributeCallback(const char* myhtmlpage, const char* myurl, void (*function)(haut_t*, strfragment_t*, strfragment_t*)) {
    char* const html = GetWebPage(myhtmlpage);
    const size_t len = strlen(html);

    link_t links;
    links.content = malloc(1024*sizeof(char));
    links.used = 0;
    links.len = 1024;
    links.base = myurl;
    links.baselen = strlen(myurl);

    haut_t parser;
    haut_init(&parser);
    parser.userdata = &links;
    parser.events.attribute = function;
    
    haut_setInput(&parser, html, len);
    haut_parse(&parser);

    haut_destroy(&parser);

    char* ret = NULL;
    if (links.used != 0) {
        ret = malloc(links.used * sizeof(char) +1);
        memcpy(ret, links.content, links.used);
        ret[links.used] = '\0';
    }
    free(links.content);
    free(html);
    return ret;
}

// Takes 2 urls as parameter, and a function. First url is the (sub)page of a domain you want to get links from (in <a href="url.test"/> format).
// Second url is the base domain name (e.g. domain.test, if you want to get domain.test/subdir/test.html links)
// Returns found links with newlines ('\n') between them
char* GetLinksFromWebPage(const char* myhtmlpage, const char* myurl) {
    return parseWithAttributeCallback(myhtmlpage, myurl, callback_link);
}