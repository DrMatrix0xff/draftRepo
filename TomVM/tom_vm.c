#include <stdio.h>
#include <stdlib.h>

#include "../diyre.h"

typedef struct {
    int n;
    int *entry_offset;
} *List;

List clist, nlist;

static void init_list(int len) {

    clist = (List) malloc(sizeof (*clist) + len * sizeof (int));
    clist->n = 0;
    clist->entry_offset = (int *)(clist + 1);

    nlist = (List) malloc(sizeof (*nlist) + len * sizeof (int));
    nlist->n = 0;
    nlist->entry_offset = (int *)(nlist + 1);

}

#define add_to_list(ls, inst) do { \
    int i__; \
    for (i__ = 0; i__ < ls->n; i__+=1) { \
        if (ls->entry_offset[i__] == ((inst) - prog->entry)) break; \
    } \
    if (i__ >= ls->n) { \
    ls->entry_offset[ls->n] = ((inst) - prog->entry); \
    ls->n += 1; \
    } \
} while (0)

int execute(struct program *prog, const char s[]) {
    int p, i;
    struct instruct *pc;

    init_list(prog->len);

    pc = prog->entry; // no offset from prog entry 
    add_to_list(clist, pc);

    for (p = 0; s[p]; p += 1) {
        for (i = 0; i < clist->n; i += 1) {
            pc = prog->entry + clist->entry_offset[i];
            switch (pc->op) {
                case Char:
                    if (pc->ch == s[p])
                        add_to_list(nlist, pc+1);
                    break;
                case Dot:
                    add_to_list(nlist, pc+1);
                    break;
                case Jmp:
                    add_to_list(clist, pc->x);
                    break;
                case Match:
                    return 1;
                case Split:
                    add_to_list(clist, pc->x);
                    add_to_list(clist, pc->y);
                    break;
                default:
                    fprintf(stderr, "unknown kind instruction\n");
                    exit(1);
            }
        }
        {
            List l;
            l = clist; clist = nlist; nlist = l;
        }
        nlist->n = 0;
    }
    return 0;
}
