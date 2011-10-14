#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <glib/gregex.h>

#include "p_panel.h"
#include "../core/types.h"

static GRegex *whitespaces;
static GRegex *reTitolo1;
static GRegex *reTitolo2;
static GRegex *reTitolo3;
static GRegex *reLastReply1;
static GRegex *reLastReply2;
static GRegex *reSubforum;
static GRegex *rePager;

void p_panel_init(void) {
    whitespaces = g_regex_new("^[\\s\\t]+$", 0,0,NULL);
    reTitolo1 = g_regex_new(
        "<a href='m\\.aspx\\?m_id=(\\d+)' "
        "class='fh1' style='font-size:1\\.25em; "
        "color: #0000EE;'>(.*?)</a>"
        , 0, 0,NULL);
    reTitolo2 = g_regex_new("\\(<b>(\\d+)</b>\\)", 0,0,NULL);
    reTitolo3 = g_regex_new(
        "<a href='\\.\\./cerca.asp\\?s=nick%"
        "3a%22.*%22' "
        "class=\\'nick\\'>(.+)</a></b>",0,0,NULL);
    reLastReply1 = g_regex_new(
        "<div><img src='images/trasp\\.gif' "
        "border='0' alt=' ' class='dotto' "
        "style='padding-right: 3px;'>"
        "(di <b><a href='\\.\\./cerca\\.asp\\?s=nick%"
        "3a%22.*%22' "
        "class='nick'>(.*?)</a></b> )? (alle|del) "
        "(.*)</div>",0,0,NULL);
    reLastReply2 = g_regex_new(
            "<div><img src='images/trasp\\.gif' border='0' "
            "alt=' ' class='dotto' style='padding-right: "
            "3px;'>Ultimo: <a href='m\\.aspx\\?m_id=(\\d+)' "
            "class='fh1' style='font-size:1\\.25em; color: "
            "#0000EE;'>(.*?)</a>( di <b><a "
            "href='\\.\\./cerca\\.asp\\?s=nick%"
            "3a%22.*%22' "
            "class='nick'>(.+?)</a></b>)?\\s+(alle|del) (.*)</div>",0,0,NULL);
    reSubforum = g_regex_new(
        "<div><img src='images/trasp\\.gif' border='0' "
        "alt=' ' class='dotto' style='padding-right: "
        "3px;'><a href='hp\\.aspx\\?m_id=\\d+' "
        "style='color: green; font-weight: "
        "bold;'>(.*)</a></div>",0,0,NULL);
    rePager = g_regex_new(
            "<td style=\"FONT-SIZE:0.85em;PADDING-TOP:10px\"> Pagina <b>(\\d+)</b> di <b>(\\d+)</b> - ",0,0,NULL);
}

void p_panel_end(void) {
    g_regex_unref(whitespaces);
    g_regex_unref(reTitolo1);
    g_regex_unref(reTitolo2);
    g_regex_unref(reTitolo3);
    g_regex_unref(reLastReply1);
    g_regex_unref(reLastReply2);
    g_regex_unref(reSubforum);
    g_regex_unref(rePager);
}

char *newline = "\n";

enum stages {
    TITOLO = 0,
    ULTIMO = 1,
    FORUM = 2,
    PAGER = 3
};

panel *p_panel_parse(char *page) {
    panel *panel = new_panel();
    char **lines = g_strsplit(page, newline, 0);
    int n_lines = g_strv_length(lines);
    int stage = TITOLO;
    thread *current;

    for (int i=0;i<n_lines;++i) {
        char *line = lines[i];
        GMatchInfo *gmInfo;

        //salta linee vuote
        g_regex_match(whitespaces, line, 0, &gmInfo);
        if (g_match_info_matches(gmInfo)) {
            g_match_info_free(gmInfo);
            continue;
        } else {
            g_match_info_free(gmInfo);
        }

        switch (stage) {
            case TITOLO:
                //reTitolo1: m_id e titolo del capothread
                g_regex_match(reTitolo1, line, 0, &gmInfo);
                if (g_match_info_matches(gmInfo)) {
                    current = new_thread();
                    current->capothread = new_post();
                    current->capothread->m_id = g_match_info_fetch(gmInfo, 1);
                    current->capothread->titolo = g_match_info_fetch(gmInfo, 2);
                    g_match_info_free(gmInfo);

                    //reTitolo2: n_risposte, se c'è
                    g_regex_match(reTitolo2, line, 0, &gmInfo);
                    if (g_match_info_matches(gmInfo)) {
                        current->n_risposte = atoi( g_match_info_fetch(gmInfo, 1) );
                    }
                    g_match_info_free(gmInfo);

                    //reTitolo3: autore, se c'è
                    g_regex_match(reTitolo3, line, 0, &gmInfo);
                    if (g_match_info_matches(gmInfo)) {
                        if (strlen(g_match_info_fetch(gmInfo, 1)) > 0) {
                            current->capothread->autore = g_match_info_fetch(gmInfo, 1);
                        } else {
                            free(g_match_info_fetch(gmInfo, 1));
                        }
                    }
                    g_match_info_free(gmInfo);

                    stage++;
                } else {
                    // panel con meno di 10 messaggi
                    g_regex_match(rePager, line, 0, &gmInfo);
                    if (g_match_info_matches(gmInfo)) {
                        panel->pagina = atoi(g_match_info_fetch(gmInfo, 1));
                        panel->n_pagine = atoi(g_match_info_fetch(gmInfo, 2));
                        free(g_match_info_fetch(gmInfo,1));
                        free(g_match_info_fetch(gmInfo,2));
                        stage = PAGER;
                        stage++;
                    }
                    g_match_info_free(gmInfo);
                    break;
                }
                break;
            case ULTIMO:
                //reLastReply1: autore se autenticato e un solo msg
                // oppure titolo ultimo msg e autore ultimo e data ultimo
                g_regex_match(reLastReply1, line, 0, &gmInfo);
                if (g_match_info_matches(gmInfo)) {
                    free(g_match_info_fetch(gmInfo, 1));
                    if (    current->capothread->autore == NULL &&
                            g_match_info_fetch(gmInfo, 2) != NULL) {
                        if (strlen(g_match_info_fetch(gmInfo, 2)) > 0) {
                            current->capothread->autore = g_match_info_fetch(gmInfo, 2);
                        } else {
                            free(g_match_info_fetch(gmInfo, 2));
                        }
                    }
                    free(g_match_info_fetch(gmInfo, 3));
                    current->capothread->data_unparsed = g_match_info_fetch(gmInfo, 4);
                } else {
                    g_match_info_free(gmInfo);
                    if (current->n_risposte == 0) {
                        return NULL; //situazione virtualmente impossibile
                    }

                    //reLastReply2: m_id, titolo, autore, data_unparsed dell'ultimo msg
                    g_regex_match(reLastReply2, line, 0, &gmInfo);
                    current->ultimo = new_post();
                    current->ultimo->m_id = g_match_info_fetch(gmInfo, 1);
                    current->ultimo->titolo = g_match_info_fetch(gmInfo, 2);
                    free(g_match_info_fetch(gmInfo,3));
                    if (strlen(g_match_info_fetch(gmInfo, 4)) > 0) {
                        current->ultimo->autore = g_match_info_fetch(gmInfo, 4);
                    } else {
                        free(g_match_info_fetch(gmInfo, 4));
                    }
                    free(g_match_info_fetch(gmInfo, 5));
                    current->ultimo->data_unparsed = g_match_info_fetch(gmInfo, 6);
                }
                g_match_info_free(gmInfo);
                stage++;
                break;
            case FORUM:
                //reSubforum: nome del subforum, se c'è (diverso da Forum dei Troll)
                g_regex_match(reSubforum, line, 0, &gmInfo);
                if (g_match_info_matches(gmInfo)) {
                    current->subforum = g_match_info_fetch(gmInfo, 1);
                }
                panel->threads[(panel->n_threads)] = current;
                panel->n_threads++;
                if (panel->n_threads == 10)
                    stage++;
                else
                    stage = TITOLO;
                break;
            case PAGER:
                //rePager: cerca num pagina corrente e totale
                g_regex_match(rePager, line, 0, &gmInfo);
                if (g_match_info_matches(gmInfo)) {
                    panel->pagina = atoi(g_match_info_fetch(gmInfo, 1));
                    panel->n_pagine = atoi(g_match_info_fetch(gmInfo, 2));
                    free(g_match_info_fetch(gmInfo,1));
                    free(g_match_info_fetch(gmInfo,2));
                    stage++;
                }
                g_match_info_free(gmInfo);
            default:
                break;
        }
    }
    g_strfreev(lines);
    return panel;
}
