#include <haut/haut.h>
#include <iostream>


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
    return 0;
}