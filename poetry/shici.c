#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

char *_tao_hua_shan[] = {
    "《桃花扇》",
    "秦淮无语话斜阳，",
    "家家临水应红妆。",
    "春风不知玉颜改，",
    "依旧欢歌绕画舫。",
    "月明人断肠！",
    "\0"
};

char *_tao_hua_an_ge[] = {
    "《桃花庵歌》",
    "桃花坞里桃花庵，",
    "桃花庵下桃花仙。",
    "桃花仙人种桃树，",
    "又摘桃花换酒钱。",
    "\0"
};

char *_die_lian_hua[] = {
    "《蝶恋花 春景》",
    "花褪残红青杏小。燕子飞时，绿水人家绕。",
    "枝上柳绵吹又少。天涯何处无芳草！",
    "   ",
    "墙里秋千墙外道。墙外行人，墙里佳人笑。",
    "笑渐不闻声渐悄。多情却被无情恼。 ",
    "\0"
};
char *_feng_qiao_ye_bo[] = {
    "《枫桥夜泊》",
    "月落乌啼霜满天，",
    "江枫渔火对愁眠。",
    "姑苏城外寒山寺，",
    "夜半钟声到客船。",
    "\0"
};

char **_shici[] = {
    _tao_hua_shan, 
    _tao_hua_an_ge, 
    _die_lian_hua,
    _feng_qiao_ye_bo,
    NULL
};

int * winsize(int *ws) {
    ws[0] = ws[1] = 0;

    struct winsize _wz;
    ioctl (1, TIOCGWINSZ, &_wz);

    if (_wz.ws_col > 0)
        ws[1] = _wz.ws_col;
    if (_wz.ws_row > 0)
        ws[0] = _wz.ws_row;

    return ws;
}

void clear() {
    printf("\x1b[2J\x1b[;H");
}

void move(int row, int column) {
    printf("\x1b[%d;%dH", row, column);
}

void mvoutstr(int row, int column, char *s) {
    move(row, column);
    printf("%s", s);
    fflush(stdout);
}

void mvoutc(int row, int column, char c) {
    move(row, column);
    printf("%c", c);
    fflush(stdout);
}

void enable_scroll() {
    printf("\x1b[r");
}

void scroll_down(int lines) {
    for(int i=0; i<lines; i++) {
        printf("\033D");
    }
}

void scroll_up(int lines) {
    for(int i=0; i<lines; i++) {
        printf("\x1bM");
    }
}


int main(int argc, char *argv[]) {

    enable_scroll();

    int ws[2];
    winsize(ws);

    int row = 2;
    int width = 2;
    int move_size = 3;
    int len = 0;
    int i = 0;
    int col;
    char word[5] = {'\0'};
    int k = 0;
    char **sci;

    clear();

start_out:;
    
    sci = _shici[k];
    if(sci == NULL)
        goto end_out;

    i=0;

    while(sci[i][0]!='\0') {
        len = strlen(sci[i]);
        col = 8;
        for (int j=0; j<len; j+=move_size) {
            if (sci[i][j]>0) {
                while(j<len) {
                    if (sci[i][j]<0)
                        break;
                    mvoutc(row, col, sci[i][j]);
                    col++;
                    j++;
                }
            }
            strncpy(word, sci[i]+j, 3);
            move(row, col);
            printf("%s", word);
            fflush(stdout);
            col += width;
            usleep(100000);
            winsize(ws);
        }
        row++;
        if (row > ws[0])
            scroll_down(1);
        i++;
    }
    if (row > ws[0])
        scroll_down(1);
    mvoutstr(row++, 1, "  ");
    row++;
    if (row > ws[0])
        scroll_down(1);
    k++;

    if (_shici[k]!=NULL)
        goto start_out;

end_out:;

	return 0;
}

