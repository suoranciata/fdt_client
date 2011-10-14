#include <stdlib.h>
#include <curl/curl.h>
#include <util/xstring.h>
#include <http/client.h>

typedef struct string {
    char *str;
} string;

size_t http_curl_writer_callback(void *ptr, size_t size, size_t nmemb, string *s) {
    s->str = xstrcat(s->str, (char*) ptr);
    return size * nmemb;
}

char *http_get(char *page) {
    char *url = xstrdup("http://www.forumdeitroll.it");
    url = xstrcat(url, page);
    string *out = (string*) malloc (sizeof(string));
    out->str = (char*) malloc (1);
    out->str[0] = '\0';
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl,
        CURLOPT_URL,                        url);
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
    free(url);
    if (res == CURLE_OK) {
        char *ret = out->str;
        free(out);
        return ret;
    }
    free(out->str);
    free(out);
    return NULL;
}
