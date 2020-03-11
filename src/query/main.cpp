#include <iostream>

#include "mapper/mapper.h"

// Simple function to display information in case someone did not provide the right amount of parameters
static void usage(const char* name) {
    std::cout << name << "<query>" << std::endl;
    std::cout<< R"HERE(
    <query> Keyword you wish to search for
)HERE";
    exit(-1);
}

int main(int argc, char* argv[]) {
    if (argc != 2)
        usage(argv[0]);
    if (!hasReverseIndex()) // If the spider hasn't crawled, we cannot return relevant urls
        return -1;

    for (const auto& url : getRelevantUrls(argv[1])) {
        std::cout << url.url << '\n';
    }
    return 0;
}
