#include <vector>
#include <string>
#include <regex>

#include <experimental/filesystem>
#include <fstream>
#include <cctype>
#include <algorithm>

#include "mapper/url/url.h"
#include "mapper.h"

static int getSeparatorPos(const std::string& str, char sep) {
    for (unsigned x = 0; x < str.size(); ++x)
        if (str[x] == sep)
            return x;
    return -1;
}

static std::string str_tolower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c){ return std::tolower(c); }
    );
    return s;
}

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
    //TODO: Ignore https://, http://, www.

    std::string urlstring = url.url;
    size_t tmp = urlstring.find("www.");

    if (tmp != urlstring.npos)
        urlstring = urlstring.substr(tmp+4);

    const std::regex rgx1(" -|\\/|#|\\.’`&?!+=():;");
    std::sregex_token_iterator iter1(urlstring.begin(), urlstring.end(), rgx1, -1);
    for (std::sregex_token_iterator end; iter1 != end; ++iter1)
        if (iter1->str().size() > 3)
            result.push_back(str_tolower(iter1->str()));

    const std::regex rgx2(" !+=()&#:/;");
    std::sregex_token_iterator iter2(url.title.begin(), url.title.end(), rgx2, -1);
    for (std::sregex_token_iterator end; iter2 != end; ++iter2)
        if (iter2->str().size() > 3)
            result.push_back(str_tolower(iter2->str()));

    return result;
}

// Give a keyword as parameter. Returns (hopefully) relevant url's
std::vector<Url> getRelevantUrls(std::string keyword) {
    std::vector<Url> result;
    keyword = str_tolower(keyword);
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

bool hasReverseIndex() {
    return std::experimental::filesystem::exists("indexed/");
}

bool hasPics() {
    return std::experimental::filesystem::exists("pics/");
}