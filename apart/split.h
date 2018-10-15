#ifndef APART_SPLIT_H
#define APART_SPLIT_H

void set_null_substr(char *text, int n);

char *__split_core(char *text, const char *dm, char **save);

char *split (char *text, const char *dm);


#endif

