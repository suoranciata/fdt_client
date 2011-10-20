#ifndef ns_hpp
#define ns_hpp

#include <string>
#include <list>
#include <cstdlib>
#include <cstring>

#define CLOG(x) \
    std::cout << x << std::endl;

namespace fdt {
    class Post {
        public:
            std::string *autore, *titolo, *data_unparsed, *m_id;
            Post();
            ~Post();
            std::string *tostring();
    };
    class Thread {
        public:
            Post *op, *ultimo;
            std::string *forumName;
            int n_risposte;
            Thread();
            ~Thread();
            std::string *tostring();
    };
    enum TipiForum {
        Tipo_Panel = 0,
        Tipo_Forum = 1
    };
    class Forum {
        public:
            std::string *nome;
            int pagina, n_pagine, tipo;
            std::list<Thread> threads;
            Forum();
            ~Forum();
            std::string *tostring();
    };
    namespace util {
        class ChrPtrHolder {
            public:
                char* ptr;
                int len;
                ChrPtrHolder() {
                    ptr = NULL;
                    len = 0;
                }
                void append(char* s, int l) {
                    if (ptr == NULL) {
                        ptr = (char*)malloc(l+1);
                        memcpy(ptr, s, l);
                        ptr[l] = '\0';
                        len = l;
                    } else {
                        ptr = (char*)realloc(ptr, len+l+1);
                        memcpy(ptr+len, s, l);
                        ptr[len+l] = '\0';
                        len += l;
                    }
                }
        };
    }
    namespace http {
        char *get(std::string *resource);
    }
    namespace parsers {
        namespace panel {
            fdt::Forum* parse(char *html);
        }
    }
}

#endif
