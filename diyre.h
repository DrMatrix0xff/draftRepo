/*
regex = reterm '|' regex.
regex = reterm.

reterm = refactor reterm
reterm = refactor

refactor = reatom['*'|'?']

reatom = letter | '.' | '(' regex ')'

a* => (repeat (char a))
a.b* => (concat (concat (char a) dot) (repeat (char b)))

(ab)* => (repeat (concat (char a) (char b)))

ab*|cc* => (alternative (concat (char a) (repeat (char b))) (concat (char c) (repeat (char c))))
*/

#ifndef DIY_RE_H
#define DIY_RE_H

enum {
    Char = 1,
    Dot,
    Split,
    Jmp,
    Match
};

struct instruct {
    int op;
    int ch; /* for Char */
    struct instruct *x;
    struct instruct *y;
};

struct program {
    struct instruct *entry;
    int len;
};

typedef enum {
    re_char=1,
    re_concat,
    re_repeat,
    re_optional,
    re_plus,
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

extern re_node *parse_re_exp(const char s[]);
extern void print_re_node(re_node *root, int level);
extern struct program *compile(re_node *node);
extern void print_prog_code(struct program *p);
extern int execute(struct program *p, const char s[]);

extern re_node *make_alter_node(re_node *sub1, re_node *sub2);
extern re_node *make_concat_node(re_node *sub1, re_node *sub2);
extern re_node *make_char_node(char c);
extern re_node *make_repeat_node(re_node *sub1);
extern re_node *make_plus_node(re_node *sub1);
extern re_node *make_optional_node(re_node *sub1);
extern re_node *make_dot_node(void);

#endif


