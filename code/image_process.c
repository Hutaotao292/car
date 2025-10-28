#include "image_process.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_tft180.h"
#include "rev_perspective.h"
#include "zf_common_font.h"
#include "switch.h"
uint8 left_border[100][2];  // 左边界
uint8 right_border[100][2];  // 右边界
int left_dirs[100];  // 左爬线方向
int right_dirs[100];  // 右爬线方向
uint8 middle_line[100][2];  // 中线
uint8 middle_line_single[100][2];  // 中线单Y
uint8 distance_middle_line[100][2];  // 最长白列中线
uint8 left_distance[100][2];  // 左距离
uint8 right_distance[100][2];  // 右距离
uint8 left_distance_line[100][2];  // 左距离线
uint8 right_distance_line[100][2];  // 右距离线
uint8 distances[100];  // 距离

int max_middle_line_height = 0;
int left_border_index = 0;
int right_border_index = 0;
int left_skip_index = 0;
int right_skip_index = 0;
int middle_line_index = 0;
int middle_line_single_index = 0;
int distance_index = 0;
int distance_middle_line_index = 0;
int narrow_line_index = 0;

int bottom_start_x;
int bottom_end_x;

uint8 road_color = 0;  // 道路颜色
uint8 border_color = 1;  // 边界颜色


void calculate_contrast_x8(uint8 *dst_image, const uint8 *src_image, int16 image_w, int16 image_h) {
    for (int y = 1; y < image_h - 1; y++) {
        for (int x = 1; x < image_w - 1; x++) {
            // 采样周围8个像素
            int pixel1 = src_image[y * image_w + (x - 1)];
            int pixel2 = src_image[y * image_w + (x + 1)];
            int pixel3 = src_image[(y - 1) * image_w + x];
            int pixel4 = src_image[(y + 1) * image_w + x];
            int pixel5 = src_image[(y - 1) * image_w + (x - 1)];
            int pixel6 = src_image[(y - 1) * image_w + (x + 1)];
            int pixel7 = src_image[(y + 1) * image_w + (x - 1)];
            int pixel8 = src_image[(y + 1) * image_w + (x + 1)];


            // 计算像素之间的差异和总和
            int diff1 = pixel1 - pixel2;
            int diff2 = pixel3 - pixel4;
            int diff3 = pixel5 - pixel6;
            int diff4 = pixel7 - pixel8;
            int sum1 = pixel1 + pixel2;
            int sum2 = pixel3 + pixel4;
            int sum3 = pixel5 + pixel6;
            int sum4 = pixel7 + pixel8;

            // 计算差比和并放大255倍
            // (sum != 0)避免除以0
            int contrast1 = (sum1 != 0) ? abs(diff1 * 96 / sum1) : 0;
            int contrast2 = (sum2 != 0) ? abs(diff2 * 96 / sum2) : 0;
            int contrast3 = (sum3 != 0) ? abs(diff3 * 32 / sum3) : 0;
            int contrast4 = (sum4 != 0) ? abs(diff4 * 32 / sum4) : 0;
            dst_image[y * image_w + x] = contrast1 + contrast2 + contrast3 + contrast4;
        }
    }
}

void compress_image(uint8 *Dst, const uint8 *Src){
    uint16 i;
    uint16 j;
    uint16 compress_width = MT9V03X_W / 2;
    uint16 compress_height = MT9V03X_H / 2;
    for(i = 0; i < compress_height; i++){
        for(j = 0; j < compress_width; j++){
            Dst[i * compress_width + j] = (Src[i*2*MT9V03X_W + j*2] + Src[i*2*MT9V03X_W + j*2+1] + Src[(i*2+1)*MT9V03X_W + j*2] + Src[(i*2+1)*MT9V03X_W + j*2+1]) / 4;
        }
    }
}

void cover_car_head(){
    for(int x = 29; x < 65; x++){
        for(int y = 39; y < 45; y++){
            gray_image[y][x] = gray_image[y-1][x];
        }
    }
}

uint8 get_otsu_threshold(uint16 x1, uint16 y1, uint16 x2, uint16 y2, const uint8 *image) {
#define GrayScale 256
    int Pixel_Max = 0;
    int Pixel_Min = 255;
    uint16 width = x2 - x1;
    uint16 height = y2 - y1;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j;
    int pixelSum = width * height/4;
    uint8 threshold = 120;
    const uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = y1; i < y2; i += 2)
    {
        for (j = x1; j < x2; j += 2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
            if(data[i * width + j]>Pixel_Max)   Pixel_Max=data[i * width + j];
            if(data[i * width + j]<Pixel_Min)   Pixel_Min=data[i * width + j];
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = Pixel_Min; i < Pixel_Max; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = Pixel_Min; j < Pixel_Max; j++)
    {

        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += (float)j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

        w1 = 1 - w0;
        u1tmp= gray_sum / pixelSum - u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
//        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = (float)(w0 *w1* (u0 - u1)* (u0 - u1)) ;
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = (uint8)j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
    }

    return threshold;
}

int get_border_line(int detect_count_max) {
    int left_x, left_y, right_x, right_y;  // 爬线时坐标
    left_border_index = 0;  // 爬线时左边界索引
    right_border_index = 0;  // 爬线时右边界索引
    left_skip_index = 0;  // 爬线时左边界跳过索引
    right_skip_index = 0;  // 爬线时右边界跳过索引
    int left_dir;  // 爬线时左边界方向
    int right_dir;  // 爬线时右边界方向
    int detect_count;  // 爬线时检测次数




    memset(left_border, 0, sizeof(left_border));
    memset(right_border, 0, sizeof(right_border));
    memset(left_dirs, 0, sizeof(left_dirs));
    memset(right_dirs, 0, sizeof(right_dirs));
    // 初始边界点在道路内
    int first_x;
    int first_y = 43;
    for (first_x = (bottom_start_x + bottom_end_x) / 2; first_x > 0; first_x--) {
        if (binary_image[first_y][first_x] == road_color && binary_image[first_y][first_x - 1] != road_color) {
            left_border[left_border_index][0] = first_x;
            left_border[left_border_index][1] = first_y;
            break;
        }
    }

    for (first_x = (bottom_start_x + bottom_end_x)/2; first_x < 94; first_x++) {
        if (binary_image[first_y][first_x] == road_color && binary_image[first_y][first_x + 1] != road_color) {
            right_border[right_border_index][0] = first_x;
            right_border[right_border_index][1] = first_y;
            break;
        }
    }
    left_x = left_border[left_border_index][0];
    left_y = left_border[left_border_index][1];
    right_x = right_border[right_border_index][0];
    right_y = right_border[right_border_index][1];
    left_dir = -1;  // 重置左边界方向
    right_dir = -1;  // 重置右边界方向
    for(detect_count = 0; detect_count < detect_count_max; detect_count++) {
        // 左边界
        if(detect_count > 15 && sqrt((left_x - left_border[left_border_index-5][0]) * (left_x - left_border[left_border_index-5][0]) + (left_y - left_border[left_border_index-5][1]) * (left_y - left_border[left_border_index-5][1])) < 4){
            if(left_skip_index == 0){
                left_skip_index = left_border_index;
            }
        } else if(left_y < 3 || (detect_count > 15 && left_x < 4)) {
            if(left_skip_index == 0){
                left_skip_index = left_border_index;
            }
        } else if (left_dir != 3 && binary_image[left_y - 1][left_x + 1] == road_color &&
                   binary_image[left_y - 1][left_x] == border_color){  // 右上黑，方向3，正上白
            left_y = left_y - 1;
            left_x = left_x + 1;
            left_dir = 6;
        } else if (left_dir != 4 && binary_image[left_y][left_x + 1] == road_color &&
                   binary_image[left_y - 1][left_x + 1] == border_color){  // 正右黑，方向4，右上白
            left_x = left_x + 1;
            left_dir = 5;
        } else if (left_dir != 1 && binary_image[left_y - 1][left_x] == road_color &&
                   binary_image[left_y - 1][left_x - 1] == border_color){  // 正上黑，方向1，左上白
            left_y = left_y - 1;
            left_dir = 0;
        } else if (left_dir != 2 && binary_image[left_y - 1][left_x - 1] == road_color &&
                   binary_image[left_y][left_x - 1] == border_color){  // 左上黑，方向2，正右白
            left_y = left_y - 1;
            left_x = left_x - 1;
            left_dir = 7;
        } else if (left_dir !=5 && binary_image[left_y][left_x - 1] == road_color &&
                   binary_image[left_y + 1][left_x - 1] == border_color){  // 正左黑，方向5，左下白
            left_x = left_x - 1;
            left_dir = 4;
        } else if (left_dir != 6 && binary_image[left_y + 1][left_x - 1] == road_color &&
                   binary_image[left_y + 1][left_x] == border_color){  // 左下黑，方向6，正下白
            left_y = left_y + 1;
            left_x = left_x - 1;
            left_dir = 3;
        } else if (left_dir != 7 && binary_image[left_y + 1][left_x + 1] == road_color &&
                   binary_image[left_y][left_x + 1] == border_color){  // 右下黑，方向7，正右白
            left_y = left_y + 1;
            left_x = left_x + 1;
            left_dir = 2;
        } else if (left_dir != 0 && binary_image[left_y + 1][left_x] == road_color &&
                   binary_image[left_y + 1][left_x + 1] == border_color){  // 正下黑，方向8，右下白
            if(left_skip_index == 0){
                left_skip_index = left_border_index;
            }
        } else {
            break;
        }

        left_border_index++;
        left_border[left_border_index][0] = left_x;
        left_border[left_border_index][1] = left_y;
        left_dirs[left_border_index] = left_dir;

        if(left_x == right_x && left_y == right_y){
            break;
        }

        // 右边界
        if(detect_count > 15 && sqrt((right_x - right_border[right_border_index-5][0]) * (right_x - right_border[right_border_index-5][0]) + (right_y - right_border[right_border_index-5][1]) * (right_y - right_border[right_border_index-5][1])) < 4){
            if(right_skip_index == 0){
                right_skip_index = right_border_index;
            }
        } else if(right_y < 3 || (detect_count > 15 && right_x > 90)) {
            if(right_skip_index == 0){
                right_skip_index = right_border_index;
            }
        } else if(right_dir != 2 && binary_image[right_y - 1][right_x - 1] == road_color &&
                  binary_image[right_y - 1][right_x] == border_color) {  // 左上黑，方向2，正上白
            right_y = right_y - 1;
            right_x = right_x - 1;
            right_dir = 7;
        } else if(right_dir != 5 && binary_image[right_y][right_x - 1] == road_color &&
                  binary_image[right_y - 1][right_x - 1] == border_color){  // 正左黑，方向5，正上白
            right_x = right_x - 1;
            right_dir = 4;
        } else if(left_dir != 1 && binary_image[right_y - 1][right_x] == road_color &&
                  binary_image[right_y - 1][right_x + 1] == border_color){  // 正上黑，方向1，右上白
            right_y = right_y - 1;
            right_dir = 0;
        } else if (right_x < 94 && right_dir != 3 && binary_image[right_y - 1][right_x + 1] == road_color &&
                   binary_image[right_y][right_x + 1] == border_color){  // 右上黑，方向3，正右白
            right_y = right_y - 1;
            right_x = right_x + 1;
            right_dir = 6;
        } else if(right_dir != 4 && binary_image[right_y][right_x + 1] == road_color &&
                  binary_image[right_y + 1][right_x + 1] == border_color){  // 正右黑，方向4，正下白
            right_x = right_x + 1;
            right_dir = 5;
        } else if(right_dir != 6 && binary_image[right_y + 1][right_x - 1] == road_color &&
                  binary_image[right_y][right_x - 1] == border_color){  // 左下黑，方向6，正左白
            right_y = right_y + 1;
            right_x = right_x - 1;
            right_dir = 3;
        } else if(right_dir != 7 && binary_image[right_y + 1][right_x + 1] == road_color &&
                  binary_image[right_y + 1][right_x] == border_color) {  // 右下黑，方向7，正下白
            right_y = right_y + 1;
            right_x = right_x + 1;
            right_dir = 2;
        } else if(right_dir != 0 && binary_image[right_y + 1][right_x] == road_color &&
                  binary_image[right_y + 1][right_x - 1] == border_color){  // 正下黑，方向8，左下白
            if(right_skip_index == 0){
                right_skip_index = right_border_index;
            }
        } else {
            break;
        }
        right_border_index++;
        right_border[right_border_index][0] = right_x;
        right_border[right_border_index][1] = right_y;
        right_dirs[right_border_index] = right_dir;

        if(left_x == right_x && left_y == right_y){
            break;
        }
    }
    detect_count_max = detect_count;

    // 计算中线
    memset(middle_line, 0, sizeof(middle_line));
    middle_line_index = 0;
    for(int q = 0; q < detect_count_max; q++){
        middle_line[q][0] = (left_border[q][0] + right_border[q][0]) / 2;
        middle_line[q][1] = (left_border[q][1] + right_border[q][1]) / 2;
        middle_line_index++;
    }

    memset(middle_line_single, 0, sizeof(middle_line_single));
    middle_line_single_index = 0;
    int middle_line_current_y = -1;
    for(int q = 0; q < detect_count_max; q++){
        if(middle_line[q][1] != middle_line_current_y && middle_line[q][1] != 0){
            middle_line_single[middle_line_single_index][0] = middle_line[q][0];
            middle_line_single[middle_line_single_index][1] = middle_line[q][1];
            middle_line_single_index++;
            middle_line_current_y = middle_line[q][1];
        }
    }

    return detect_count_max;
}

void bottom_start_end_x_get(){
    int maxStartIndex = -1;
    int maxEndIndex = -1;
    int currentStartIndex = -1;
    int currentEndIndex = -1;
    int maxLength = 0;
    int currentLength = 0;
    for (int i = 0; i < 94; i++) {
        if (gray_binary_image[43][i] == 1) {
            if (currentStartIndex == -1) {
                currentStartIndex = i;
            }
            currentEndIndex = i;
            currentLength = currentEndIndex - currentStartIndex;
            if (currentLength > maxLength) {
                maxLength = currentLength;
                maxStartIndex = currentStartIndex;
                maxEndIndex = currentEndIndex;
            }
        } else {
            currentStartIndex = -1;
        }
    }
    bottom_start_x = maxStartIndex;
    bottom_end_x = maxEndIndex;
}

void tft180_draw_border_line(uint16 x, uint16 y, const uint8 line[][2], uint16 color){
    uint16 i = 0;
    while(line[i][0] || line[i][1]) {
        tft180_draw_point(x + line[i][0], y + line[i][1], color);
        i++;
    }
}