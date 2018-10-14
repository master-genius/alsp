#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "vlist.h"

#define APART_MAX_SIZE       8192

#define TEXT_MAX_SIZE        65536

#define APART_MAX       64

#define STD_IN_KYBD     1
#define STD_IN_FIFO     2

#define ARGS_OALL       0
#define ARGS_COMCHR     1
#define ARGS_FROMTO     2
#define ARGS_NEXTLINE   3

#define ARGS_END        6

char __args[ARGS_END]   = {'\0'};
unsigned int __col_apart[APART_MAX+1] = {0,};


void * apart_add(void*data, void*idata) {
    strncpy((char*)data, (char*)idata, APART_MAX_SIZE);
}


int apart_text(struct vnode_list *vh, char *text, char *t) {
    
    char *find;

    find = strtok(text, t);
    while (find) {
        if (vnode_list_add(vh, 0, (void*)find, NULL) == NULL)
            return -1;
        find = strtok(NULL, t);
    }
    return 0;
}

void out_apart(struct vnode_list *vh) {
    struct vnode_list *v = vh->next;
    int i=1;
    while(v) {
        if (i <= APART_MAX && __col_apart[i] == 1) {
            printf("%s ", (char *)v->data);
        }
        i++;    
        v = v->next;
    }
    if (__args[ARGS_NEXTLINE])
        printf("\n");
}


int main(int argc, char *argv[]) {

    struct vnode_list vh;
    vnode_list_init(&vh);

    int std_in_type = STD_IN_KYBD;
    struct stat st;
    if (fstat(0, &st) < 0) {
        perror("fstat");
        return -1;
    }

    if (S_ISFIFO(st.st_mode) )
        std_in_type = STD_IN_FIFO;
    else
        return 0;

    int apart_ind = 0;
    char *t = NULL;
    int count;
    for(int i=1; i<argc; i++) {
        if (argv[i][0] == 'F') {
            apart_ind = atoi(argv[i]+1);
            if (apart_ind <=0 || apart_ind > APART_MAX ) {

                return -1;
            }
            __col_apart[apart_ind] = 1;
            count ++;
        } else if (strcmp(argv[i], "-d") == 0) {
            if (t) {
                dprintf(2, "Error: already set delim char\n");
                return -1;
            }
            i++;
            if (i >= argc) {
                dprintf(2, "Error: -d must with delim char\n");
                return -1;
            }
            t = argv[i];
            continue;
        } else if (strcmp(argv[i], "-n") == 0) {
            __args[ARGS_NEXTLINE] = 1;
        } else {
        
        }
    }

    if (t == NULL) {
        dprintf(2, "Error: no delim char \n");
        return -1;
    }

    if (count <= 0) {
        dprintf(2, "Error: no fields\n");
        return -1;
    }

    char buffer[TEXT_MAX_SIZE+1] = {'\0'};
    char *errb;

  start_apart:;
    errb = fgets(buffer, TEXT_MAX_SIZE - 1, stdin);
    if (errb == NULL)
        goto end_apart;

    if (apart_text(&vh, buffer, t) < 0)
        goto end_apart;

    out_apart(&vh);

    goto start_apart;

  end_apart:;
    vnode_list_free(&vh, 0);

	return 0;
}

