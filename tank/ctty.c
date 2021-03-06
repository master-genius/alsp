#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>


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

void tty_reset(){
    printf("\033c");
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

void scroll_zone(int start, int end) {
    printf("\033[%d;%dr", start, end);
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

    clear();

    int ws[2];
    winsize(ws);

    int start = 5;
    int row_end = 5;

    scroll_zone(start, row_end+start);

    int row = start;
    char c= 'a';
    for (int i=0; i<5; i++) {
        mvoutc(row, 0, c++);
        scroll_down(1);
        row++;
        if (row > row_end)
            row = start;
        usleep(250000);
    }

    mvoutc(row_end+start+2, 0, '\0');

    //tty_reset();
	return 0;
}

