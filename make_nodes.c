#include <stdio.h>
#include <stdlib.h>

#include "diyre.h"

re_node *make_char_node(char c) {
    re_node *p;
    p = (re_node *) calloc(1, sizeof(re_node));
    if (p != NULL) {
        p->kind = re_char;
        p->ch = c;
    }
    return p;
}

re_node *make_concat_node(re_node *sub1, re_node *sub2) {
    re_node *p;
    p = (re_node *) calloc(1, sizeof(re_node));
    if (p != NULL) {
        p->kind = re_concat;
        p->fc = sub1;
        p->sc = sub2;
    }
    return p;
}

re_node *make_alter_node(re_node *sub1, re_node *sub2) {
    re_node *p;
    p = (re_node *) calloc(1, sizeof(re_node));
    if (p != NULL) {
        p->kind = re_alter;
        p->fc = sub1;
        p->sc = sub2;
    }
    return p;
}

re_node *make_repeat_node(re_node *sub) {
    re_node *p;
    p = (re_node *) calloc(1, sizeof(re_node));
    if (p != NULL) {
        p->kind = re_repeat;
        p->fc = sub;
    }
    return p;
}


