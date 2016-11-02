/*
 * instructions: [Char x], [Split x, y], [Jmp x], [Match]
 *
 * (ab)*a =>
 * 0. Split 1, 4
 * 1. Char a
 * 2. Char b
 * 3. Jmp 0
 * 4. Char a
 * 5. Match
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "diyre.h"
/*
enum {
    Char = 1,
    Dot,
    Split,
    Jmp,
    Match
};

struct instruct {
    int op;
    int ch; // for Char
    struct instruct *x;
    struct instruct *y;
};

struct program {
    struct instruct *entry;
    int len;
};
*/

static struct {
    struct instruct *inst;
    int p;
} stack[512], array[1000];

static int sp = 0, ap = 0;

/*
 * This is a backtrack style virtual machine,
 * the vm always execute from the first instruction
 * of the program
 */

static int search_array(struct instruct *ip, int cp) {
    int i;
    for (i = 0; i < ap; i++) {
        if (array[i].inst == ip && array[i].p == cp)
            break;
    }
    return i;
}

int execute(struct program *prog, const char s[]) {
    struct instruct *pc; /* program counter */
    int p; /* p: text cursor of s */
    int i;
    stack[sp].inst = prog->entry;
    stack[sp].p = 0;
    sp += 1;
    while (sp >= 1) {
        sp -= 1;
        pc = stack[sp].inst;
        p = stack[sp].p;
        for ( ; ; ) {
            switch (pc->op) {
                case Char:
                case Dot:
                    if (pc->ch == s[p] || (pc->ch == '.' && s[p] != '\0')) {
                        pc += 1;
                        p += 1;
                        continue;
                    } else
                        goto Break;
                case Split:
                    if (sp >= 512) {
                        fprintf(stderr, "stack overflow\n");
                        exit(1);
                    }
                    if ((i = search_array(pc->y, p)) >= ap) {
                        array[ap].inst = stack[sp].inst = pc->y;
                        array[ap].p = stack[sp].p = p;
                        sp += 1;
                        ap += 1;
                    }
                    pc = pc->x;
                    continue;
                case Jmp:
                    pc = pc->x;
                    continue;
                case Match:
                    if (s[p] != '\0')
                        printf("leftover part: %s\n", s + p);
                    else
                        printf("exact matched.\n");
                    for (i = 0; i < ap; i++)
                        fprintf(stderr, "pc: %d, p: %d\n", (int)(array[i].inst - prog->entry), array[i].p);
                    return 1;
                default:
                    fprintf(stderr, "op code of unknown kind\n");
                    exit(1);
            }
        }
    Break:
        ;
    }
    return 0;
}

/*
int main(int argc, char *argv[]) {
    const char *str = "ababababcca";
    struct program *prog;
    prog = (struct program *) malloc(sizeof (struct program) + 6 * sizeof (struct instruct));
    if (prog != NULL) {
        struct instruct *i;
        prog->len = 6;
        prog->entry = (struct instruct *)(prog + 1);
        i = prog->entry;
        // 0
        i->op = Split;
        i->x = prog->entry + 1;
        i->y = prog->entry + 4;
        i += 1;

        // 1
        i->op = Char;
        i->ch = 'a';
        i += 1;

        // 2
        i->op = Char;
        i->ch = 'b';
        i += 1;

        // 3
        i->op = Jmp;
        i->x = prog->entry;
        i += 1;

        // 4
        i->op = Dot;
        i->ch = '.';
        i += 1;

        // 5
        i->op = Match;
        if (execute(prog, str) != 0) {
            printf("result of vm execution: matched\n");
        } else {
            printf("result of vm execution: not matched\n");
        };
        free(prog);
    }
    return 0;
}
*/


