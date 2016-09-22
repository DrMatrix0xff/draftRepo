#include <stdio.h>
#include <stdlib.h>

#include "diyre.h"

re_node the_dot_node = {.kind=re_dot, .ch='.', .fc=NULL, .sc=NULL};

int main(int argc, char *argv[]) {
    const char *patt = "c(a|b.)*";
    // const char *patt = "abc.|C(a|d)n.d.";
    // const char *patt = "C(a|d)n.d.";
    re_node *rt;
    int ii;
    rt = parse_re_exp(patt, &ii, 0);
    print_re_node(rt, 1);
    return 0;
}

