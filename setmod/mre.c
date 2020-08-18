#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>

#define REGEX_CHAR  ".^*$"
//选项：是否把换行符作为结尾
#define REG_OPT_LNEND   0
#define REG_OPT_UPLOW   1
#define REG_OPT_END     4

int _REG_OPT[REG_OPT_END];
char* start_regex = NULL;

int matchs(char *regex, char *text);
int match(char *, char *);
int matchreg(char *, char *);
int matchchar(char, char *, char *);
int gotomatch(char* regex, char* text);


int matchs(char *regex, char *text) {
    char *sub_regex;
    char *tbuf;

    sub_regex = strtok(regex, "|");

    while(sub_regex!=NULL) {
        tbuf = text;
        if (match(sub_regex, tbuf)) {
            return 1;
		}
        sub_regex = strtok(NULL, "|");
        start_regex = NULL;
    }
    return 0;
}

int match(char *regex, char *text) {

    if (start_regex == NULL) {
        start_regex = regex;
    }

    if (regex[0] == '^') {
        return matchreg(regex+1, text);
    }
	
	return gotomatch(regex, text);
}

int gotomatch(char* regex, char* text) {

    int i = 0;

    if (strchr(REGEX_CHAR, regex[0]) == NULL) {

        if (regex[i]=='\\') {
            i++;
		}

        while((*text != '\0' && !(_REG_OPT[REG_OPT_LNEND] && *text == '\n'))
          &&
          (_REG_OPT[REG_OPT_UPLOW]?(*text != regex[i]):(tolower(*text)!=tolower(regex[i])) ) ) {

            text++;
        }
    }

    return matchreg(regex, text);
}

int matchreg(char *regex, char *text) {

    if (regex[0] == '\0')
        return 1;

    if (regex[0] == '\\') {
        return matchchar(regex[1], regex+1, text);
    }

    if (regex[0] == '$' && regex[1] == '\0') {
        return (*text == '\0' || (_REG_OPT[REG_OPT_LNEND] && *text == '\n') );
    }

    if (regex[1] == '*') {
        if (regex[0]=='.') {
            while(*text!='\0') {
                if (matchreg(regex+2, text))
                    return 1;
                text++;
            }
        } else {
            char c = regex[0];
            char *tbuf = text;
            while(*tbuf != '\0') {
                if (matchreg(regex+2, tbuf)) {
                    while((*text!='\0')
                        && text!=tbuf 
                        && (*text==c || (_REG_OPT[REG_OPT_UPLOW]==0 && tolower(*text)==tolower(c)) )
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
        return matchreg(regex+2, text);
    }

    if (regex[0] == '.' && (*text!='\0')) {
        text++;
        return matchreg(regex+1, text);
    }

    return matchchar(regex[0], regex, text);
}

int matchchar(char c, char *regex, char *text) {
    while((*text != '\0')
      && (*text == c || ( _REG_OPT[REG_OPT_UPLOW]==0 && tolower(*text)==tolower(c) ) ) ) {

        text++;

        if (matchreg(regex+1, text)) {
            return 1;
		} else {
            if (regex == start_regex) {
                return gotomatch(regex,text);
            }
            return 0;
        }
    
    }

    return 0;
}

