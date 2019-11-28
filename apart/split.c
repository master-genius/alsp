#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "split.h"

void set_null_substr(char *text, int n) {
    for(int i=0; i<n; i++) {
        text[i] = '\0';
    }
}

char * __split_core(char *text, const char *dm, char **save) {

    char *old = NULL;
    char *cur = NULL;

    if (text == NULL)
        text = *save;
    
    if (*text == '\0') {
        *save = text;
        return NULL;
    }
    int n = strlen(dm);

                    start_substr:;
    old = text;
    cur = strstr(text, dm);
    if (cur != NULL) {
        set_null_substr(cur, n);
        if (cur == text) {
            cur = cur + n;
            text = cur;
            goto start_substr;
        }
        *save = cur + n;
        text = *save;
        while((cur = strstr(text, dm))!=NULL && cur==text) {
            set_null_substr(cur, n);
            cur = cur + n;
            text = cur;
            *save = cur;
        }

    } else {
        *save = text + strlen(text);
    }
    return old;
}

char *split(char *text, const char *dm) {

    static char *sr;

    return __split_core(text, dm, &sr);
}

