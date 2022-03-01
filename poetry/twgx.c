#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>


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
    "披绣闼，俯雕甍，山原旷其盈视，川泽纡其骇瞩。闾阎扑地，钟鸣鼎食之家；舸舰迷津，青雀黄龙之舳。",
    "云销雨霁，彩彻区明。落霞与孤鹜齐飞，秋水共长天一色。渔舟唱晚，响穷彭蠡之滨，雁阵惊寒，声断衡阳之浦。",
    "\n",
    "遥襟甫畅，逸兴遄飞。爽籁发而清风生，纤歌凝而白云遏。睢园绿竹，气凌彭泽之樽；邺水朱华，光照临川之笔。四美具，二难并。",
    "穷睇眄于中天，极娱游于暇日。天高地迥，觉宇宙之无穷；兴尽悲来，识盈虚之有数。望长安于日下，目吴会于云间。",
    "地势极而南溟深，天柱高而北辰远。关山难越，谁悲失路之人；萍水相逢，尽是他乡之客。怀帝阍而不见，奉宣室以何年？",
    "\n",
    "嗟乎！时运不齐，命途多舛。冯唐易老，李广难封。屈贾谊于长沙，非无圣主；窜梁鸿于海曲，岂乏明时？",
    "所赖君子见机，达人知命。老当益壮，宁移白首之心？穷且益坚，不坠青云之志。酌贪泉而觉爽，处涸辙以犹欢。",
    "北海虽赊，扶摇可接；东隅已逝，桑榆非晚。孟尝高洁，空余报国之情；阮籍猖狂，岂效穷途之哭！",
    "\n",
    "勃，三尺微命，一介书生。无路请缨，等终军之弱冠；有怀投笔，慕宗悫之长风。舍簪笏于百龄，奉晨昏于万里。",
    "非谢家之宝树，接孟氏之芳邻。他日趋庭，叨陪鲤对；今兹捧袂，喜托龙门。杨意不逢，抚凌云而自惜；钟期既遇，奏流水以何惭？",
    "呜乎！胜地不常，盛筵难再；兰亭已矣，梓泽丘墟。临别赠言，幸承恩于伟饯；登高作赋，是所望于群公。敢竭鄙怀，恭疏短引；一言均赋，四韵俱成。请洒潘江，各倾陆海云尔：",
    "--sst",
    "滕王高阁临江渚，",
    "\n",
    "佩玉鸣鸾罢歌舞。",
    "\n",
    "画栋朝飞南浦云，",
    "\n",
    "珠帘暮卷西山雨。",
    "\n",
    "闲云潭影日悠悠，",
    "\n",
    "物换星移几度秋。",
    "\n",
    "阁中帝子今何在？",
    "\n",
    "槛外长江空自流。",
    "--sse",
    "\0"
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

#define DEF_DELAY       150000


int main(int argc, char *argv[]) {

    int delay = DEF_DELAY;

    if (argc > 1) {
        delay = atoi(argv[1]);
        if (delay < 5000 || delay > 999999) {
            delay = DEF_DELAY;
        }
    }

    enable_scroll();

    int ws[2];
    winsize(ws);

    clear();

    char word[5] = {'\0'};
    int i = 0;
    int row = 1;
    int col = ws[1] - 1;
    int scrn = 0;
    char *p = NULL;

    int out_row = ws[0]-2;
    int top = 1;

    while(_twgx[i][0]!='\0') {
        if (_twgx[i][0] == '\n') {
            col -= 4;
            row = top;
            i++;
            continue;
        } else if (strcmp(_twgx[i], "--sst")==0) {
            //col = ws[1]-1;
            top = ws[0]/2 - 4;
            row = top;
            col -= 4;
            //scrn++;
            //scroll_down(out_row+2);
            i++;
            continue;
        } else if (strcmp(_twgx[i], "--sse") == 0) {
            //col = ws[1] - 1;
            col -= 4;
            top = 1;
            row = top;
            i++;
            continue;
        }
        p = _twgx[i];
        for(int k=0; k<strlen(p);) {
            if (row >= out_row) {
                row = top;
                col -= 3;
            }
            if (col < 4) {
                col = ws[1] - 1;
                scrn ++;
                scroll_down(ws[0]);
                row = top;
            }

            if (p[k]>0) {
                mvoutc(row+1, col, p[k]);
                k++;
            } else {
                strncpy(word, p+k, 3);
                mvoutstr(row+1, col, word);
                k+=3;
            }
            row++;
            usleep(delay);
        }

        i++;
    }

    scroll_down(1);
    mvoutc(ws[0], 0, '\0');

	return 0;
}

