#include <vector>
#include <string>
#include <regex>

#include <experimental/filesystem>
#include <fstream>
#include "mapper/url/url.h"

#include "mapper.h"

static int getSeparatorPos(const std::string& str, char sep) {
    for (unsigned x = 0; x < str.size(); ++x)
        if (str[x] == sep)
            return x;
    return -1;
}

// Takes a url and its keywords. Stores the url at all files with a keyword
// If you want to reverse-index then, all you have to do is open the file
// matching a keyword, and read its contents for the relevant urls.
bool storeReverseIndex(Url url, std::vector<std::string> keywords) {
    std::experimental::filesystem::create_directory("indexed/");
    for (std::string keyword : keywords) {
        std::ofstream out("indexed/"+keyword, std::ios::out|std::ios::app);
        if (!out.is_open())
            return false;
        out << url << '\n';
        out.close();
    }
    return true;
}

// Give a url as parameter. Returns relevant keywords for that url
std::vector<std::string> getIndex(Url url) {
    std::vector<std::string> result;

    const std::regex rgx1("-/#\\.");
    std::sregex_token_iterator iter1(url.url.begin(), url.url.end(), rgx1, -1);
    for (std::sregex_token_iterator end; iter1 != end; ++iter1)
        result.push_back(iter1->str());

    const std::regex rgx2(" ");
    std::sregex_token_iterator iter2(url.title.begin(), url.title.end(), rgx2, -1);
    for (std::sregex_token_iterator end; iter2 != end; ++iter2)
        result.push_back(iter2->str());
    return result;
}


// Give a keyword as parameter. Returns (hopefully) relevant url's
std::vector<Url> getRelevantUrls(std::string keyword) {
    std::vector<Url> result;
    
    std::ifstream in("indexed/"+keyword, std::ios::in);
    if (in.is_open()) {
        std::string to;
        while(std::getline(in,to,'\n')) {
            int pos = getSeparatorPos(to, '\x1');
            if (pos < 0)
                continue;

            Url url(to.substr(0, pos), to.substr(pos+1));
            result.push_back(url);
        }
    }
    return result;
}