#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "vlist.h"
#include "split.h"

#define APART_MAX_SIZE       8192

#define TEXT_MAX_SIZE        16384

#define APART_MAX       64

#define STD_IN_KYBD     1
#define STD_IN_FIFO     2

#define ARGS_OALL       0
#define ARGS_COMCHR     1
#define ARGS_FROMTO     2
#define ARGS_NEXTLINE   3
#define ARGS_JOIN       4

#define ARGS_END        6

#define APART_SPLIT     1

#define HELP_INFO   "\
    apart : 分割字符串命令\n\
        主要用于脚本处理时分割字符串并提取指定列的情况，awk很强大，不仅仅是分割文本这么简单，\n\
        但是awk使用过于复杂，初学者需要花费很多时间，此命令十分简单，仅仅是分割一段文本。\n\
        一行字符串长度不能超过16384，分割最大支持64列。\n\
        \n\
        -d '[STRING]' : 指定要对文本分割的字符集。（在其中的字符都符合要求）\n\
        -D '[STRING]' : 指定要对文本分割的字串。（和高级语言提供的分割类似）\n\
        -n            : 处理一行之后输出换行，默认是不换行的。\n\
        -f [NUMBER]   : 指定要输出的列\n\
        -j [CHAR]     : 指定要使用的连接字符，默认为空会把所有列连接成一个字符串\n\
    example:\n\
        ps -e -o user,pid,ppid,comm | egrep 'root.*nginx' | apart -d ' ' -f 2\n\
        echo 123abc234abc345abc456 | apart -D 'abc' -j '-' -f 1,2,3\n\
"


char __args[ARGS_END]   = {'\0'};
unsigned int __col_apart[APART_MAX+1] = {0,};
int __max_field = 1;


void * apart_add(void*data, void*idata) {
    strncpy((char*)data, (char*)idata, APART_MAX_SIZE);
}


int apart_text(struct vnode_list *vh, char *text, char *t, int flag) {

    char *find;

    char *(*split_func)(char *, const char *);
    split_func = strtok;

    if (flag == APART_SPLIT) {
        split_func = split;
    }

    find = split_func(text, t);
    while (find) {
        if (vnode_list_add(vh, 0, (void*)find, NULL) == NULL)
            return -1;
        find = split_func(NULL, t);
    }
    return 0;
}

void out_apart(struct vnode_list *vh) {
    struct vnode_list *v = vh->next;
    int i=1;
    while(v) {
        if (i <= APART_MAX && __col_apart[i] == 1) {
			if (v->next == NULL || i == __max_field || __args[ARGS_JOIN] == '\0') {
				printf("%s", (char*)v->data);
			} else {
				printf("%s%c", (char *)v->data, __args[ARGS_JOIN]);
			}
        }
        i++;
        v = v->next;
    }
    if (vh->next) {
        if (__args[ARGS_NEXTLINE])
            printf("\n");
    }
}

void help(){
    printf("%s\n", HELP_INFO);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        help(); return 0;
    }

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

    int split_flag = 0;
    int apart_ind = 0;
    char *t = NULL;
    int count;
    char *field;
    for(int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            help(); return 0;
        }else if (strcmp(argv[i], "-j") == 0) {
            i += 1;
            if (i >= argc) {
                dprintf(2, "Error: less join char\n");
                return -1;
            }
            __args[ARGS_JOIN] = argv[i][0];
            continue;
        } else if (strcmp(argv[i], "-f") == 0) {
            i++;
            if (i >= argc) {
                dprintf(2, "Error: no fields\n");
                return -1;
            }
            field = strtok(argv[i], ",");
            while(field) {
                apart_ind = atoi(field);
                if (apart_ind <=0 || apart_ind > APART_MAX ) {
                    dprintf(2, "Error: field out of limits\n");
                    return -1;
                }
                if (__max_field < apart_ind)
                    __max_field = apart_ind;
                __col_apart[apart_ind] = 1;
                count ++;
                field = strtok(NULL, ",");
            }
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-D") == 0) {
            if (t) {
                dprintf(2, "Error: already set delim char\n");
                return -1;
            }
            if (argv[i][1] == 'D')
                split_flag = APART_SPLIT;
            i++;
            if (i >= argc) {
                dprintf(2, "Error: -d/D must with delim char\n");
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

    if (apart_text(&vh, buffer, t, split_flag) < 0)
        goto end_apart;

    out_apart(&vh);

    goto start_apart;

  end_apart:;
    vnode_list_free(&vh, 0);

	return 0;
}

