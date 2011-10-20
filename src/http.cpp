#include <string>
#include <iostream>
#include <curl/curl.h>
#include <ns.hpp>

int cb(char* ptr, size_t size, size_t nmemb, fdt::util::ChrPtrHolder* out) {
    out->append(ptr, size*nmemb);
    return size*nmemb;
}

char* fdt::http::get(std::string* page) {
    std::string* url = new std::string("http://www.forumdeitroll.it");
    url->append(*page);
    fdt::util::ChrPtrHolder *out = new fdt::util::ChrPtrHolder;

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl,
        CURLOPT_URL, url->c_str());
    curl_easy_setopt(curl,
        CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl,
        CURLOPT_WRITEDATA, out);
    curl_easy_setopt(curl,
        CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl,
        CURLOPT_COOKIE, "PITROOL_DISCLAIMER=1");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    delete url;
    if (res == CURLE_OK) {
        char* page = out->ptr;
        delete out;
        return page;
    }
    std::cout << "Curlcode was : " << res << std::endl;
    if (out->ptr != NULL)
        free(out->ptr);
    delete out;
    return NULL;
}
