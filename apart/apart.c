#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "vlist.h"

#define APART_MAX_SIZE       8192

#define TEXT_MAX_SIZE        65536

#define APART_MAX       9

#define STD_IN_KYBD     1
#define STD_IN_FIFO     2



void * apart_add(void*data, void*idata) {
    strncpy((char*)data, (char*)idata, APART_MAX_SIZE);
}


int apart_text(vnode_list *vh, char *text, char *t) {
    char *find;

    find = strtok(text, t);
    while (find) {
        if (vnode_list_add(vh, strlen(find)+1, (void*)find, apart_add) == NULL)
            return -1;
        find = strtok(NULL, t);
    }

    return 0;
}

#define ARGS_OALL       0
#define ARGS_COMCHR     1
#define ARGS_FROMTO     2

#define ARGS_END        4

char __args[ARGS_END]   = {'\0'};
unsigned int __col_apart[APART_MAX] = {0,};


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


    for(int i=1; i<argc; i++) {
        if (argv[i][0] == '$') {
        
        } else if (strcmp(argv[i], "-C") == 0) {
        
        } else if () {
        
        } else 
    }

    char buffer[TEXT_MAX_SIZE+1] = {'\0'};
    
  start_apart:;
    d = fgets(buffer, TEXT_MAX_SIZE - 1, stdin);
    if (d == NULL)
        goto end_apart;
    
    if (apart_text(&vh, text, t) < 0)
        goto end_apart;
    
    out_apart(&vh);

    goto start_apart;
  
  end_apart:; 
    vnode_list_free(&vh, FREE_DATA);

	return 0;
}

