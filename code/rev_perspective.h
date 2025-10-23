//
// Created by RUPC on 2022/9/20.
//
#ifndef SMART_CAR_CAMERA_REV_PERSPECTIVE_H
#define SMART_CAR_CAMERA_REV_PERSPECTIVE_H
#include "zf_common_typedef.h"
#include "zf_device_mt9v03x.h"

#define RESULT_H 45//结果图行列
#define RESULT_W 47

extern uint8 gray_pers_image[RESULT_H][RESULT_W];
extern uint8 contrast_pers_image[RESULT_H][RESULT_W];
extern uint8 binary_pers_image[RESULT_H][RESULT_W];
extern uint8 gray_binary_pers_image[RESULT_H][RESULT_W];

void InitLookupTable(void);

void ImagePerspective(void);

#endif //SMART_CAR_CAMERA_REV_PERSPECTIVE_H
