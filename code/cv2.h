//
// Created by ashkore on 2023/9/22.
//

#ifndef CV2_H
#define CV2_H

#include "zf_device_mt9v03x.h"

void my_cv2_threshold(uint8 *image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint8 threshold, uint8 maxvalue);

void my_cv2_gaussian_blur_3x3(uint8 *dst_image, const uint8 *src_image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2,uint16 y2);

void my_cv2_calculateGradient_sobel_5x5(uint8 *dst_image, const uint8 *src_image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

void my_cv2_nonMaximumSuppression(uint8 *dst_image, const uint8 *src_image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

void my_cv2_doubleThreshold(uint8 *dst_image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint8 lowThreshold, uint8 highThreshold);

void my_cv2_checkConnectivity(uint8 *image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

void my_cv2_Canny(uint8 *dst_image, uint8 *src_image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

void my_cv2_gaussian_blur_5x5(uint8 *dst_image, const uint8 *src_image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

void my_cv2_enhanceAlphaBeta(uint8 *image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2, float alpha, float beta);

void my_cv2_fix_center_high_light(uint8 *image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2, float factor);

void my_cv2_equalization_hist(uint8 *dst_image, const uint8 *src_image, uint16 image_width, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

#endif //CV2_H
