#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <stdlib.h>
#include <haut/haut.h>
#include "../structs/html.h"

// Simple callback, which stores whatever it receives to given open file
size_t callback_store_image(char* buffer, size_t size, size_t nmemb, FILE* file);

// Callback to collect html and store it in a html_t structure
size_t callback_write(char* buffer, size_t size, size_t nmemb, html_t* html);

// Callback to collect links and store them in a container structure
void callback_attribute(haut_t* p, strfragment_t* key, strfragment_t* value);

// Callback to collect title and store it in a container structure
void callback_inner_text(haut_t* p, strfragment_t* text);

#endif