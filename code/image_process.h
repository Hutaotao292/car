#ifndef image_process_h
#define image_process_h
#include "zf_common_typedef.h"

extern int max_middle_line_height;
extern int bottom_start_x;
extern int bottom_end_x;
extern int middle_line_index;

extern uint8 left_border[100][2];  // 左边界
extern uint8 right_border[100][2];  // 右边界
extern uint8 middle_line[100][2];  // 中线
extern uint8 middle_line_single[100][2];  // 中线单Y
extern uint8 left_distance[100][2];//
extern uint8 right_distance[100][2];//
extern uint8 left_distance_line[100][2];
extern uint8 right_distance_line[100][2];
extern uint8 distances[100];
extern uint8 distance_middle_line[100][2];


void calculate_contrast_x8(uint8 *dst_image, const uint8 *src_image, int16 image_w, int16 image_h);

void compress_image(uint8 *Dst, const uint8 *Src);

void cover_car_head();

uint8 get_otsu_threshold(uint16 x1, uint16 y1, uint16 x2, uint16 y2, const uint8 *image);

int get_border_line(int detect_count_max);

void bottom_start_end_x_get();

void tft180_draw_border_line(uint16 x, uint16 y, const uint8 line[][2], uint16 color);

#endif //image_process_h

