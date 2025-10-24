//
// Simple UI for motor control
// Created based on the original UI.c
//

#include "UI.h"
#include "motor.h"
#include "servo.h"
#include "switch.h"
// �������ʾ�����ɼ�����Ļ�⣬���� TFT
//#include "tft180.h"

#define FONT_W 8
#define FONT_H 16

char buf[32];
UI_item items[2][5];  // �򻯰棬2ҳ��ÿҳ5������
int8 cursor_pos = 0;
int8 ui_state = 0; // 0: ����ƶ�, 1: �޸���ֵ
int8 key_pressed = 0;

// Ԥ�����������ڿ��Ƶ���Ͷ��
extern int motor_speed;   // motor.c �е��ٶȱ���
extern int servo_angle;   // servo.c �еĽǶȱ���

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

// ----------------- UI item ��ʼ�� -----------------
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

// ----------------- ��ȡ������ֵ -----------------
int UI_item_get_value(UI_item *item){
    if(item->type == INT32) return *(item->var_p.int_p);
    return 0;
}

void UI_item_set_value(UI_item *item, int value){
    if(item->type == INT32) *(item->var_p.int_p) = value;
}

// ----------------- ��ʾ���ֺ���ֵ -----------------
// ������� TFT �������滻Ϊʵ����ʾ����
void UI_item_show_name(UI_item *item, int x, int y){
    // Ԥ����ʾ����
    // tft_show_string(x, y, item->name);
}

void UI_item_show_value(UI_item *item, int x, int y){
    // Ԥ����ʾ����
    // sprintf(buf, "%d", UI_item_get_value(item));
    // tft_show_string(x, y, buf);
}

// ----------------- UI ��ʼ�� -----------------
void UI_init(){
    // �������item
    for(int i=0;i<2;i++){
        for(int j=0;j<5;j++){
            items[i][j].type = EMPTY;
        }
    }

    // ��һҳ: motor ����
    UI_item_init(&items[0][0], "MotorSpeed", INT32, &motor_speed);
    UI_item_init(&items[0][1], "ServoAngle", INT32, &servo_angle);
    UI_item_init(&items[0][2], "FuncTest", FUNC, NULL); // Ԥ�����ú���

    // �ڶ�ҳ: Ԥ������
    UI_item_init(&items[1][0], "Param1", INT32, NULL);
    UI_item_init(&items[1][1], "Param2", INT32, NULL);
}

// ----------------- UI ��ʾ -----------------
void UI_show(int page){
    for(int i=0;i<5;i++){
        if(items[page][i].type != EMPTY){
            UI_item_show_name(&items[page][i], FONT_W, FONT_H*i + FONT_H);
            UI_item_show_value(&items[page][i], FONT_W*6, FONT_H*i + FONT_H);
        }
    }
}

// ----------------- UI �������� -----------------
void UI_key_process(int key_up, int key_down, int key_forward, int key_back){
    UI_item *item = &items[0][cursor_pos]; // �򻯣�ֻ������һҳ
    if(key_up){
        if(ui_state == 0) cursor_pos = (cursor_pos + 4) % 5; // �������
        else UI_item_set_value(item, UI_item_get_value(item)+1); // ������ֵ
    }
    if(key_down){
        if(ui_state == 0) cursor_pos = (cursor_pos + 1) % 5; // �������
        else UI_item_set_value(item, UI_item_get_value(item)-1); // ������ֵ
    }
    if(key_forward) ui_state = (ui_state + 1) % 2; // �л�ģʽ
    if(key_back) {
        // �������д���ػ���ִ�к���
        if(item->type == FUNC && item->var_p.func_p != NULL){
            item->var_p.func_p();
        }
    }
}
