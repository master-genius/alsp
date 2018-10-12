#ifndef MASTER_TANK_H
#define MASTER_TANK_H


#define TANK_TY_TANK    't'
#define TANK_TY_BOMB    'b'
#define TANK_TY_PRIZE   'p'
#define TANK_TY_ENEMY   'e'
#define TANK_TY_SHIEL   's'
#define TANK_TY_WALL    'r'

#define BOMB_A      '.'
#define BOMB_B      'o'
#define BOMB_C      '*'

#define BOMB_A_POWER    16
#define BOMB_B_POWER    32
#define BOMB_C_POWER    64

#define TANK_INIT_POWER 1024

#define BODY_END_LINE   8

#define BODY_MAX_LENG   16


#define MIN_ROWS    24
#define MIN_COLS    80




struct TRoot {
    char type;
    char name[256];
    char idstr[32];
};

struct Armour {
    struct TRoot *tr;

    char *body[];

    int body_end; //身体最大行截至

    int min_top;  //能到达的顶部最小位置
    int max_right; //能到达的右侧最小位置

    char weapon;  //武器类型
    int weapon_power;  //武器威力值

    int speed;  //移动速度

    int power;  //自身威力值

    int cur_row;
    int cur_col;

};

#define BODY_LINE   16

struct Tank {
    struct Armour * ar;

    char *body_up[];
    int body_up_end;

    char *body_left[];
    int body_left_end;

    char *body_down[];
    int body_down_end;

    char *body_right[];
    int body_right_end;

    char cur_ori; //当前方向


};

struct Aircraft {
    struct Armour *ar;


}




#endif
