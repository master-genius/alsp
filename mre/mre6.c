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



int matchs(char *regex, char *text);
int match(char *, char *);
int matchreg(char *, char *);
int matchchar(char, char *, char *);

int matchs(char *regex, char *text) {
    char *sub_regex;
    char *tbuf;

    sub_regex = strtok(regex, "|");
    while(sub_regex!=NULL) {
        tbuf = text;
        if (match(sub_regex, tbuf))
            return 1;
        sub_regex = strtok(NULL, "|");
    }
    return 0;
}

int match(char *regex, char *text) {

    int i=0;

    if (regex[0] == '^')
        return matchreg(regex+1, text);
    else if(strchr(REGEX_CHAR, regex[0]) == NULL) {
        if (regex[i]=='\\')
            i++;

        while((*text != '\0' && !(_REG_OPT[REG_OPT_LNEND] && *text == '\n'))
            &&
            (_REG_OPT[REG_OPT_UPLOW] ? (*text != regex[i]) : (tolower(*text)!=tolower(regex[i])) )
        ) {
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
        && (*text == c || ( _REG_OPT[REG_OPT_UPLOW]==0 && tolower(*text)==tolower(c) ) )
    ) {
        text++;

        if (matchreg(regex+1, text))
            return 1;
        else
            return match(regex,text);
    }

    return 0;
}

#define STD_IN_KYBD     1
#define STD_IN_FIFO     2

#define BUF_LEN     4096


#define REGEX_MAX_LEN   128

#define HELP_INFO   "\n\
    -h : help info\n\
    -v : inverse match\n\
    -i : up low \n"


int main(int argc, char *argv[]) {
    for(int i=0; i< REG_OPT_END; i++)
        _REG_OPT[i] = 0;

    _REG_OPT[REG_OPT_LNEND] = 1;

    if (argc < 2) {
        dprintf(2, "Error: less regexp\n");
        return -1;
    }
    
    int inverse = 0;
    int regex_ind = -1;
    int uplow = 1;
    for(int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            printf("%s\n", HELP_INFO);
            return 0;
        }
        else if (strcmp(argv[i], "-i") == 0) {
            _REG_OPT[REG_OPT_UPLOW] = 1;
        }
        else if (strcmp(argv[i], "-l") == 0) {
            //换行符不作为结尾
            _REG_OPT[REG_OPT_LNEND] = 0;
        }
        else if (strcmp(argv[i], "-v")==0) {
            inverse = 1;
        } else {
            regex_ind = i;
        }
    }

    if (regex_ind < 0) {
        dprintf(2, "Error: unknow regex\n");
        return -1;
    }

    if (strlen(argv[regex_ind]) > REGEX_MAX_LEN) {
        dprintf(2, "Error: too long regex\n");
        return -1;
    }

    int std_in_type = STD_IN_KYBD;
    struct stat st;
    if (fstat(0, &st) < 0) {
        perror("fstat");
        return -1;
    }

    if (S_ISFIFO(st.st_mode) )
        std_in_type = STD_IN_FIFO;


    int (*m)(char*, char*);
    m = match;
    if (strchr(argv[regex_ind], '|'))
        m = matchs;

    char buffer[BUF_LEN] = {'\0'};
    int len;
    char *d;
    char regex[REGEX_MAX_LEN+1] = {'\0'};

    if (std_in_type == STD_IN_KYBD) {
        while(1) {
            if(fgets(buffer, BUF_LEN-1, stdin)==NULL) {
                perror("fgets");
                continue;
            }

            len = strlen(buffer);
            if (buffer[len-1]=='\n')
                buffer[len-1] = '\0';
            strcpy(regex, argv[regex_ind]);
            if (m(regex, buffer)) {
                printf("match: %s\n", buffer);
            } else {
                printf("[%s] not match\n", buffer);
            }
        }
    } else if (std_in_type == STD_IN_FIFO) {

      read_match:;
        d = fgets(buffer, BUF_LEN-1, stdin);
        if (d == NULL)
            goto end_match;

        strcpy(regex, argv[regex_ind]);
        if (m(regex, buffer)) {
            if (inverse == 0)
                printf("%s", buffer);
        } else if (inverse) {
            printf("%s", buffer);
        }

        goto read_match;
    }

end_match:;
	return 0;
}

