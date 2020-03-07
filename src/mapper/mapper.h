#ifndef MAPPER_H
#define MAPPER_H

#include <vector>
#include <string>

#include "mapper/url/url.h"
// Give a url as parameter. Returns relevant keywords for that url
std::vector<std::string> getIndex(Url url);

// Give a keyword as parameter. Returns (hopefully) relevant url's
std::vector<Url> getRelevantUrls(std::string keyword);
#endif