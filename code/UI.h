//
// UI.h - Simplified user interface header
// Compatible with simplified ui.c
//

#ifndef _UI_H_
#define _UI_H_

#include <stdint.h>
#include <string.h>

// ---------------------- 类型定义 ----------------------
typedef int8_t  int8;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

// ---------------------- UI 元素类型 ----------------------
#define EMPTY   0   // 空项
#define INT32   1   // 整型变量
#define FUNC    2   // 函数项

// ---------------------- UI 元素结构体 ----------------------
typedef struct {
    char name[12];  // 参数名（显示）
    int type;       // 参数类型
    union {
        int *int_p;             // 整型变量指针
        float *float_p;         // 浮点型变量指针（暂未用）
        char *char_p;           // 字符型变量指针（预留）
        void (*func_p)(void);   // 函数指针（按键触发）
    } var_p;
} UI_item;

// ---------------------- 全局变量 ----------------------
extern char buf[32];
extern UI_item items[2][5];
extern int8 cursor_pos;
extern int8 ui_state;
extern int8 key_pressed;

// ---------------------- 函数声明 ----------------------
void UI_init(void);  // 初始化UI
void UI_show(int page);  // 显示UI
void UI_key_process(int key_up, int key_down, int key_forward, int key_back); // 按键控制

// 这些是内部辅助函数，也可在其他模块中用
void UI_item_init(UI_item *item, const char *name, int type, void *var_ptr);
int  UI_item_get_value(UI_item *item);
void UI_item_set_value(UI_item *item, int value);
void UI_item_show_name(UI_item *item, int x, int y);
void UI_item_show_value(UI_item *item, int x, int y);

#endif  // _UI_H_
