#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <pthread.h>


char *_luoxia = "落霞与孤鹜齐飞";
char *_quishui = "秋水共长天一色";

char *_twgx [] = {
    "豫章故郡，洪都新府。星分翼轸，地接衡庐。襟三江而带五湖，控蛮荆而引瓯越。",
    "物华天宝，龙光射牛斗之墟；人杰地灵，徐孺下陈蕃之榻。雄州雾列，俊采星驰。",
    "台隍枕夷夏之交，宾主尽东南之美。都督阎公之雅望，棨戟遥临；宇文新州之懿范，襜帷暂驻。",
    "十旬休假，胜友如云；千里逢迎，高朋满座。腾蛟起凤，孟学士之词宗；紫电青霜，王将军之武库。",
    "家君作宰，路出名区；童子何知，躬逢胜饯。",
    "\n",
    "时维九月，序属三秋。潦水尽而寒潭清，烟光凝而暮山紫。",
    "俨骖騑于上路，访风景于崇阿。临帝子之长洲，得仙人之旧馆。",
    "层峦耸翠，上出重霄；飞阁流丹，下临无地。鹤汀凫渚，穷岛屿之萦回；桂殿兰宫，即冈峦之体势。",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "\0"
};

int _stcode = -1;

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


int out_line(int ws[2], int row, int col, char *line) {
    int len = strlen(line);
    int width = 3;
    char word[5] = {'\0'};

    for(int i=0; i<len; i+=width) {
        if (line[i]>0) {
            while(i<len) {
                if (line[i]<0)
                    break;
                mvoutc(row++, col, line[i]);
                if (row > ws[0])
                    scroll_down(1);
                i++;
            }
        }

        strncpy(word, line+i, 3);
        move(row++, col);
        if(row > ws[0])
            scroll_down(1);
        printf("%s", word);
        fflush(stdout);
        usleep(180000);
        
    }
    return row;
}

/*
struct thd_args {
    int ws[2];
    int row;
    int col;
    char *line;
};

void * thread_a (void* val) {
    struct thd_args *tha = (struct thd_args*)val;
    out_line(tha->ws, tha->row, tha->col, tha->line);
    _stcode = 0;

    return NULL;
}
*/


int main(int argc, char *argv[]) {

    enable_scroll();

    int ws[2];
    winsize(ws);

    clear();
    
    int r=0;
    out_line(ws, 3, 8, _luoxia);
    r = out_line(ws, 3, 20, _quishui);
    if (r > ws[0])
        scroll_down(1);
    
    mvoutc(r, 0, '\n');


	return 0;
}

