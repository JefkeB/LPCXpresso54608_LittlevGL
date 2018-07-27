//
//
//
#ifndef IMAGE_BUF_INC_DRAW_UTIL_H_
#define IMAGE_BUF_INC_DRAW_UTIL_H_

#include <stdint.h>

extern const uint16_t *frame_buf_addr[];

#include "lvgl.h"
void Draw16BPP565Pixel(uint16_t x, uint16_t y, lv_color_t color);
uint16_t *FrameAddr(uint8_t idx);

#endif /* IMAGE_BUF_INC_DRAW_UTIL_H_ */
