#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mre.h"


int matchs(char *regex, char *text, int uplow) {
    char *sub_regex;
    char *tbuf;

    sub_regex = strtok(regex, "|");
    while(sub_regex!=NULL) {
        tbuf = text;
        if (match(sub_regex, tbuf, uplow))
            return 1;
        sub_regex = strtok(NULL, "|");
    }
    return 0;
}

int match(char *regex, char *text, int uplow) {

    int i=0;

    if (regex[0] == '^')
        return matchreg(regex+1, text, uplow);
    else if(strchr(REGEX_CHAR, regex[0]) == NULL) {
        if (regex[i]=='\\')
            i++;

        while(*text != '\0' 
            &&
            (uplow ? (*text != regex[i]) : (tolower(*text)!=tolower(regex[i])) )
        ) {
            text++;
        }
    }
    return matchreg(regex, text, uplow);
}

int matchreg(char *regex, char *text, int uplow) {

    if (regex[0] == '\0')
        return 1;

    if (regex[0] == '\\') {
        return matchchar(regex[1], regex+1, text, uplow);
    }

    if (regex[0] == '$' && regex[1] == '\0')
        return *text == '\0';

    if (regex[1] == '*') {
        if (regex[0]=='.') {
            while(*text!='\0') {
                if (matchreg(regex+2, text, uplow))
                    return 1;
                text++;
            }
        } else {
            char c = regex[0];
            char *tbuf = text;
            while(*tbuf != '\0') {
                if (matchreg(regex+2, tbuf, uplow)) {
                    while(*text!='\0'
                        && text!=tbuf 
                        && (*text==c || (uplow==0 && tolower(*text)==tolower(c)) )
                    ) {
                        text++;
                    }

                    if (text==tbuf)
                        return 1;

                    return 0;
                }
                tbuf++;
            }
        }
        return matchreg(regex+2, text, uplow);

    }

    if (regex[0] == '.' && *text!='\0') {
        text++;
        return matchreg(regex+1, text, uplow);
    }

    return matchchar(regex[0], regex, text, uplow);
}

int matchchar(char c, char *regex, char *text, int uplow) {
    while(*text != '\0' 
        && (*text == c || ( uplow==0 && tolower(*text)==tolower(c) ) )
    ) {
        text++;

        if (matchreg(regex+1, text, uplow))
            return 1;
        else
            return matchreg(regex,text, uplow);
    }

    return 0;
}

