#ifndef URL_H
#define URL_H

#include <iostream>
#include <string>

struct Url {
    std::string url, title;
    Url(std::string url, std::string title) : url(url), title(title) {}
};

std::ostream& operator<<(std::ostream& os, const Url& url) {
    os << url.url << '\x1' << url.title;
    return os;
}

#endif