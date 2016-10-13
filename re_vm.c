/*
 * instructions: [Char x], [Split x, y], [Jmp x], [Match]
 *
 * (ab)*c =>
 * 0. Split 1, 4
 * 1. Char a
 * 2. Char b
 * 3. Jmp 0
 * 4. Char c
 * 5. Match
 *
 */

#include <stdio.h>
#include <stdlib.h>

enum {
    Char = 1,
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

struct {
    struct instruct *inst;
    int p;
} stack[512];

int sp = 0;

/*
 * This is a backtrack style virtual machine,
 * the vm always execute from the first instruction
 * of the program
 */

int execute(struct program *prog, const char s[]) {
    struct instruct *pc; /* program counter */
    int p; /* sp: string pointer */
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
                    if (pc->ch == s[p]) {
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
                    stack[sp].inst = pc->y;
                    stack[sp].p = p;
                    sp += 1;
                    pc = pc->x;
                    continue;
                case Jmp:
                    pc = pc->x;
                    continue;
                case Match:
                    if (s[p] != '\0')
                        printf("leftover part:%s\n", s + p);
                    else
                        printf("exact matched.\n");
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


int main(int argc, char *argv[]) {
    const char *str = "abababc";
    struct program *prog;
    prog = (struct program *) malloc(sizeof (struct program) + 6 * sizeof (struct instruct));
    if (prog != NULL) {
        struct instruct *i;
        prog->len = 6;
        prog->entry = (struct instruct *)(prog + 1);
        i = prog->entry;
        /* 0 */
        i->op = Split;
        i->x = prog->entry + 1;
        i->y = prog->entry + 4;
        i += 1;

        /* 1 */
        i->op = Char;
        i->ch = 'a';
        i += 1;

        /* 2 */
        i->op = Char;
        i->ch = 'b';
        i += 1;

        /* 3 */
        i->op = Jmp;
        i->x = prog->entry;
        i += 1;

        /* 4 */
        i->op = Char;
        i->ch = 'c';
        i += 1;

        /* 5 */
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


