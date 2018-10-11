#ifndef MREGEX_H
#define MREGEX_H


#define REGEX_CHAR  ".^*$"

int matchs(char *regex, char *text, int uplow);
int match(char *, char *, int);
int matchreg(char *, char *, int);
int matchchar(char, char *, char *, int);

#endif

