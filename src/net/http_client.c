#include <stdlib.h>
#include <glib.h>
#include <curl/curl.h>

size_t http_curl_writer_callback(void *ptr, size_t size, size_t nmemb, GString *s) {
    g_string_append(s, (char*)ptr);
    return size * nmemb;
}

char *http_get(char *page) {
    GString *url = g_string_new("http://www.forumdeitroll.it");
    g_string_append(url, page);
    GString *out = g_string_new("");
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl,
        CURLOPT_URL,                        url->str);
    curl_easy_setopt(curl,
        CURLOPT_WRITEFUNCTION,              http_curl_writer_callback);
    curl_easy_setopt(curl,
        CURLOPT_WRITEDATA,                  out);
    curl_easy_setopt(curl,
        CURLOPT_FOLLOWLOCATION,             1);
    curl_easy_setopt(curl,
        CURLOPT_COOKIE,                     "PITROOL_DISCLAIMER=1");
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK) {
        return out->str;
    }
    g_string_free(out, TRUE);
    return NULL;
}
