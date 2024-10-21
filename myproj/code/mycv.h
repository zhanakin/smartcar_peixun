/*
 * mycv.h
 *
 *  Created on: 2024Äê10ÔÂ6ÈÕ
 *      Author: zzw
 */

#ifndef CODE_MYCV_H_
#define CODE_MYCV_H_

#include "zf_device_type.h"

#define WIDTH MT9V03X_W
#define HEIGHT MT9V03X_H
#define SAMPLED_WIDTH (MT9V03X_W/5)
#define SAMPLED_HEIGHT (MT9V03X_H/5)

#define WHITE 255
#define BLACK 0

extern uint8 (*process_input_image)[WIDTH];
extern uint8 (*process_output_image)[WIDTH];
extern uint8 g_thres_table[256];
extern uint8 g_thres_value;
extern int8 g_thres_bias;

extern uint8 foresight_up;
extern uint8 foresight_down;
extern float g_error;

uint8 image_fast_otsu();
uint8 otsuThreshold();
void image_update_thresvalue();

void get_binary(uint8 *input_image,uint8 *output_image);
void dilate(const unsigned char image[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH], const uint8 *kernel, uint8 kernel_size);
void convolution(const unsigned char image[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH], const uint8 *kernel, uint8 kernel_size);

void process_one_frame();
void tft_show_camera();
void width_show();
#endif /* CODE_MYCV_H_ */
