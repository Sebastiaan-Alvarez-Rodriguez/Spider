#include <haut/haut.h>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "retriever/retriever.h"

static const auto segment(char *sentence) {
    std::vector<std::string> ret;

    std::stringstream ss(sentence);
    std::string to;
    if (sentence != NULL)
    while(std::getline(ss,to,'\n'))
        ret.push_back(to);
    return ret;
}

static void crawl(const char* start_url, unsigned long long stop_after) {
    std::set<std::string> visited;
    visited.insert(start_url);

    for (unsigned long long x = 0; x < stop_after; ++x) {
        char* urllinks = GetLinksFromWebPage(start_url, start_url);
        const auto linklist = segment(urllinks);

        std::cout << "Found these links (start): "<<linklist[0]<<". Amount: "<<linklist.size()<<std::endl;
        free(urllinks);
    }
}


// Simple function to display information in case someone did not provide the right amount of parameters
static void usage(const char* name) {
    std::cout << name << "<web-entrypoint> <stop-after>" << std::endl;
    std::cout<< R"HERE(
    <web-entrypoint> should specify a websitewhere we will start crawling
    <stop-after> specifies after how many websites we stop crawling
)HERE";
    exit(-1);
}

int main(int argc, char* argv[]) {
    if (argc != 3)
        usage(argv[0]);
    else
        crawl(argv[1], std::strtoull(argv[2], NULL, 0));
    return 0;
}