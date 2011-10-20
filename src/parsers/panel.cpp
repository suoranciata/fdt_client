#include <string>
#include <iostream>
#include <re2/re2.h>
#include <ns.hpp>

const int s_titolo = 0;
const int s_ultimo = 1;
const int s_forum = 2;
const int s_pager = 3;
const int s_end = 4;

std::string* newline;
re2::RE2 reEmptyLine("^(\\s\\t]+)?$");
re2::RE2 reTitolo1("<a href='m\\.aspx\\?m_id=(\\d+)' class='fh1' style='font-size:1\\.25em; color: #0000EE;'>(.*?)</a>");
re2::RE2 reTitolo2("\\(<b>(\\d+)</b>\\)");
re2::RE2 reTitolo3("<a href='\\.\\./cerca.asp\\?s=nick%3a%22.*%22' class=\\'nick\\'>(.+)</a></b>");
re2::RE2 rePager("<td style=\"FONT-SIZE:0.85em;PADDING-TOP:10px\"> Pagina <b>(\\d+)</b> di <b>(\\d+)</b> - ");
re2::RE2 reLastReply1(
        "<div>"
            "<img src='images/trasp\\.gif' border='0' alt=' ' class='dotto' style='padding-right: 3px;'>"
            "(di <b>"
                "<a href='\\.\\./cerca\\.asp\\?s=nick%3a%22.*%22' class='nick'>(.*?)</a>"
            "</b> )? (alle|del) (.*)"
        "</div>");
re2::RE2 reLastReply2(
        "<div>"
            "<img src='images/trasp\\.gif' border='0' alt=' ' class='dotto' style='padding-right: 3px;'>"
            "Ultimo: "
            "<a href='m\\.aspx\\?m_id=(\\d+)' class='fh1' style='font-size:1\\.25em; color: #0000EE;'>(.*?)</a>"
            "( di <b>"
                "<a href='\\.\\./cerca\\.asp\\?s=nick%3a%22.*%22' class='nick'>(.+?)</a>"
            "</b>)?\\s+(alle|del) (.*)"
        "</div>");
re2::RE2 reSubforum(
        "<div>"
            "<img src='images/trasp\\.gif' border='0' alt=' ' class='dotto' style='padding-right: 3px;'>"
            "<a href='hp\\.aspx\\?m_id=\\d+' style='color: green; font-weight: bold;'>(.*)</a>"
        "</div>");


fdt::Forum* fdt::parsers::panel::parse(char* html) {
    char* line = strtok(html, "\n");
    int stage = s_titolo;
    fdt::Forum* panel = new fdt::Forum;
    fdt::Thread* thread;
    panel->tipo = Tipo_Panel;
    while (line != NULL) {
        if (re2::RE2::PartialMatch(line, reEmptyLine)) {
            line = strtok(NULL, "\n");
            continue;
        }
        switch (stage) {
            case s_titolo: {
                std::string m_id, titolo;
                if (re2::RE2::PartialMatch(line, reTitolo1, &m_id, &titolo)) {
                    thread = new fdt::Thread;
                    thread->op = new fdt::Post;
                    thread->op->m_id = new std::string(m_id);
                    thread->op->titolo = new std::string(titolo);
                    int n_risposte;
                    if (re2::RE2::PartialMatch(line, reTitolo2, &n_risposte))
                        thread->n_risposte = n_risposte;
                    std::string autore;
                    if (re2::RE2::PartialMatch(line, reTitolo3, &autore))
                        thread->op->autore = new std::string(autore);
                    stage++;
                } else {
                    int pagina, n_pagine;
                    if (re2::RE2::PartialMatch(line, rePager, &pagina, &n_pagine)) {
                        panel->pagina = pagina;
                        panel->n_pagine = n_pagine;
                        stage = s_end;
                    }
                }
            }
            break;
            case s_ultimo: {
                std::string m_id, skip1, autore, skip2, data;
                if (re2::RE2::PartialMatch(line, reLastReply1, &skip1, &autore, &skip2, &data)) {
                    if (thread->op->autore == NULL)
                        thread->op->autore = new std::string(autore);
                    thread->op->data_unparsed = new std::string(data);
                } else {
                    if (thread->n_risposte == 0)
                        return NULL;
                    std::string titolo;
                    re2::RE2::PartialMatch(
                            line, reLastReply2, &m_id, &titolo, &skip1, &autore, &skip2, &data);
                    thread->ultimo = new Post;
                    thread->ultimo->m_id = new std::string(m_id);
                    thread->ultimo->titolo = new std::string(titolo);
                    if (autore.length() > 0)
                        thread->ultimo->autore = new std::string(autore);
                    thread->ultimo->data_unparsed = new std::string(data);
                }
                stage++;
            }
            break;
        case s_forum: {
            std::string forumName;
            if (re2::RE2::PartialMatch(line, reSubforum, &forumName))
                thread->forumName = new std::string(forumName);
            panel->threads.push_back(*thread);
            if (panel->threads.size() == 10)
                stage++;
            else
                stage = s_titolo;
            }
            break;
        case s_pager: {
                int pagina, n_pagine;
                if (re2::RE2::PartialMatch(line, rePager, &pagina, &n_pagine)) {
                    panel->pagina = pagina;
                    panel->n_pagine = n_pagine;
                    stage++;
                }
            }
            break;
        default:
            break;
        }
        if (stage == s_end) break;
        line = strtok(NULL, "\n");
    }
    return panel;
}
