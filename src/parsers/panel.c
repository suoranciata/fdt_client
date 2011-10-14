#include <stdlib.h>
#include <pcre.h>
#include <types/forum.h>
#include <util/re.h>
#include <parsers/panel.h>

static char* new_line = "\n";

static pcre* re_empty_line;

static pcre* re_titolo1;
static pcre* re_titolo2;
static pcre* re_titolo3;

static pcre* re_last_reply1;
static pcre* re_last_reply2;

static pcre* re_subforum;
static pcre* re_pager;

void parse_panel_init(void) {
    re_empty_line = re_compile("^([\\s\\t]+)?$");

    re_titolo1 = re_compile(
        "<a href='m\\.aspx\\?m_id=(\\d+)' class='fh1' style='font-size:1\\.25em; color: #0000EE;'>(.*?)</a>");
    re_titolo2 = re_compile("\\(<b>(\\d+)</b>\\)");
    re_titolo3 = re_compile(
        "<a href='\\.\\./cerca.asp\\?s=nick%3a%22.*%22' class=\\'nick\\'>(.+)</a></b>");

    re_last_reply1 = re_compile(
        "<div>"
            "<img src='images/trasp\\.gif' border='0' alt=' ' class='dotto' style='padding-right: 3px;'>"
            "(di <b>"
                "<a href='\\.\\./cerca\\.asp\\?s=nick%3a%22.*%22' class='nick'>(.*?)</a>"
            "</b> )? (alle|del) (.*)"
        "</div>");
    re_last_reply2 = re_compile(
        "<div>"
            "<img src='images/trasp\\.gif' border='0' alt=' ' class='dotto' style='padding-right: 3px;'>"
            "Ultimo: "
            "<a href='m\\.aspx\\?m_id=(\\d+)' class='fh1' style='font-size:1\\.25em; color: #0000EE;'>(.*?)</a>"
            "( di <b>"
                "<a href='\\.\\./cerca\\.asp\\?s=nick%3a%22.*%22' class='nick'>(.+?)</a>"
            "</b>)?\\s+(alle|del) (.*)"
        "</div>");

    re_subforum = re_compile(
        "<div>"
            "<img src='images/trasp\\.gif' border='0' alt=' ' class='dotto' style='padding-right: 3px;'>"
            "<a href='hp\\.aspx\\?m_id=\\d+' style='color: green; font-weight: bold;'>(.*)</a>"
        "</div>");

    re_pager = re_compile(
        "<td style=\"FONT-SIZE:0.85em;PADDING-TOP:10px\"> Pagina <b>(\\d+)</b> di <b>(\\d+)</b> - ");
}

void parse_panel_end(void) {
    free(re_empty_line);

    free(re_titolo1);
    free(re_titolo2);
    free(re_titolo3);

    free(re_last_reply1);
    free(re_last_reply2);

    free(re_subforum);

    free(re_pager);
}

enum stages {
    S_TITOLO = 0,
    S_ULTIMO = 1,
    S_FORUM = 2,
    S_PAGER = 3,
    S_END = 4
};

forum *parse_panel(char *page) {
    char *line = strtok(page, new_line);
    int stage = S_TITOLO;
    forum *panel = new_forum(PANEL);
    thread *current;
    char **matches;
    while (line != NULL) {
        // salta linee vuote
        matches = re_exec(re_empty_line, line, 1);
        if (matches != NULL) {
            re_free_matches(matches, 1);
            line = strtok(NULL, new_line);
            continue;
        }
        switch (stage) {
            case S_TITOLO:
                // re_titolo1: m_id e titolo open-post
                matches = re_exec(re_titolo1, line, 2);
                if (matches == NULL) {
                    // panel con meno di 10 messaggi
                    matches = re_exec(re_pager, line, 2);
                    if (matches == NULL)
                        break;
                    panel->pagina = atoi(matches[1]);
                    panel->n_pagine = atoi(matches[2]);
                    re_free_matches(matches, 2);
                    stage = S_END;
                }
                current = new_thread();
                current->op = new_post();
                current->op->m_id = matches[1];
                current->op->titolo = matches[2];
                free(matches[0]);
                free(matches);
                // re_titolo2: n_risposte, se c'è
                matches = re_exec(re_titolo2, line, 1);
                if (matches != NULL) {
                    current->n_risposte = atoi( matches[1] );
                    re_free_matches(matches, 1);
                }
                // re_titolo3: autore, se c'è
                matches = re_exec(re_titolo3, line, 1);
                if (matches != NULL) {
                    current->op->autore = matches[1];
                    free(matches[0]);
                    free(matches);
                }
                stage++;
                break;
            case S_ULTIMO:
                // re_last_reply1: autore se autenticato e un solo msg
                // oppure titolo, autore e data ultimo msg
                matches = re_exec(re_last_reply1, line, 4);
                if (matches) {
                    free(matches[0]);
                    free(matches[1]);
                    if (
                            current->op->autore == NULL 
                            && matches[2] != NULL 
                            && strlen(matches[2]) > 0) {
                            current->op->autore = matches[2];
                    } else {
                        free(matches[2]);
                    }
                    free(matches[3]);
                    current->op->data_unparsed = matches[4];
                    free(matches);
                } else {
                    if (current->n_risposte == 0) {
                        return NULL; //imbozzibile
                    }
                    // re_last_reply2: m_id, titolo, autore, data_unparsed dell'ultimo msg
                    matches = re_exec(re_last_reply2, line, 6);
                    current->ultimo = new_post();
                    free(matches[0]);
                    current->ultimo->m_id = matches[1];
                    current->ultimo->titolo = matches[2];
                    free(matches[3]);
                    if (strlen(matches[4]) > 0) {
                        current->ultimo->autore = matches[4];
                    } else {
                        free(matches[4]);
                    }
                    free(matches[5]);
                    current->ultimo->data_unparsed = matches[6];
                    free(matches);
                }
                stage++;
                break;
            case S_FORUM:
                // re_subforum: nome del subforum se c'è
                matches = re_exec(re_subforum, line, 1);
                if (matches != NULL) {
                    current->subforum = matches[1];
                    free(matches[0]);
                    free(matches);
                }
                panel->threads[(panel->n_threads)] = current;
                panel->n_threads++;
                if (panel->n_threads == 10)
                    stage++;
                else
                    stage = S_TITOLO;
            case S_PAGER:
                matches = re_exec(re_pager, line, 2);
                if (matches != NULL) {
                    panel->pagina = atoi(matches[1]);
                    panel->n_pagine = atoi(matches[2]);
                    re_free_matches(matches, 2);
                    stage++;
                }
            default:
                break;
        }
        if (stage == S_END)
            break;
        line = strtok(NULL, new_line);
    }
    return panel;
}
