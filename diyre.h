/*
regex = reterm '|' regex.
regex = reterm.

reterm = refactor reterm
reterm = refactor

refactor = reatom['*']

reatom = letter | '.' | '(' regex ')'

a* => (repeat (char a))
a.b* => (concat (concat (char a) dot) (repeat (char b)))

(ab)* => (repeat (concat (char a) (char b)))

ab*|cc* => (alternative (concat (char a) (repeat (char b))) (concat (char c) (repeat (char c))))
*/
#ifndef DIY_RE_H
#define DIY_RE_H

typedef enum {
    re_char=1,
    re_concat,
    re_repeat,
    re_alter,
    re_dot
} re_kind;

struct node__ {
    re_kind kind;
    char ch;
    struct node__ *fc; /* fc short for First Child */
    struct node__ *sc; /* sc short for Second Child */
};

typedef struct node__ re_node;

extern re_node the_dot_node;
extern re_node *parse_re_exp(const char s[], int *step);
extern void print_re_node(re_node *root, int level);
#endif
