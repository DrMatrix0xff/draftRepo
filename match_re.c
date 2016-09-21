#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include "diyre.h"

static BOOL match_concat_node(re_node *node, const char **ss);
static BOOL match_alter_node(re_node *node, const char **ss);

BOOL match_re_node(re_node *node, const char s[], int *step) {
    const char *ss = s;
    switch (node->kind) {
        case re_char:
            if (*ss == node->ch) {
                *step = 1;
                return 1;
            } else {
                return 0;
            }
        case re_dot:
            if (*ss != '\0') {
                *step = 1;
                return 1;
            } else {
                return 0;
            }
        case re_concat:
        {
            int res;
            res = match_concat_node(node, &ss);
            *step = ss -s;
            return res;
        }
        case re_alter:
        {
            int res;
            res = match_alter_node(node, &ss);
            *step = ss -s;
            return res;
        }
        case re_repeat:
        default:
            fprintf(stderr, "not implement yet\n");
            exit(1);
    }
}

static BOOL match_concat_node(re_node *node, const char **ss) {
    int step;
    int res;
    assert(node->kind == re_concat);
    res = match_re_node(node->fc, *ss, &step);
    if (res == 0)
        return 0;
    *ss = *ss + step;
    res = match_re_node(node->sc, *ss, &step);
    *ss = *ss + step;
    return res;
}

static BOOL match_alter_node(re_node *node, const char **ss) {
    int step;
    int res;
    assert(node->kind == re_alter);
    res = match_re_node(node->fc, *ss, &step);
    if (res == 1) {
        *ss = *ss + step;
        return 1;
    }
    res = match_re_node(node->sc, *ss, &step);
    *ss = *ss + step;
    return res;
}
