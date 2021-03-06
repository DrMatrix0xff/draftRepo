#include <stdio.h>
#include <stdlib.h>

#include "diyre.h"

#define nil ((void *)0)

static struct instruct code[201];
static int ci = 0; /* code instruction index */

static void emit(re_node *node);

struct program *compile(re_node *node) {
    struct program *p;
    p = (struct program *) malloc(sizeof *p);
    if (p != nil) {
        emit(node);
        code[ci].op = Match;
        ci++;
        p->len = ci;
        p->entry = code;
    }
    return p;
}

static void emit(re_node *node) {
    if (ci >= 200) {
        fprintf(stderr, "re exp overflow\n");
        exit(1);
    }
    switch (node->kind) {
        case re_char:
            code[ci].op = Char;
            code[ci].ch = node->ch;
            ci += 1;
            break;
        case re_dot:
            code[ci].op = Dot;
            code[ci].ch = '.';
            ci += 1;
            break;
        case re_concat:
            emit(node->fc);
            emit(node->sc);
            break;
        /*
         * code(e1|e2) =>
         * 
         * Split L1, L2
         * L1: code(e1)
         * Jmp L3
         * L2: code(e2)
         * L3:
         *
         */
        case re_alter:
            {
                int i = ci;
                code[ci].op = Split;
                code[ci].x = code + (ci + 1);
                ci += 1;
                emit(node->fc);
                code[ci++].op = Jmp;
                code[i].y = code + ci;
                i = ci - 1;
                emit(node->sc);
                code[i].x = code + ci;
            }
            break;
        /*
         * code(e*)
         *
         * L0: Split L1, L2
         * L1: code(e)
         * Jmp L0
         * L2:
         *
         */
        case re_repeat:
            {
                int i = ci;
                code[ci].op = Split;
                code[ci].x = code + (ci + 1);
                ci += 1;
                emit(node->fc);
                code[ci].op = Jmp;
                code[ci].x = code + i;
                ci += 1;
                code[i].y = code + ci;
            }
            break;
        /*
         * code(e?)
         *
         * L0: Split L1, L2
         * L1: code(e)
         * L2:
         *
         */
        case re_optional:
            {
                int i = ci;
                code[ci].op = Split;
                code[ci].x = code + (ci + 1);
                ci += 1;
                emit(node->fc);
                code[i].y = code + ci;
            }
            break;
        /*
         * code(e+)
         *
         * L0: code(e)
         * L1: Split L0, L2
         * L2:
         *
         */
        case re_plus:
            {
                int i = ci;
                emit(node->fc);
                code[ci].op = Split;
                code[ci].x = code + i;
                code[ci].y = code + (ci + 1);
                ci += 1;
            }
            break;
        default:
            fprintf(stderr, "do not know how to emit\n");
            exit(1);
    }
}

void print_prog_code(struct program *p) {
    int i;
    struct instruct *inst;
    for (i = 0; i < p->len; i++) {
        inst = p->entry + i;
        switch (inst->op) {
            case Char:
                printf("%d. Char %c\n", i, inst->ch);
                break;
            case Dot:
                printf("%d. Dot\n", i);
                break;
            case Split:
                printf("%d. Split %d, %d\n", i, (int)(inst->x - p->entry), (int)(inst->y - p->entry));
                break;
            case Jmp:
                printf("%d. Jmp %d\n", i, (int)(inst->x - p->entry));
                break;
            case Match:
                printf("%d. Match\n", i);
                break;
            default:
                fprintf(stderr, "unknown kind of instruction.\n");
                exit(1);
        }
    }
}


