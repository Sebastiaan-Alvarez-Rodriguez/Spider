#include <haut/haut.h>
#include <queue>
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <experimental/filesystem>

#include "mapper/url/url.h"
#include "mapper/mapper.h"
#include "tree/tree.h"
#include "retriever/retriever.h"
#include "retriever/structs/container/container.h"

// Fetch an unvisited url, in a Breadth First Search pattern. 
// Returns true on success, false if the queue gets empty before finding an unvisited url.
static bool fetch(std::string& visit_url, Tree& visited, std::queue<std::string>& to_visit) {
    do {
        if (to_visit.empty())
            return false;
        visit_url = to_visit.front();
        to_visit.pop();
    } while (!visited.insert(visit_url)); 
    // Note: insert(), right above this line, only returns true if the url did not yet exist.
    // So, this loops continues until an unvisited url is found (which is then inserted in visited)
    // or until there are no more urls to try
    return true;
}

static void map(std::string urlstring, std::string title) {
    Url url = Url(urlstring, title);
    auto w = getIndex(url);
    storeReverseIndex(url, w);
}

static void crawl(const char* start_url, unsigned long long stop_after) {
    Tree visited;

    std::queue<std::string> to_visit;
    to_visit.push(start_url);

    size_t image_number = 0;
    std::experimental::filesystem::create_directory("pics/");

    for (unsigned long long x = 0; x < stop_after; ++x) {
        std::string visit_url;
        if (!fetch(visit_url, visited, to_visit))
            break;

        std::cout << "Visiting url "<<x<<": " << visit_url;

        container_t* container = GetDataFromWebPage(visit_url.c_str(), visit_url.c_str());
        if (container != NULL) {
            if (container->title == NULL)
                std::cout <<". No title found!\n";
            else
                std::cout << ". Found title '"<<container->title <<"'.\n";
            std::cout <<"Found "<<container->used_urls<<" links and "<<container->used_imgs<<" jpg-images"<<std::endl;
            for (size_t x = 0; x < container->used_urls; ++x) {
                std::string url = std::string(container->urls[x].url, container->urls[x].len);
                to_visit.push(std::string(url)); //No need to check if url was visited already. fetch() function does that already
            }
            map(visit_url, container->title == NULL ? "" : std::string(container->title));
            image_number = StoreImages(container, image_number);
            container_destroy(container);
            free(container);
        }
        std::cout << '\n';
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
