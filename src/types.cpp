#include <string>
#include <iostream>
#include <sstream>
#include <ns.hpp>

fdt::Post::Post() {
    titolo = NULL;
    autore = NULL;
    data_unparsed = NULL;
    m_id = NULL;
}
fdt::Post::~Post() {
    delete titolo;
    delete autore;
    delete data_unparsed;
    delete m_id;
}

fdt::Thread::Thread() {
    op = NULL;
    ultimo = NULL;
    n_risposte = 0;
    forumName = NULL;
}

fdt::Thread::~Thread() {
    if (forumName != NULL) delete forumName;
    delete op;
    delete ultimo;
}

fdt::Forum::Forum() {
    nome = NULL;
    pagina = 0;
    n_pagine = 0;
    tipo = 0;
}

#define sappend(s, s2) \
    if (s2 != NULL) \
        s->append(*s2);\
    else\
        s->append("<null>");

std::string inline itoa(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

template <class T>
void sappendT(std::string* out, T *a) {
    if (a != NULL)
        out->append(*(a->tostring()));
    else
        out->append("<null>");
}


std::string *fdt::Post::tostring() {
    std::string *out = new std::string("fdt::Post{");
    out->append("titolo=");
    sappend(out, titolo);
    out->append(",autore=");
    sappend(out, autore);
    out->append(",data_unparsed=");
    sappend(out, data_unparsed);
    out->append(",m_id=");
    sappend(out, m_id);
    out->append("}");
    return out;
}

std::string *fdt::Thread::tostring() {
    std::string *out = new std::string("Thread{");
    out->append("op=");
    sappendT(out, op);
    out->append(",ultimo=");
    sappendT(out, ultimo);
    out->append(",n_risposte=");
    out->append(itoa(n_risposte));
    out->append(",forumName=");
    sappend(out, forumName);
    out->append("}");
    return out;
}

std::string *fdt::Forum::tostring() {
    std::string *out = new std::string("Forum{");
    out->append("nome=");
    sappend(out, nome);
    out->append(",pagina=");
    out->append(itoa(pagina));
    out->append(",n_pagine=");
    out->append(itoa(n_pagine));
    out->append(",tipo=");
    out->append(itoa(tipo));
    out->append(",threads=");
    for (std::list<fdt::Thread>::iterator it = threads.begin(); it != threads.end(); it++) {
        out->append(*(it->tostring()));
    }
    out->append("}");
    return out;
}
