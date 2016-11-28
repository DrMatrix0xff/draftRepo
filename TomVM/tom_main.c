#include <stdio.h>
#include <stdlib.h>

#include "../diyre.h"

int main(int argc, char *argv[]) {
    const char *patt = "(ab*)+ac.";
    const char *str = "aaaaacdxyz";
    // const char *str = "cd";
    // const char *patt = "ab*(c|C(a(b)))|d*e";
    re_node *rt;
    struct program *prog;
    rt = parse_re_exp(patt);
    print_re_node(rt, 1);

    prog = compile(rt);
    print_prog_code(prog);
    if (execute(prog, str) != 0)
        printf("re pattern: %s, string: %s, match.\n", patt, str);
    else
        printf("re pattern: %s, string: %s, not match.\n", patt, str);
    return 0;
}
