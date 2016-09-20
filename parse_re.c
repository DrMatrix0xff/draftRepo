#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "diyre.h"

static re_node *parse_re_term(const char **ss);
static re_node *parse_re_factor(const char **ss);
static re_node *parse_re_atom(const char **ss);

re_node *make_alter_node(re_node *sub1, re_node *sub2);
re_node *make_concat_node(re_node *sub1, re_node *sub2);
re_node *make_char_node(char c);
re_node *make_repeat_node(re_node *sub1);

re_node *parse_re_exp(const char s[], int *step) {
    re_node *p, *pp;
    const char *ss = s;
    p = parse_re_term(&ss);
    while (*ss == '|') {
        ss++;
        pp = parse_re_term(&ss);
        p = make_alter_node(p, pp);
    }
    assert(*ss == '\0' || *ss == ')');
    *step = ss - s;
    return p;
}

static re_node *parse_re_term(const char **ss) {

    re_node *p, *pp;
    p = parse_re_factor(ss);
    while (**ss != '|' && **ss != '\0' && **ss != '(' && **ss != ')') {
        /* ss++; */
        pp = parse_re_factor(ss);
        p = make_concat_node(p, pp);
    }
    return p;

}

static re_node *parse_re_factor(const char **ss) {

    re_node *p;
    p = parse_re_atom(ss);
    if (**ss == '*') {
        *ss = *ss + 1;
        p = make_repeat_node(p);
    }
    return p;
}

static re_node *parse_re_atom(const char **ss) {

    re_node *p;
    char c;
    int step;
    c = **ss;
    if (isalpha(c)) {
        *ss = *ss + 1;
        p = make_char_node(c);
    } else if (c == '.') {
        *ss = *ss + 1;
        p = &the_dot_node;
    } else if (c == '(') {
        *ss = *ss + 1;
        p = parse_re_exp(*ss, &step);
        *ss = *ss + step;
        if (**ss == ')')
            *ss = *ss + 1;
        else {
            fprintf(stderr, "error: ) expected, got, %c\n", **ss);
            exit(1);
        }
    } else {
        fprintf(stderr, "error: unknown character, %c.\n", c);
        exit(1);
    }
    return p;

}


void print_re_node(re_node *root, int level) {

    char *indent;
    int i;
    indent = (char *) calloc(level*2+1, sizeof(char));
    for (i = 0; i < 2*level; i++) {
        indent[i] = '\40';
    }
    indent[i] = '\0';

    switch (root->kind) {
        case re_dot:
            fprintf(stdout, "%s(dot)\n", indent);
            break;
        case re_char:
            fprintf(stdout, "%s(char %c)\n", indent, root->ch);
            break;
        case re_concat:
            fprintf(stdout, "%s(concat\n", indent);
            print_re_node(root->fc, level+1);
            print_re_node(root->sc, level+1);
            fprintf(stdout, "%s)\n", indent);
            break;
        case re_repeat:
            fprintf(stdout, "%s(repeat\n", indent);
            print_re_node(root->fc, level+1);
            fprintf(stdout, "%s)\n", indent);
            break;
        case re_alter:
            fprintf(stdout, "%s(alternative\n", indent);
            print_re_node(root->fc, level+1);
            print_re_node(root->sc, level+1);
            fprintf(stdout, "%s)\n", indent);
            break;
        default:
            fprintf(stderr, "error, node of unknown kind\n");
            exit(1);
    }
    free(indent);
}

