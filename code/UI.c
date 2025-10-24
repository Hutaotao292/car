//
// Simple UI for motor control
// Created based on the original UI.c
//

#include "UI.h"
#include "motor.h"
#include "servo.h"
#include "switch.h"
// 如果有显示屏，可加上屏幕库，例如 TFT
//#include "tft180.h"

#define FONT_W 8
#define FONT_H 16

char buf[32];
UI_item items[2][5];  // 简化版，2页，每页5个参数
int8 cursor_pos = 0;
int8 ui_state = 0; // 0: 光标移动, 1: 修改数值
int8 key_pressed = 0;

// 预留变量，用于控制电机和舵机
extern int motor_speed;   // motor.c 中的速度变量
extern int servo_angle;   // servo.c 中的角度变量

typedef struct {
    char name[12];
    int type;
    union {
        int *int_p;
        float *float_p;
        char *char_p;
        void (*func_p)(void);
    } var_p;
} UI_item;

// ----------------- UI item 初始化 -----------------
void UI_item_init(UI_item *item, const char *name, int type, void *var_ptr){
    strcpy(item->name, name);
    item->type = type;
    switch(type){
        case INT32:
            item->var_p.int_p = (int *)var_ptr;
            break;
        case FUNC:
            item->var_p.func_p = var_ptr;
            break;
    }
}

// ----------------- 获取和设置值 -----------------
int UI_item_get_value(UI_item *item){
    if(item->type == INT32) return *(item->var_p.int_p);
    return 0;
}

void UI_item_set_value(UI_item *item, int value){
    if(item->type == INT32) *(item->var_p.int_p) = value;
}

// ----------------- 显示名字和数值 -----------------
// 如果你有 TFT 屏，可替换为实际显示函数
void UI_item_show_name(UI_item *item, int x, int y){
    // 预留显示功能
    // tft_show_string(x, y, item->name);
}

void UI_item_show_value(UI_item *item, int x, int y){
    // 预留显示功能
    // sprintf(buf, "%d", UI_item_get_value(item));
    // tft_show_string(x, y, buf);
}

// ----------------- UI 初始化 -----------------
void UI_init(){
    // 清空所有item
    for(int i=0;i<2;i++){
        for(int j=0;j<5;j++){
            items[i][j].type = EMPTY;
        }
    }

    // 第一页: motor 控制
    UI_item_init(&items[0][0], "MotorSpeed", INT32, &motor_speed);
    UI_item_init(&items[0][1], "ServoAngle", INT32, &servo_angle);
    UI_item_init(&items[0][2], "FuncTest", FUNC, NULL); // 预留调用函数

    // 第二页: 预留参数
    UI_item_init(&items[1][0], "Param1", INT32, NULL);
    UI_item_init(&items[1][1], "Param2", INT32, NULL);
}

// ----------------- UI 显示 -----------------
void UI_show(int page){
    for(int i=0;i<5;i++){
        if(items[page][i].type != EMPTY){
            UI_item_show_name(&items[page][i], FONT_W, FONT_H*i + FONT_H);
            UI_item_show_value(&items[page][i], FONT_W*6, FONT_H*i + FONT_H);
        }
    }
}

// ----------------- UI 按键处理 -----------------
void UI_key_process(int key_up, int key_down, int key_forward, int key_back){
    UI_item *item = &items[0][cursor_pos]; // 简化，只操作第一页
    if(key_up){
        if(ui_state == 0) cursor_pos = (cursor_pos + 4) % 5; // 光标上移
        else UI_item_set_value(item, UI_item_get_value(item)+1); // 增加数值
    }
    if(key_down){
        if(ui_state == 0) cursor_pos = (cursor_pos + 1) % 5; // 光标下移
        else UI_item_set_value(item, UI_item_get_value(item)-1); // 减少数值
    }
    if(key_forward) ui_state = (ui_state + 1) % 2; // 切换模式
    if(key_back) {
        // 这里可以写返回或者执行函数
        if(item->type == FUNC && item->var_p.func_p != NULL){
            item->var_p.func_p();
        }
    }
}
