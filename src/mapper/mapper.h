#ifndef MAPPER_H
#define MAPPER_H

#include <vector>
#include <string>

#include "mapper/url/url.h"

// Takes a url and its keywords. Stores the url at all files with a keyword
// If you want to reverse-index then, all you have to do is open the file
// matching a keyword, and read its contents for the relevant urls.
bool storeReverseIndex(Url url, std::vector<std::string> keywords);

// Give a url as parameter. Returns relevant keywords for that url
std::vector<std::string> getIndex(Url url);

// Give a keyword as parameter. Returns (hopefully) relevant url's
std::vector<Url> getRelevantUrls(std::string keyword);
#endif