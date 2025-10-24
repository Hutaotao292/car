//
// UI.h - Simplified user interface header
// Compatible with simplified ui.c
//

#ifndef _UI_H_
#define _UI_H_

#include <stdint.h>
#include <string.h>

// ---------------------- ���Ͷ��� ----------------------
typedef int8_t  int8;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

// ---------------------- UI Ԫ������ ----------------------
#define EMPTY   0   // ����
#define INT32   1   // ���ͱ���
#define FUNC    2   // ������

// ---------------------- UI Ԫ�ؽṹ�� ----------------------
typedef struct {
    char name[12];  // ����������ʾ��
    int type;       // ��������
    union {
        int *int_p;             // ���ͱ���ָ��
        float *float_p;         // �����ͱ���ָ�루��δ�ã�
        char *char_p;           // �ַ��ͱ���ָ�루Ԥ����
        void (*func_p)(void);   // ����ָ�루����������
    } var_p;
} UI_item;

// ---------------------- ȫ�ֱ��� ----------------------
extern char buf[32];
extern UI_item items[2][5];
extern int8 cursor_pos;
extern int8 ui_state;
extern int8 key_pressed;

// ---------------------- �������� ----------------------
void UI_init(void);  // ��ʼ��UI
void UI_show(int page);  // ��ʾUI
void UI_key_process(int key_up, int key_down, int key_forward, int key_back); // ��������

// ��Щ���ڲ�����������Ҳ��������ģ������
void UI_item_init(UI_item *item, const char *name, int type, void *var_ptr);
int  UI_item_get_value(UI_item *item);
void UI_item_set_value(UI_item *item, int value);
void UI_item_show_name(UI_item *item, int x, int y);
void UI_item_show_value(UI_item *item, int x, int y);

#endif  // _UI_H_
